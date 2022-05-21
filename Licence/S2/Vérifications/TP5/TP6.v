Require Import FunInd.
Require Extraction.

(*Exercice 3*)

Inductive is_fact : nat -> nat -> Prop :=
 | is_fact_0 : is_fact 0 1
 | is_fact_S : forall n f : nat, is_fact n f -> is_fact (S n) (f * (S n)).

Lemma fact : forall n : nat, {v : nat | is_fact n v}.
Proof.
 intros.
 elim n.
 exists (S 0).
 apply is_fact_0.
 intros.
 inversion H.
 exists (mult x (S n0)).
 apply is_fact_S.
 assumption.
Defined. 

Recursive Extraction fact.

(*Exercice 4*)
Lemma eq_nat : forall n m : nat, {n = m} + {n <> m}.
Proof.
 (double induction n m).
 decide equality.
 intros.
 decide equality.
 intros.
 decide equality.
 intros.
 decide equality.
Defined.

Recursive Extraction eq_nat.

(*Exercice 5*)
Require Export List.
Open Scope list_scope.
Import ListNotations.

Inductive is_rev : list nat -> list nat -> Prop :=
| is_rev_nil : is_rev nil nil
| is_rev_cons : forall (n : nat) (l1 l2 : list nat), 
is_rev l1 l2 -> is_rev (n :: l1) (l2 ++ [n]).

Lemma rev : forall l : list nat, {l' : list nat | is_rev l l'}.
Proof.
 intros.
 elim l.
 exists (nil).
 apply is_rev_nil.
 intros.
 inversion H.








 

 
 
 
 