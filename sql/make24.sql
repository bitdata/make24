WITH RECURSIVE numbers(n) AS(
    SELECT 10 FROM dual
    UNION ALL
    SELECT n-1 FROM numbers WHERE n>1
),
operators(op) AS(
    SELECT '+' FROM dual
    UNION ALL
    SELECT '-' FROM dual
    UNION ALL
    SELECT '*' FROM dual
    UNION ALL
    SELECT '/' FROM dual
),
combinations(v1,v2,v3,v4,op1,op2,op3) AS(
	SELECT N1.n,N2.n,N3.n,N4.n,O1.op,O2.op,O3.op
    FROM numbers AS N1,numbers AS N2,numbers AS N3,numbers AS N4,operators O1,operators AS O2,operators AS O3
),
first_calculations(v1,v2,v3,v4,op1,op2,op3,r1,e1) AS(
	SELECT v1,v2,v3,v4,op1,op2,op3,CASE op1 WHEN '+' THEN v1+v2 WHEN '-' THEN v1-v2 WHEN '*' THEN v1*v2 ELSE v1 DIV v2 END,CONCAT(v1,op1,v2)
    FROM combinations
    WHERE NOT ( (op1 IN ('+','*') AND v1>v2) OR (op1='-' AND v1<=v2) OR (op1='/' AND (v1<v2 OR v2=1 OR v1 MOD v2>0)) )    
),
serial_second_calculations(v1,v2,v3,v4,op1,op2,op3,r2,e2) AS(
	SELECT v1,v2,v3,v4,op1,op2,op3,
		CASE op2 WHEN '+' THEN r1+v3 WHEN '-' THEN CASE swap WHEN 1 THEN v3-r1 ELSE r1-v3 END WHEN '*' THEN r1*v3 ELSE CASE swap WHEN 1 THEN v3 DIV r1 ELSE r1 DIV v3 END END,
        CASE swap WHEN 1 THEN CONCAT(v3,op2,e1) ELSE CONCAT(e1,op2,v3) END
	FROM (
		SELECT v1,v2,v3,v4,op1,op2,op3,r1,CASE WHEN op2 IN ('*','/') AND op1 IN ('+','-') THEN CONCAT('(',e1,')') ELSE e1 END AS e1,
			CASE WHEN op2 IN ('+','*') AND r1>v3 THEN 1 WHEN op2 IN ('-','/') AND r1<v3 THEN 1 ELSE 0 END AS swap
		FROM first_calculations
		WHERE NOT ( 
				(op2='+' AND op1='-') OR (op2='+'  AND op1='+' AND v3>v1) OR (op2='*' AND op1='/') OR (op2='*' AND op1='*' AND v3>v1)
				OR (op2='-' AND v3=r1) OR (op2='-' AND op1 IN ('+','-') AND v3>r1) 
				OR (op2='/' AND (v3=1 OR r1=1 OR (v3>r1 AND v3 MOD r1>0) OR (v3<r1 AND r1 MOD v3>0))) OR (op2='/' AND op1 IN ('*','/') AND v3>r1)
			)
	)  AS V
),
serial_third_calculations(v1,v2,v3,v4,r3,e3) AS(
	SELECT v1,v2,v3,v4,
		CASE op3 WHEN '+' THEN r2+v4 WHEN '-' THEN CASE swap WHEN 1 THEN v4-r2 ELSE r2-v4 END WHEN '*' THEN r2*v4 ELSE CASE swap WHEN 1 THEN v4 DIV r2 ELSE r2 DIV v4 END END,
        CASE swap WHEN 1 THEN CONCAT(v4,op3,e2) ELSE CONCAT(e2,op3,v4) END
	FROM (
		SELECT v1,v2,v3,v4,op1,op2,op3,r2,CASE WHEN op3 IN ('*','/') AND op2 IN ('+','-') THEN CONCAT('(',e2,')') ELSE e2 END AS e2,
			CASE WHEN op3 IN ('+','*') AND r2>v4 THEN 1 WHEN op3 IN ('-','/') AND r2<v4 THEN 1 ELSE 0 END AS swap
		FROM serial_second_calculations
		WHERE NOT ( 
				(op3='+' AND op2='-') OR (op3='+'  AND op2='+' AND v4>v3) OR (op3='*' AND op2='/') OR (op3='*' AND op2='*' AND v4>v3)
				OR (op3='-' AND op2 IN ('+','-') AND v4>r2) 
				OR (op3='/' AND (v4=1 OR r2=1 OR (v4>r2 AND v4 MOD r2>0) OR (v4<r2 AND r2 MOD v4>0))) OR (op3='/' AND op2 IN ('*','/') AND v4>r2)
			)
	) AS V
),
parallel_second_calculations(v1,v2,v3,v4,op3,r1,e1,r2,e2) AS(
	SELECT v1,v2,v3,v4,op3,r1,CASE WHEN op3 IN ('*','/') THEN CONCAT('(',e1,')') ELSE e1 END,
		CASE op2 WHEN '+' THEN v3+v4 WHEN '-' THEN v3-v4 WHEN '*' THEN v3*v4 ELSE v3 DIV v4 END,CASE WHEN op3 IN ('*','/') THEN CONCAT('(',v3,op2,v4,')') ELSE CONCAT(v3,op2,v4) END
    FROM first_calculations
    WHERE NOT ( 
			(op3 IN ('+','-') AND op1 IN ('+','-')) OR (op3 IN ('+','-') AND op2 IN ('+','-')) 
			OR (op3 IN ('*','/') AND op1 IN ('*','/')) OR (op3 IN ('*','/') AND op2 IN ('*','/')) 
			OR (op2 IN ('+','*') AND v3>v4) OR (op2='-' AND v3<=v4) OR (op2='/' AND (v3<v4 OR v4=1 OR v3 MOD v4>0))
        )
),
parallel_third_calculations(v1,v2,v3,v4,r3,e3) AS(
	SELECT v1,v2,v3,v4,CASE op3 WHEN '+' THEN r1+r2 WHEN '-' THEN r1-r2 WHEN '*' THEN r1*r2 ELSE r1 DIV r2 END,CONCAT(e1,op3,e2)
	FROM parallel_second_calculations
	WHERE NOT ( (op3 IN('+','*') AND r1>r2) OR (op3 IN ('-','/') AND r1<=r2) OR (op3='/' AND (r2=1 OR r1 MOD r2>0)) )
),
raw_results(v1,v2,v3,v4,e) AS(
	SELECT v1,v2,v3,v4,e3
	FROM serial_third_calculations
	WHERE r3=24
	UNION ALL
	SELECT v1,v2,v3,v4,e3
	FROM parallel_third_calculations
	WHERE r3=24
),
results(v1,v2,v3,v4,e) AS(
	SELECT DISTINCT v1,CASE WHEN v2>v3 THEN v3 ELSE v2 END AS v2,CASE WHEN v2>v3 THEN v2 ELSE v3 END AS v3,v4,e
	FROM (
		SELECT CASE WHEN v1>v3 THEN v3 ELSE v1 END AS v1,CASE WHEN v2>v4 THEN v4 ELSE v2 END AS v2,CASE WHEN v1>v3 THEN v1 ELSE v3 END AS v3,CASE WHEN v2>v4 THEN v2 ELSE v4 END AS v4,e
		FROM (
			SELECT CASE WHEN v1>v2 THEN v2 ELSE v1 END AS v1,CASE WHEN v1>v2 THEN v1 ELSE v2 END AS v2,CASE WHEN v3>v4 THEN v4 ELSE v3 END AS v3,CASE WHEN v3>v4 THEN v3 ELSE v4 END AS v4,e
			FROM raw_results
		) AS A
	) AS B
),
fractional_calculations(v1,v2,v3,v4,r,e) AS(
	SELECT v1,v2,v3,v4,
		CASE op3 WHEN '*' THEN v3*v4+ CASE op2 WHEN '+' THEN 1 ELSE -1 END *(v1*v4 DIV v2) ELSE v4*v2 DIV CASE WHEN op2='+' THEN v1+v2*v3 WHEN v1>v2*v3 THEN v1-v2*v3 ELSE v2*v3-v1 END END,
		CONCAT(v4,op3,'(',CASE WHEN op2='-' AND v1>v2*v3 THEN CONCAT(v1,'/',v2,op2,v3) ELSE CONCAT(v3,op2,v1,'/',v2) END,')')
    FROM combinations
    WHERE op1='/' AND op2 IN ('+','-')  AND (v1 MOD v2)<>0 AND (
			(op3='*' AND (v1*v4 MOD v2)=0) OR (op3='/' AND op2='+' AND (v4*v2 MOD (v1+v2*v3))=0)
            OR (op3='/' AND op2='-' AND v1<>v2*v3 AND (v4*v2 MOD(v1-v2*v3))=0)
		)
),
fractional_results(v1,v2,v3,v4,e) AS(
	SELECT DISTINCT v1,CASE WHEN v2>v3 THEN v3 ELSE v2 END AS v2,CASE WHEN v2>v3 THEN v2 ELSE v3 END AS v3,v4,e
	FROM (
		SELECT CASE WHEN v1>v3 THEN v3 ELSE v1 END AS v1,CASE WHEN v2>v4 THEN v4 ELSE v2 END AS v2,CASE WHEN v1>v3 THEN v1 ELSE v3 END AS v3,CASE WHEN v2>v4 THEN v2 ELSE v4 END AS v4,e
		FROM (
			SELECT CASE WHEN v1>v2 THEN v2 ELSE v1 END AS v1,CASE WHEN v1>v2 THEN v1 ELSE v2 END AS v2,CASE WHEN v3>v4 THEN v4 ELSE v3 END AS v3,CASE WHEN v3>v4 THEN v3 ELSE v4 END AS v4,e
			FROM fractional_calculations
            WHERE r=24
		) AS A
	) AS B
)
SELECT v1,v2,v3,v4,e
FROM results
UNION ALL
SELECT v1,v2,v3,v4,e
FROM fractional_results F
WHERE NOT EXISTS (SELECT * FROM results WHERE v1=F.v1 AND v2=F.v2 AND v3=F.v3 AND v4=F.v4)
ORDER BY v1,v2,v3,v4,e