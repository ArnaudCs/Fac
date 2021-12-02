--Q1. X (Y) donne les droits de lecture de sa table ABONNE à l'utilisate
GRANT SELECT ON ABONNE TO e000000000;
--Q2. X (Y) donne les droits de modification de sa table LIVRE
GRANT SELECT ON LIVRE TO e0000000;
GRANT UPDATE ON LIVRE TO e0000000;
--Q3. Vérifier que les privilèges ont été bien accordés en inte
--droit que j'ai donné
SELECT GRANTOR, TABLE NAME FROM USER TAB PRIVS made;
--droit que j'ai recue
SELECT GRANTOR, TABLE NAME FROM USER TAB PRIVS recd;
--tous les drolts
SELECT GRANTOR, TABLE NAME FROM USER TAB PRIVS;

--SELECT table name FROM cat WHERE no(table name LIKE 'SYS%' o
--SELECT *FROM USER TAB PRIVS;

--Q4. Testez vos nouveaux droits (Les objets que vous interrogez
--n'ètes pas propriétaire sont désignés par leur
--numpropriétaire.nomobjet:lenuméropropriétairecorrespond
--disponible sur la page web de Sapiens : E2021.

SELECT* FROM E20190003378.ABONNE;

--Q5. Créez une vue sur une table sur laquelle X (Y) vous a don
CREATE VIEW abonne Gatien AS SELECT FROM e20190002767.abonne;


--2.2 Ordre REVOKE :
--Q1. Enlever les privilèges précédemment accordés
REVOKE SELECT ON ABONNE FROM e000000000;
--Q2. Vérifier   que   les   privilèges   ont   bien   été   supprimés,   notamment   en
--interrogeant les tables sur lesquelles vous n’avez plus de droits et en
--consultant   également   les   tables   du   méta-schéma   comme   USER   TAB
--PRIVS.
SELECT quelquechose from UneTable;
--Q3. Que constatez vous par rapport à la vue précédemment créée ?
-- On ne peut plus la créer. La vue est considérée comme inexistante. 

--3 Verrous :
--Q1. Faites des sélections sur les mêmes lignes des mêmes tables avec deux noms utilisateurs différents.
SELECT nom, prenom FROM e20000000.ABONNE;


