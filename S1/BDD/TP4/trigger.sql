CREATE OR REPLACE TRIGGER test_exemplaire
BEFORE DELETE ON EXEMPLAIRE
FOR EACH ROW
DECLARE
    state VARCHAR(15);

BEGIN
	IF(:OLD.ETAT = 'BON') THEN
	raise_application_error(-20000, 'Suppression impossible, l état du livre est BON');
	END IF;
END;
/
