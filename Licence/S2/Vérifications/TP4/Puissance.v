Require Import FunInd.

Inductive is_power : nat -> nat -> nat -> Prop :=
 | is_power_0 : forall n : nat, (is_power n 0 1)
 | is_power_S : forall n f t : nat, (is_power n f t) -> (is_power n (S f) (t * n)).
 
Fixpoint power (n f : nat) : nat :=
 match f with 
  | 0 => 1
  | (S p) => (power n p) * n
 end.

(*Ecrire le schéma d'induction fonctionnelle associé à cette fonction*)
Functional Scheme power_ind := Induction for power Sort Prop.

Lemma powercorrec : forall n f t : nat, (power n f) = t -> (is_power n f t).
Proof.
 intro.
 intro.
 functional induction (power n f) using power_ind.
 intros.
 rewrite <- H.
 apply is_power_0.
 intros.
 rewrite <- H.
 apply is_power_S.
 apply IHn0.
 reflexivity.
Qed.

