# Company Wins Tool

## Summary
The Company Wins Tool is a web-based tool I am building for my company to help us track employee "wins". We are a large organization (1,500+ employees in a 19,000+ company) and the nature of our department often involves us collaborating with individuals from other department. This results in many "wins", or employee accomplishments, that we wish to highlight in senior-level manager meetings.

Keeping track of all these "wins" and analyzing them is labor-intensive. Previously, our organization built a VBA-based tool in Excel that we used to log employee wins in an Excel spreadsheet, but the tool was cumbersome to use and difficult to maintain. Using the skills learned in CS50, I am building an attractive, easy-to-use web-based tool that replaces the older VBA-based "wins" tool.

## Features
The new Company Wins Tool offers the following features:

- Web-based tool accessible from any browser
- Attractive and responsive designed based on Bootstrap 4.0
- Built using Flask, HTML, and CSS for the frontend and SQL for the backend database
- Javascript used for autocomplete of analyst names, the ability to add additional fields for additional analysts, and highlighting the active page
- Pages are dynamically built using Jinja blocks
- Fields are dynamically populated based on a SQL query and Jinja blocks 

## Upcoming Features
The Company Wins Tool will also offer the following features (still under development):

- Javascript to aid in validation, including a calendar "date picker" to ensure the date is always entered in the proper format
- Javascript to prevent the user to choose a theme or approach that they have already chosen as the primary
- The ability to add a "review" process for managers to approved reviewed wins, rather than wins going straight into the database

## A Note about Progress
In an effort to submit the project before the end of the year, I am submitting the project as a work-in-progress, although the majority of the development is complete. Additional pages need to be added for additional "win" categories, and once the final page is completed, the data will be saved into a SQL database where it can be analyzed later. Based on what I've completed already, I believe I have already met the project requirements.