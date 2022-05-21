CREATE OR REPLACE FUNCTION nb_pret (num2 ABONNE.NUM_AB%TYPE) 
	
RETURN INTEGER IS
	nb INTEGER;
BEGIN 
	SELECT COUNT(*) INTO nb 
	FROM EMPRUNT
	WHERE num_ab = num2; 
	RETURN (nb);
END;
/

select num_ab, nb_pret(num_ab) from ABONNE;
