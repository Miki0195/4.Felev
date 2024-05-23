SET SERVEROUTPUT ON;

drop table teli_eredmenyek;

CREATE TABLE teli_eredmenyek AS
SELECT * FROM vdani.teli_eredmenyek;

--1.Feladat
DELETE FROM teli_eredmenyek
WHERE nev IN (
    SELECT nev
    FROM teli_eredmenyek
    GROUP BY nev
    HAVING COUNT(DISTINCT olimpia) = 1
);

CREATE TABLE orszagok AS
SELECT * FROM vdani.orszagok;

--2.Feladat
DELETE FROM orszagok
WHERE gdp < (
    SELECT AVG(gdp)
    FROM orszagok t2
    WHERE t2.regio = orszagok.regio
);

--3.Feladat
SELECT * FROM  orszagok
WHERE terulet > (
    SELECT SUM(terulet)
    FROM orszagok t2
    WHERE t2.regio = orszagok.regio
    AND t2.tld <> orszagok.tld
);

--4.Feladat
CREATE OR REPLACE FUNCTION haromszog(n IN NUMBER) RETURN NUMBER AS
    s NUMBER := 0;
    k NUMBER := 1;
BEGIN
    WHILE s < n LOOP
        s := s + k;
        k := k + 1;
    END LOOP;
    
    IF s = n THEN
        RETURN 1;
    ELSE
        RETURN 0;
    END IF;
END;
/

SELECT haromszog(9) FROM dual;

--5.Feladat
-- SQL
CREATE OR REPLACE PROCEDURE helyezes(ev_param IN NUMBER, nem_parameter IN VARCHAR2) AS
  TYPE t_rec IS RECORD (
    nev teli_eredmenyek.nev%TYPE,
    sportag teli_eredmenyek.sportag%TYPE,
    versenyszam teli_eredmenyek.versenyszam%TYPE,
    helyezes teli_eredmenyek.helyezes%TYPE
  );
  v_rec t_rec;
  CURSOR c IS
    SELECT nev, sportag, versenyszam, helyezes
    FROM (
      SELECT nev, sportag, versenyszam, helyezes,
             ROW_NUMBER() OVER (PARTITION BY nev ORDER BY TO_NUMBER(helyezes)) AS rn
      FROM teli_eredmenyek
      WHERE olimpia = ev_param AND nem = nem_parameter AND REGEXP_LIKE(helyezes, '^[0-9]+$')
    ) 
    WHERE rn = 1;
BEGIN
  OPEN c;
  LOOP
    FETCH c INTO v_rec;
    EXIT WHEN c%NOTFOUND;
    DBMS_OUTPUT.PUT_LINE(v_rec.nev || ': ' || v_rec.versenyszam || ', helyezés: ' || v_rec.helyezes);
  END LOOP;
  CLOSE c;
END helyezes;
/

execute helyezes(2014, 'nő');

SELECT * FROM teli_eredmenyek
WHERE olimpia = 2014 AND nem = 'nő';

--6.Feladat
CREATE OR REPLACE FUNCTION csucsok RETURN NUMBER AS
  csucsok_szama NUMBER := 0;
  prev_magassag magassag.y%TYPE;
  curr_magassag magassag.y%TYPE;
  next_magassag magassag.y%TYPE;
BEGIN
  FOR i IN (SELECT y FROM magassag) LOOP
    next_magassag := i.y;
    IF curr_magassag IS NOT NULL AND prev_magassag IS NOT NULL THEN
      IF curr_magassag > prev_magassag AND curr_magassag > next_magassag THEN
        csucsok_szama := csucsok_szama + 1;
      END IF;
    END IF;
    prev_magassag := curr_magassag;
    curr_magassag := next_magassag;
  END LOOP;
  RETURN csucsok_szama;
END csucsok;
/

DECLARE
  csucsok_szama NUMBER;
BEGIN
  csucsok_szama := csucsok();
  DBMS_OUTPUT.PUT_LINE('A csúcsok száma: ' || csucsok_szama);
END;
/