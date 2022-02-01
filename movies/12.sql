/*
In 12.sql, write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.

    Your query should output a table with a single column for the title of each movie.
    You may assume that there is only one person in the database with the name Johnny Depp.
    You may assume that there is only one person in the database with the name Helena Bonham Carter.
*/

SELECT j.title
FROM
  (SELECT m.title
   FROM movies m
   INNER JOIN stars s ON s.movie_id = m.id
   INNER JOIN people p ON s.person_id = p.id
   WHERE p.name = 'Johnny Depp') j
INNER JOIN
  (SELECT m.title
   FROM movies m
   INNER JOIN stars s ON s.movie_id = m.id
   INNER JOIN people p ON s.person_id = p.id
   WHERE p.name = 'Helena Bonham Carter') h
WHERE j.title = h.title
ORDER BY j.title;