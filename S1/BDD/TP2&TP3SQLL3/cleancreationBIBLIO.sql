/*
Suppression des relations 
*/
/*
*************************************************
ATTENTION NE PAS TOUCHER AUX LIGNES SUIVANTES
ELLES PERMETTENT DE SUPPRIMER PROPREMENT LES RELATIONS
*************************************************
*/
prompt "Suppression des relations"

BEGIN
EXECUTE IMMEDIATE 'DROP TABLE ECRIT';
EXCEPTION
 WHEN OTHERS THEN
	IF SQLCODE != -942 THEN
	RAISE;
	END IF;
END;
/
BEGIN
EXECUTE IMMEDIATE 'DROP TABLE EMPRUNT';
EXCEPTION
 WHEN OTHERS THEN
	IF SQLCODE != -942 THEN
	RAISE;
	END IF;
END;
/

BEGIN
EXECUTE IMMEDIATE 'DROP TABLE CARACTERISE';
EXCEPTION
 WHEN OTHERS THEN
	IF SQLCODE != -942 THEN
	RAISE;
	END IF;
END;
/

BEGIN
EXECUTE IMMEDIATE 'DROP TABLE EXEMPLAIRE';
EXCEPTION
 WHEN OTHERS THEN
	IF SQLCODE != -942 THEN
	RAISE;
	END IF;
END;
/

BEGIN
EXECUTE IMMEDIATE 'DROP TABLE AUTEUR';
EXCEPTION
 WHEN OTHERS THEN
	IF SQLCODE != -942 THEN
	RAISE;
	END IF;
END;
/
BEGIN
EXECUTE IMMEDIATE 'DROP TABLE ABONNE';
EXCEPTION
 WHEN OTHERS THEN
	IF SQLCODE != -942 THEN
	RAISE;
	END IF;
END;
/

BEGIN
EXECUTE IMMEDIATE 'DROP TABLE MOT_CLEF';
EXCEPTION
 WHEN OTHERS THEN
	IF SQLCODE != -942 THEN
	RAISE;
	END IF;
END;
/

BEGIN
EXECUTE IMMEDIATE 'DROP TABLE LIVRE';
EXCEPTION
 WHEN OTHERS THEN
	IF SQLCODE != -942 THEN
	RAISE;
	END IF;
END;
/

/*
*************************************************
*/

/*
Création des relations 
*/

prompt "Création des relations"


CREATE TABLE LIVRE (
	ISBN VARCHAR(15),
	TITRE VARCHAR(50) CONSTRAINT LITITRE NOT NULL, 
	SIECLE NUMERIC(2,0) CHECK (SIECLE BETWEEN 0 and 21),
	CATEGORIE VARCHAR(16),
	CONSTRAINT PK_LIVRE PRIMARY KEY (ISBN)
);

CREATE TABLE ABONNE (
	NUM_AB  NUMERIC(6,0),  
	NOM VARCHAR(15)  CONSTRAINT ABNOM NOT NULL, 
	PRENOM VARCHAR(20), 
	VILLE VARCHAR(13), 
	AGE NUMERIC(3,0),
 	TARIF NUMERIC(3,0) ,
 	REDUC NUMERIC(3,0) ,
	CONSTRAINT PK_ABONNE PRIMARY KEY (NUM_AB), -- permet de définir la clé primaire comme primary key
	CONSTRAINT DOM_AGE CHECK (AGE BETWEEN 0 AND 120) 
);

CREATE TABLE EXEMPLAIRE (
	NUMERO NUMERIC(4,0), 
	DATE_ACHAT DATE, 
	PRIX NUMERIC(5,2), 
	CODE_PRET VARCHAR(20) ,
	ETAT VARCHAR(15) CHECK (ETAT IN ('BON','ABIME','EN_REPARATION')), 
	ISBN  VARCHAR(15), 
	CONSTRAINT KP_EXEMPLAIRE PRIMARY KEY (NUMERO),
	CONSTRAINT FK_EXEMPLAIRE_LIVRE FOREIGN KEY (ISBN) REFERENCES LIVRE(ISBN) ON DELETE CASCADE,
	CONSTRAINT DOM_CODE_PREF CHECK (CODE_PRET IN ('EXCLU','EMPRUNTABLE', 'CONSULTABLE')) 
);

CREATE TABLE MOT_CLEF (
	MOT VARCHAR(16) CONSTRAINT PK_MOT_CLEF PRIMARY KEY,
	MOT_PARENT VARCHAR(16),
	FOREIGN KEY (MOT_PARENT) REFERENCES MOT_CLEF(MOT) ON DELETE CASCADE
);

CREATE TABLE EMPRUNT (
	NUM_AB  NUMBER(6,0),
	NUM_EX NUMBER (4,0),
	D_EMPRUNT DATE, 
	D_RETOUR DATE, 
	D_RET_REEL DATE, 
	NB_RELANCE NUMBER(1,0) CHECK (NB_RELANCE IN (1,2,3)),
	CONSTRAINT PK_EMPRUNT PRIMARY KEY (NUM_AB,NUM_EX,D_EMPRUNT),
	CONSTRAINT FK_EMPRUNT_ABONNE FOREIGN KEY (NUM_AB) REFERENCES ABONNE(NUM_AB) ON DELETE CASCADE, 
	CONSTRAINT FK_EMPRUNT_EX FOREIGN KEY (NUM_EX)  REFERENCES EXEMPLAIRE(NUMERO) ON DELETE CASCADE
);

CREATE TABLE CARACTERISE (
	ISBN VARCHAR(20), 
	MOT VARCHAR(20),
	CONSTRAINT PK_CARACTERISE PRIMARY KEY (ISBN,MOT),
	CONSTRAINT FK_CARACTERISE_LIVRE FOREIGN KEY (ISBN) REFERENCES LIVRE(ISBN) ON DELETE CASCADE,
	CONSTRAINT FK_CARACTERISE_MOT_CLEF FOREIGN KEY (MOT) REFERENCES MOT_CLEF(MOT) ON DELETE CASCADE
);
