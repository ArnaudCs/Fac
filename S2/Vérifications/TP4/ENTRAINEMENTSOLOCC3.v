Require Import ListSet.

Fixpoint mult (n m : nat) {struct n} : nat :=
 match n with 
  | 0 => 0
  | S p => (plus (mult p m) m)
 end.

Lemma ex1 : forall n : nat, (mult 2 n) = (plus n n).
Proof.
 intros.
 elim n.
 simpl.
 reflexivity.
 intros.
 simpl.
 reflexivity.
Qed.

Lemma ex1rap : forall n : nat, (mult 2 n) = (plus n n).
Proof.
 intro.
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

 








