/*
In 6.sql, write a SQL query to determine the average rating of all movies released in 2012.

    Your query should output a table with a single column and a single row (plus optional header) containing the average rating.
*/

SELECT AVG(r.rating) AS average_rating FROM ratings r INNER JOIN movies m ON r.movie_id = m.id  WHERE m.year = 2012;