/*
In 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.

    Your query should output a table with a single column for the name of each person.
    There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
    Kevin Bacon himself should not be included in the resulting list.
*/

SELECT DISTINCT others.name
FROM
  (SELECT m.id
   FROM movies m
   INNER JOIN stars s ON s.movie_id = m.id
   INNER JOIN people p ON s.person_id = p.id
   WHERE p.name = 'Kevin Bacon' AND p.birth = 1958) kb
INNER JOIN
  (SELECT p.name, m.id
   FROM movies m
   INNER JOIN stars s ON s.movie_id = m.id
   INNER JOIN people p ON s.person_id = p.id) others
WHERE kb.id = others.id AND others.name != 'Kevin Bacon'
ORDER BY others.name ASC;