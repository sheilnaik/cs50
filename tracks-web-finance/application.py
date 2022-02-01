import os
from datetime import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    rows = db.execute("SELECT * FROM portfolios WHERE user_id = :user_id",
                      user_id=session["user_id"])

    portfolio_total = 0.0

    for row in rows:
        if row['symbol'] != 'CASH':
            ticker_data = lookup(row['symbol'])
            row['name'] = ticker_data['name']
            row['price'] = '${:.2f}'.format(ticker_data['price'])
            row['total'] = row['shares'] * ticker_data['price']
            row['formatted_total'] = '${:,.2f}'.format(row['shares'] * ticker_data['price'])
        else:
            row['shares'] = ''
            row['formatted_total'] = '${:,.2f}'.format(row['total'])

        portfolio_total += row['total']
        formatted_portfolio_total = '${:,.2f}'.format(portfolio_total)


    return render_template("index.html", rows=rows, formatted_portfolio_total=formatted_portfolio_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure shares were submitted
        elif not request.form.get("shares"):
            return apology("must provide shares", 403)

        # Ensure shares are positive
        elif int(request.form.get("shares")) < 1:
            return apology("must provide positive number of shares", 403)

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        result = transact('buy', symbol, shares)

        if result == 'not_enough_cash':
            return apology("not enough cash to buy", 403)

        # Redirect user to home page
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute("SELECT * FROM transactions WHERE user_id = :user_id",
                      user_id=session["user_id"])

    for row in rows:
        if row['symbol'] != 'CASH':
            ticker_data = lookup(row['symbol'])
            row['price'] = '${:,.2f}'.format(ticker_data['price'])

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        if not request.form.get("ticker"):
            return apology("must provide ticker symbol", 403)

        ticker = request.form.get("ticker")
        ticker_data = lookup(ticker)

        if not ticker_data:
            return apology("ticker symbol is invalid", 403)

        formatted_price = '${:,.2f}'.format(ticker_data['price'])

        return render_template("quoted.html", ticker_symbol=ticker_data['symbol'], company_name=ticker_data['name'], price=formatted_price)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        elif not request.form.get("password") == request.form.get("confirm_password"):
            return apology("passwords do not match", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        if len(rows) > 0:
            return apology("username already exists", 403)

        username = request.form.get("username")
        password_hash = generate_password_hash(request.form.get("password"))

        # Add user to database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :password_hash)",
                username=username,
                password_hash=password_hash)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Update cash value for user
        db.execute("INSERT INTO portfolios (user_id, symbol, total) VALUES (:user_id, 'CASH', 10000)",
                user_id=rows[0]["id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure shares was submitted
        elif not request.form.get("shares"):
            return apology("must provide shares", 403)

        # Ensure shares are positive
        elif int(request.form.get("shares")) < 1:
            return apology("must provide positive number of shares", 403)

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        result = transact('sell', symbol, shares)

        if result == 'selling_too_many_shares':
            return apology("trying to sell more shares than owned", 403)

        return redirect("/")
    else:
        return render_template("sell.html")


@app.route("/change_password", methods=["GET", "POST"])
def change_password():
    """Change password"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure old password was submitted
        if not request.form.get("old_password"):
            return apology("must provide old password", 403)

        # Ensure new password was submitted
        elif not request.form.get("new_password"):
            return apology("must provide new password", 403)

        # Ensure confirm new password was submitted
        elif not request.form.get("confirm_new_password"):
            return apology("must confirm new password", 403)

        # Ensure confirm new password was submitted
        elif request.form.get("new_password") != request.form.get("confirm_new_password"):
            return apology("new passwords do not match", 403)

        # Confirm password matches hash
        rows = db.execute("SELECT * FROM users WHERE id = :user_id",
                        user_id=session['user_id'])

        if not check_password_hash(rows[0]["hash"], request.form.get("old_password")):
            return apology("old password does not match", 403)

        # Update password hash
        new_password_hash = generate_password_hash(request.form.get("new_password"))

        db.execute("UPDATE users SET hash = :new_password_hash WHERE id = :user_id",
                        new_password_hash=new_password_hash,
                        user_id=session['user_id'])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change_password.html")


def transact(transaction_type, symbol, shares):

    if transaction_type == 'sell':
        shares *= -1

    ticker_data = lookup(symbol)

    if not ticker_data:
        return apology("ticker symbol is invalid", 403)

    price = float(ticker_data['price'])

    # Check if the user is buying or selling more than they can
    rows = db.execute("SELECT * FROM portfolios WHERE user_id = :user_id AND symbol = 'CASH'",
                      user_id=session["user_id"])

    if transaction_type == 'buy' and (shares * price) > rows[0]['total']:
        return 'not_enough_cash'

    if transaction_type == 'sell':
        rows = db.execute("SELECT * FROM portfolios WHERE user_id = :user_id AND symbol = :symbol",
                          user_id=session["user_id"],
                          symbol=symbol)

        if shares * -1 > rows[0]['shares']:
            return 'selling_too_many_shares'

    # Record transaction
    db.execute("INSERT INTO transactions (user_id, symbol, shares, price, transaction_time) VALUES (:user_id, :symbol, :shares, :price, :transaction_time)",
            user_id=session["user_id"],
            symbol=symbol,
            shares=shares,
            price=price,
            transaction_time=datetime.now())

    # Update portfolio
    ## Update cash
    rows = db.execute("SELECT * FROM portfolios WHERE user_id = :user_id AND symbol = 'CASH'",
                      user_id=session["user_id"])

    print(rows)
    current_cash_value = float(rows[0]["total"])
    new_cash_value = current_cash_value - (shares * price)

    db.execute("UPDATE portfolios SET total = :total WHERE user_id = :user_id AND symbol='CASH'",
            user_id=session["user_id"],
            total=new_cash_value)

    ## Check if ticker already exists in portfolio and update
    rows = db.execute("SELECT * FROM portfolios WHERE user_id = :user_id AND symbol = :symbol",
                      user_id=session["user_id"],
                      symbol=symbol)

    if len(rows) > 0:
        db.execute("UPDATE portfolios SET shares = :new_share_amt WHERE user_id = :user_id AND symbol = :symbol",
                new_share_amt=rows[0]['shares'] + shares,
                user_id=session["user_id"],
                symbol=symbol)
    else:
        db.execute("INSERT INTO portfolios (user_id, symbol, shares) VALUES (:user_id, :symbol, :shares)",
                user_id=session["user_id"],
                symbol=symbol,
                shares=shares)

    return 'success'


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
