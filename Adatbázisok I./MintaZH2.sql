SET SERVEROUTPUT ON;

--20 pontos rész - 1-3 feladatnál nem vagyok benne biztos, hogy elég a SELECT, elehet INSERT INTO-ra gondolt
--1.Feladat
SELECT dnev, fizetes
FROM dolgozo
WHERE jutalek IS NULL OR fizetes < (SELECT AVG(fizetes) FROM dolgozo);

--2.Feladat
SELECT dnev, COALESCE(jutalek, 0) + 3000 AS uj_jutalek
FROM dolgozo
WHERE dkod IN (SELECT DISTINCT fo.dkod
                    FROM dolgozo fo
                    JOIN dolgozo beo ON fo.dkod = beo.fonoke
                    GROUP BY fo.dkod
                    HAVING COUNT(*) >= 2);

--3.Feladat
SELECT nemfonok.dnev, nemfonok.fizetes + (fonok.fizetes * 0.1) AS uj_fizetes
FROM dolgozo nemfonok
JOIN dolgozo fonok ON nemfonok.fonoke = fonok.dkod;

--4.Feladat
DELETE FROM dolgozo -- SELECT dnev
WHERE oazon IN (SELECT OAZON FROM Osztaly WHERE TELEPHELY = 'DALLAS');

--5.Feladat
DELETE FROM osztaly -- SELECT *
WHERE OAZON IN (
    SELECT d1.OAZON
    FROM dolgozo d1
    JOIN dolgozo d2 ON d1.OAZON = d2.OAZON AND d1.DKOD <> d2.DKOD
    JOIN fiz_kategoria f ON d1.FIZETES BETWEEN f.ALSO AND f.FELSO AND f.KATEGORIA = 2
    JOIN fiz_kategoria f2 ON d2.FIZETES BETWEEN f2.ALSO AND f2.FELSO AND f2.KATEGORIA = 2
);

--6.Feladat
DELETE FROM dolgozo -- SELECT *
WHERE (OAZON, FIZETES) IN (
    SELECT d1.OAZON, MIN(d1.FIZETES)
    FROM dolgozo d1
    JOIN dolgozo d2 ON d1.OAZON = d2.OAZON
    GROUP BY d1.OAZON
);

--40 pontos rész
--1.Feladat
CREATE OR REPLACE FUNCTION ZH2(p_string1 IN VARCHAR2, p_string2 IN VARCHAR2) RETURN NUMBER IS
  v_count NUMBER := 0;
BEGIN
  IF LENGTH(p_string1) != LENGTH(p_string2) THEN
    RETURN -1;
  ELSE
    FOR i IN 1..LENGTH(p_string1) LOOP
      IF SUBSTR(p_string1, i, 1) = SUBSTR(p_string2, i, 1) THEN
        v_count := v_count + 1;
      END IF;
    END LOOP;
    RETURN v_count;
  END IF;
END ZH2;
/

DECLARE
  v_result NUMBER;
BEGIN
  v_result := ZH2('alma', 'alma');
  DBMS_OUTPUT.PUT_LINE('ZH2(''alma'', ''alma'') => ' || v_result);
  
  v_result := ZH2('alma', 'alal');
  DBMS_OUTPUT.PUT_LINE('ZH2(''alma'', ''alal'') => ' || v_result);
  
  v_result := ZH2('alma', 'körte');
  DBMS_OUTPUT.PUT_LINE('ZH2(''alma'', ''körte'') => ' || v_result);
END;
/

--2.Feladat
CREATE OR REPLACE PROCEDURE ZH4(p_osztaly_nev IN VARCHAR2) IS
  CURSOR c_dolgozok IS
    SELECT d.dnev, d.fizetes
    FROM dolgozo d
    JOIN osztaly o ON d.oazon = o.oazon
    WHERE SUBSTR(o.onev, -2) = p_osztaly_nev AND d.fizetes >= 4000;
BEGIN
  FOR r_dolgozo IN c_dolgozok LOOP
    DBMS_OUTPUT.PUT_LINE('Before: ' || r_dolgozo.dnev || ' ' || r_dolgozo.fizetes);
    UPDATE dolgozo
    SET fizetes = fizetes * 1.15
    WHERE dnev = r_dolgozo.dnev;
    DBMS_OUTPUT.PUT_LINE('After: ' || r_dolgozo.dnev || ' ' || (r_dolgozo.fizetes * 1.15));
  END LOOP;
  
  ROLLBACK;
END ZH4;
/

CALL ZH4('NG');

--3.Feladat
CREATE OR REPLACE FUNCTION ZH5_resz(p_string1 IN VARCHAR2, p_string2 IN VARCHAR2) RETURN NUMBER IS
  v_result VARCHAR2(32767);
BEGIN
  FOR i IN REVERSE 1..LENGTH(p_string2) LOOP
    v_result := v_result || SUBSTR(p_string2, i, 1);
  END LOOP;

  IF p_string1 = v_result THEN
    RETURN 1;
  ELSE
    RETURN 0;
  END IF;
END ZH5_resz;
/

CREATE OR REPLACE FUNCTION ZH5(p_username IN VARCHAR2, p_id IN NUMBER, p_password IN VARCHAR2) RETURN NUMBER IS
  CURSOR c_dolgozok IS
    SELECT d.dnev, f.dnev AS fonok_nev
    FROM dolgozo d
    JOIN dolgozo f ON d.fonoke = f.dkod
    WHERE d.dnev = p_username AND f.dkod = p_id;
  r_dolgozo c_dolgozok%ROWTYPE;
BEGIN
  OPEN c_dolgozok;
  FETCH c_dolgozok INTO r_dolgozo;
  IF c_dolgozok%FOUND THEN
    IF ZH5_resz(r_dolgozo.fonok_nev, p_password) = 1 THEN
      CLOSE c_dolgozok;
      RETURN 1;
    END IF;
  END IF;
  CLOSE c_dolgozok;
  RETURN 0;
END ZH5;
/

DECLARE
  v_result NUMBER;
BEGIN
  v_result := ZH5('BLAKE', 7839, 'GNIK');
  DBMS_OUTPUT.PUT_LINE('ZH5(''BLAKE'', 7839, ''GNIK'') => ' || v_result);
  
  v_result := ZH5('TURNER', 7698, 'EKALB');
  DBMS_OUTPUT.PUT_LINE('ZH5(''TURNER'', 7698, ''EKALB'') => ' || v_result);
  
  v_result := ZH5('BLAKE', 7839, 'KING');
  DBMS_OUTPUT.PUT_LINE('ZH5(''BLAKE'', 7839, ''KING'') => ' || v_result);
  
  v_result := ZH5('BLAKE', 7838, 'GNIK');
  DBMS_OUTPUT.PUT_LINE('ZH5(''BLAKE'', 7838, ''GNIK'') => ' || v_result);
  
  v_result := ZH5('BLAKA', 7839, 'GNIK');
  DBMS_OUTPUT.PUT_LINE('ZH5(''BLAKA'', 7839, ''GNIK'') => ' || v_result);
END;
/






