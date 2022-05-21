Require Import FunInd.

Inductive is_sum : nat -> nat -> Prop :=
 | is_sum_0 : is_sum 0 0
 | is_sum_S : forall n s : nat, is_sum n s -> is_sum (S n) (s + (S n)).

Fixpoint sum (n : nat) : nat :=
 match n with 
  | 0 => 0
  | (S p) => (sum p) + (S p)
 end.


(*Génération du schéma d'induction fonctionnelle*)
Functional Scheme sum_ind := Induction for sum Sort Prop. 

Lemma sum_correction : forall (n s : nat), (sum n) = s -> is_sum n s.
Proof.
 induction n.
 intros.
 rewrite <- H.
 simpl.
 apply is_sum_0.
 intros.
 rewrite <- H.
 simpl.
 apply is_sum_S.
 apply IHn.
 reflexivity.
Qed.
 
Lemma sum_completude : forall (n s : nat), is_sum n s -> (sum n) = s.
Proof. 
 intros.
 elim H.
 intros.
 simpl.
 reflexivity. 
 intros.
 simpl.
 rewrite H1.
 reflexivity.
Qed.

Lemma sum_completude2 : forall (n s : nat), is_sum n s -> (sum n) = s.
Proof. 
 induction n.
 intros.
 simpl.
 inversion H.
 reflexivity.
 intros. 
 simpl.
 inversion H.
 rewrite (IHn s0 H1).
 reflexivity.
Qed.






















