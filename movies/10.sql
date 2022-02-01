/*
In 10.sql, write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.

    Your query should output a table with a single column for the name of each person.
*/

SELECT DISTINCT p.name
FROM people p
INNER JOIN directors d ON p.id = d.person_id
INNER JOIN ratings r ON r.movie_id = d.movie_id
WHERE r.rating >= 9.0;