DECLARE 
	numero NUMERIC(6,0);	
	NB NUMERIC(3,0);
	numero2 ABONNE.NUM_AB%TYPE;
	no_emp EXCEPTION;

BEGIN
numero := &numero_abo;
	SELECT num_ab into numero2 FROM ABONNE where num_ab = numero;
	SELECT COUNT(*) INTO NB FROM EMPRUNT WHERE num_ab = numero;
	
	IF NB = 0 THEN
	RAISE no_emp;
	ELSE
	INSERT INTO AB_NB VALUES(numero, NB);
	END IF;

	EXCEPTION
	WHEN no_data_found THEN INSERT INTO AB_NB VALUES (numero, -1);
	WHEN no_emp THEN INSERT INTO AB_NB VALUES (numero, NULL);

END;
/
