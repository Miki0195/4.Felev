SET SERVEROUTPUT ON;

--Kursorok.txt
--1.Feladat - Saját
CREATE OR REPLACE PROCEDURE print_odd_employees IS
  CURSOR c IS
    SELECT DNEV, FIZETES, ROW_NUMBER() OVER (ORDER BY DNEV) rn
    FROM vdani.dolgozo;
  v_rec c%ROWTYPE;
BEGIN
  OPEN c;
  LOOP
    FETCH c INTO v_rec;
    EXIT WHEN c%NOTFOUND;
    IF MOD(v_rec.rn, 2) = 1 THEN
      DBMS_OUTPUT.PUT_LINE('Name: ' || v_rec.DNEV || ', Salary: ' || v_rec.FIZETES);
    END IF;
  END LOOP;
  CLOSE c;
END print_odd_employees;
/

call print_odd_employees();

--1.Feladat - Imre
create or replace procedure paratlan_abcsorrend as
    cursor k is select * from vdani.dolgozo order by dnev;
    sor dolgozo%rowtype;
    i integer;
begin
    i := 0;
    for sor in k loop
        i := i + 1;
        if(i mod 2 = 1) then
            dbms_output.put_line(sor.dnev || ' ' || sor.fizetes);
        end if;
    end loop;
end;
/

call paratlan_abcsorrend();

--2.Feladat - Saját
CREATE OR REPLACE PROCEDURE increase_salary (p_category IN NUMBER) IS
  v_avg_salary NUMBER(10,2);
BEGIN
  -- Increase the salary of employees in the specified category
  UPDATE dolgozo d
  SET d.FIZETES = d.FIZETES + 1
  WHERE EXISTS (
    SELECT 1
    FROM fiz_kategoria fk
    WHERE fk.KATEGORIA = p_category
    AND d.FIZETES BETWEEN fk.ALSO AND fk.FELSO
  );

  -- Calculate the average of the new salaries
  SELECT AVG(FIZETES)
  INTO v_avg_salary
  FROM dolgozo d
  WHERE EXISTS (
    SELECT 1
    FROM fiz_kategoria fk
    WHERE fk.KATEGORIA = p_category
    AND d.FIZETES BETWEEN fk.ALSO AND fk.FELSO
  );

  -- Round the average to two decimal places
  v_avg_salary := ROUND(v_avg_salary, 2);

  -- Display the average
  DBMS_OUTPUT.PUT_LINE('The average of the new salaries is: ' || v_avg_salary);
  
  rollback;
END increase_salary;
/

call increase_salary(4);

--2.Feladat - Imre
create or replace procedure Feladat2(fiz_kat integer) as
    cursor k is select * from dolgozo d join fiz_kategoria f on d.fizetes >= f.also and d.fizetes <= f.felso where f.kategoria = fiz_kat for update;
    kat integer;
    fiz_avg real;
begin

    select round(avg(fizetes),2) into fiz_avg from dolgozo d join fiz_kategoria f on d.fizetes >= f.also and d.fizetes <= f.felso where f.kategoria = fiz_kat;
    
    dbms_output.put_line(fiz_avg);
    
    for sor in k loop
        update dolgozo set fizetes = fizetes + 1 where sor.dkod = dkod;
    end loop;
    
    select round(avg(fizetes),2) into fiz_avg from dolgozo d join fiz_kategoria f on d.fizetes >= f.also and d.fizetes <= f.felso where f.kategoria = fiz_kat;
    
    dbms_output.put_line(fiz_avg);
    
    rollback;
end;
/


call Feladat2(4);

--Kollekciók.txt
--1.Feladat - Saját
CREATE OR REPLACE PROCEDURE print_last_odd_employee IS
  TYPE t_emp_rec IS RECORD (
    DNEV dolgozo.DNEV%TYPE,
    FIZETES dolgozo.FIZETES%TYPE
  );
  TYPE t_emp_tab IS TABLE OF t_emp_rec;
  v_emps t_emp_tab;
  TYPE t_emp IS TABLE OF dolgozo.DNEV%TYPE INDEX BY PLS_INTEGER;
  v_names t_emp;
  v_salaries t_emp;
  v_last_index PLS_INTEGER;
BEGIN
  SELECT DNEV, FIZETES BULK COLLECT INTO v_emps FROM dolgozo ORDER BY DNEV;

  FOR i IN 1..v_emps.COUNT LOOP
    IF MOD(i, 2) = 1 THEN
      v_names(i) := v_emps(i).DNEV;
      v_salaries(i) := v_emps(i).FIZETES;
      v_last_index := i;
    END IF;
  END LOOP;
  
  DBMS_OUTPUT.PUT_LINE('Last odd employee: Name: ' || v_names(v_last_index) || ', Salary: ' || v_salaries(v_last_index));
END print_last_odd_employee;
/

call print_last_odd_employee();

--2.Feladat - Saját
CREATE OR REPLACE PROCEDURE print_higher_salary_employees IS
  TYPE t_emp_rec IS RECORD (
    DNEV dolgozo.DNEV%TYPE,
    FIZETES dolgozo.FIZETES%TYPE
  );
  TYPE t_emp_tab IS TABLE OF t_emp_rec;
  v_emps t_emp_tab;
BEGIN
  SELECT DNEV, FIZETES BULK COLLECT INTO v_emps FROM dolgozo ORDER BY DNEV;

  FOR i IN 2..v_emps.COUNT LOOP
    IF v_emps(i).FIZETES > v_emps(i-1).FIZETES THEN
      DBMS_OUTPUT.PUT_LINE('Employee: Name: ' || v_emps(i).DNEV || ', Salary: ' || v_emps(i).FIZETES);
    END IF;
  END LOOP;
  
END print_higher_salary_employees;
/

call print_higher_salary_employees();

--2.Feladat - Imre
create or replace procedure Feladat3 as
    TYPE arrayType IS TABLE OF dolgozo%rowtype;
    arr arrayType;
begin
    select * bulk collect into arr
    from dolgozo
    order by dnev;
    
    for i in 2..arr.COUNT loop
        if arr(i - 1).fizetes < arr(i).fizetes then
            dbms_output.put_line(arr(i).dnev || ' ' || arr(i).fizetes);
        end if;
    end loop;
end;
/

call Feladat3();