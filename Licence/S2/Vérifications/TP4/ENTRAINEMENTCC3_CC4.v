Require Import ListSet.
Require Import FunInd.

Fixpoint mult (n m : nat) {struct n} : nat :=
 match n with 
  | 0 => 0
  | S p => (plus (mult p m) m)
 end.

Inductive is_fact : nat -> nat -> Prop :=
 | is_fact_0 : is_fact 0 1
 | is_fact_S : forall n f : nat, is_fact n f -> is_fact (S n) (f * (S n)).
 
Fixpoint fact (n : nat) : nat :=
 match n with 
  | 0 => 1
  | (S p) => (fact p) * n
 end.

Lemma ex1 : forall n : nat, (mult 2 n) = (plus n n).
Proof.
 intros.
 simpl.
 reflexivity.
Qed.

Lemma com : forall n m : nat, (plus n m) = (plus m n).
Proof.
 intros.
 elim m.
 elim n.
 reflexivity.
 intros.
 simpl.
 rewrite H.
 simpl.
 reflexivity.
 intros.
 simpl.
 rewrite <- H.
 elim n.
 simpl.
 reflexivity.
 intros.
 simpl.
 rewrite H0.
 reflexivity.
Qed.

Lemma ex2 : forall n : nat, (mult n 2) = (plus n n).
Proof.
 intros.
 elim n.
 simpl.
 reflexivity.
 intros.
 simpl.
 rewrite H.
 simpl.
 rewrite com.
 rewrite (com n0 (S n0)).
 simpl.
 reflexivity.
Qed.

(*Exercice 2*)

Open Scope list.
Parameters A : Type.

Fixpoint rev (l : list A) {struct l} : list A :=
 match l with
  | nil => nil
  | a :: l1 => (app (rev l1) (a :: nil))
 end.

Lemma ex3 : forall l : list A, forall e : A, (rev (app l (e :: nil))) = (e :: (rev l)).
Proof.
 intros.
 elim l.
 simpl.
 reflexivity.
 intros.
 simpl.
 rewrite H.
 reflexivity.
Qed.

Lemma ex4 : forall l : list A, (rev (rev l)) = l.
Proof.
 intros.
 elim l.
 simpl.
 reflexivity.
 intros.
 simpl.
 rewrite ex3.
 rewrite H.
 reflexivity.
Qed.

(*Exercice 3*)
Parameters S : Set.
 
Inductive FProp : Set :=
 | Symb : S -> FProp
 | Not : FProp -> FProp
 | And : FProp -> FProp -> FProp
 | Or : FProp -> FProp -> FProp
 | Impl : FProp -> FProp -> FProp
 | Equ : FProp -> FProp -> FProp.
 
Fixpoint sub (f : FProp) {struct f} : list FProp := (*renvoi sous formule d'une formule f*)
 match f with 
  | (Symb s) => (Symb s) :: nil
  | (Not f1) => (Not f1) :: (sub f1)
  | (Or f1 f2) => (Or f1 f2) :: (app (sub f1) (sub f2))
  | (And f1 f2) => (And f1 f2) :: (app (sub f1) (sub f2))
  | (Impl f1 f2) => (Impl f1 f2) :: (app (sub f1) (sub f2))
  | (Equ f1 f2) => (Equ f1 f2) :: (app (sub f1) (sub f2))
 end.

Fixpoint nbc (f : FProp) {struct f} : nat :=
  match f with 
  | (Symb s) => 0
  | (Not f1) => 1 + (nbc f1)
  | (Or f1 f2) => 1 + (nbc f1) + (nbc f2)
  | (And f1 f2) => 1 + (nbc f1) + (nbc f2)
  | (Impl f1 f2) => 1 + (nbc f1) + (nbc f2)
  | (Equ f1 f2) => 1 + (nbc f1) + (nbc f2)
  end.

(*Partie 4*)

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
 elim H.
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




















