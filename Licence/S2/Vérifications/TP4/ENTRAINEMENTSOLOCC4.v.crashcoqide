Require Import FunInd.

Inductive is_fact : nat -> nat -> Prop :=
 | is_fact_0 : is_fact 0 1
 | is_fact_S : forall n f : nat, is_fact n f -> is_fact (S n) (f * (S n)).
 
Fixpoint fact (n : nat) : nat :=
 match n with 
  | 0 => 1
  | (S p) => (fact p) * n
 end.

(*Ecrire le schéma d'induction fonctionnelle associé à cette fonction*)
Functional Scheme fact_ind := Induction for fact Sort Prop.

(*Démontrer la correction en utilisant le schéma d'induction structurelle*)
Lemma fact_correction : forall n f : nat, (fact n) = f -> is_fact n f.
Proof.
 induction n.
 intros.
 rewrite <- H.
 simpl.
 apply is_fact_0.
 intros.
 rewrite <- H.
 simpl.
 apply is_fact_S.
 apply IHn.
 reflexivity.
Qed.

(*Démontrer la correction en utilisant le schéma d'induction fonctionelle*)
Lemma fact_correction_fun : forall n f : nat, (fact n) = f -> is_fact n f.
Proof.
 intro.
 functional induction (fact n) using fact_ind.
 intros.
 rewrite <- H.
 apply is_fact_0.
 intros.
 rewrite <- H.
 apply is_fact_S.
 apply IHn0.
 reflexivity.
Qed.

(*Démontrer la complétude en utilisant le schéma d'induction sur la relation*)
Lemma fact_completude : forall n f : nat, is_fact n f -> (fact n) = f.
Proof.
 intros.
 elim H.
 simpl.
 reflexivity.
 intros.
 simpl.
 rewrite H1.
 reflexivity.
Qed.

(*Exercice 2*)

Inductive is_even : nat -> Prop :=
 | is_even_0 : is_even 0
 | is_even_S : forall n : nat, is_even n -> is_even (S (S n)).
 
Fixpoint even (n : nat) : Prop :=
 match n with 
  | 0 => True
  | 1 => False
  | S (S p) => even p
 end.

Functional Scheme even_ind := Induction for even Sort Prop.

Lemma x1 : forall n : nat, (even n) = True -> is_even n.
Proof.
 intros.
 functional induction (even n) using even_ind. 
 intros.
 apply is_even_0.
 intros.
 elimtype False.
 rewrite H.
 reflexivity.
 apply is_even_S.
 apply IHP.
 rewrite H.
 reflexivity.
Qed.

(*Lemma x2 : forall n : nat, (even n) = False -> ~(is_even n).
Proof.
 intros.
 functional induction (even n) using even_ind.
 elimtype False.
 rewrite <- H.
 reflexivity.

Lemma x3 : forall n : nat, (is_even n) -> (even n) = True.
Proof.
 intros. 
 functional induction (even n) using even_ind.
 reflexivity.
 inversion H.
 rewrite IHP.
 reflexivity.
 apply x1.*)
 
Lemma x4 : forall n : nat, ~(is_even n) -> (even n) = False.














