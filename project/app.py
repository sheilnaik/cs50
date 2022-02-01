from flask import Flask, render_template, request, redirect
import mysql.connector

db = mysql.connector.connect(
  host="localhost",
  user="root",
  password="",
  database='GDWins'
)
data = db.cursor(dictionary=True)

app = Flask(__name__)

win = {
    'analysts': [],
    'title': '',
    'date': '',
    'description': '',
    'primary_theme': '',
    'secondary_themes': []
}

@app.route("/")
def index():
    return render_template("index.html")


@app.route("/add/analyst", methods=['GET', 'POST'])
def analyst():
    if request.method == "GET":
        analyst_list = list(get_formatted_analyst_list().keys())
        return render_template("add/analyst.html", analyst_list=analyst_list)
    else:
        win['analysts'] = request.form.getlist('analyst_name[]')
        return redirect("/add/description")


@app.route("/add/description", methods=['GET', 'POST'])
def description():
    if request.method == "GET":
        return render_template("add/description.html")
    else:
        win['title'] = request.form.get('win_title')
        win['date'] = request.form.get('win_date')
        win['description'] = request.form.get('win_description')
        return redirect("/add/theme")


@app.route("/add/theme", methods=['GET', 'POST'])
def theme():
    theme_list = get_items('theme')

    if request.method == "GET":
        return render_template("add/theme.html", theme_list=theme_list)
    else:
        win['primary_theme'] = request.form.get('primary_theme')
        win['secondary_themes'] = request.form.getlist('secondary_themes')
        return redirect("/add/approach")


@app.route("/add/approach", methods=['GET', 'POST'])
def approach():
    approach_list = get_items('approach')

    if request.method == "GET":
        return render_template("add/approach.html", approach_list=approach_list)
    else:
        win['primary_approach'] = request.form.get('primary_approach')
        win['secondary_approaches'] = request.form.getlist('secondary_approaches')
        return redirect("/add/approach")


@app.route("/add/impact")
def impact():
    # TODO: Add impact content
    return None


@app.route("/add/roi")
def roi():
    # TODO: Add ROI content
    return None


@app.route("/add/technology")
def technology():
    # TODO: Add technology content
    return None


@app.route("/review")
def review():
    # TODO: Add review stuff
    return None

def get_formatted_analyst_list():
    analyst_list = {}

    data.execute("SELECT UUID, Name, Team FROM HeadcountCS50")
    headcount = data.fetchall()
    for analyst in headcount:
        analyst_list[f"{analyst['Name']} ({analyst['Team']})"] = analyst['UUID']
    
    return analyst_list


def get_items(item_name):
    item_list = []

    if item_name == 'theme':
        key_name = 'theme_name'
        query = f"SELECT {key_name} FROM theme"
    elif item_name == 'approach':
        key_name = 'approach_name'
        query = f"SELECT {key_name} FROM approach"

    data.execute(query)
    rows = data.fetchall()
    for row in rows:
        item_list.append(row[key_name])
    return item_list