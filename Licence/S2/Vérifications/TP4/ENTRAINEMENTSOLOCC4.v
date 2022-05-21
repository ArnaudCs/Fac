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
 elim H.  (* induction H *)
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
 inversion H.*)
 

Lemma x3 : forall n : nat, (is_even n) -> (even n) = True.
Proof.
 intros.
 induction H.
 auto.
 simpl.
 apply IHis_even.
Qed.

(*induction n = elim H.*)



Parameter E : Set .
Parameters Q P :E-> Prop. 

Lemma x1 : (forall x : E, P(x) -> Q(x)) -> (exists x : E, P(x)) -> (exists x : E, Q(x)).
Proof. 
 intros.
 elim H0.
 intros.
 exists x.
 apply H.
 assumption.
Qed.

Lemma x2 : (exists x : E, P(x) /\ Q(x)) -> (exists x : E, P(x)) /\ (exists x : E, Q(x)).
Proof.
 intros.
 split.
 elim H.
 intro.
 intros.
 exists x.
 elim H0.
 intros.
 assumption.
 elim H.
 intro.
 intros.
 exists x.
 elim H0.
 intros.
 assumption.
Qed.

Parameter R : Set .
Parameters zero one : R.
Parameter opp : R -> R.
Parameters plus mult : R -> R -> R.
Section Commutative_ring .
Variables a b c : R.
Axiom ass_plus : plus (plus a b) c = plus a (plus b c).
Axiom com_plus : plus a b = plus b a .
Axiom com_mult : mult a b = mult b a .
Axiom ass_mult : mult ( mult a b ) c = mult a ( mult b c ) .
Axiom dis_left : mult a ( plus b c ) = plus ( mult a b ) ( mult a c ) .
Axiom dis_right : mult ( plus b c) a = plus (mult b a ) (mult c a ) .
Axiom neu_plus_r : plus a zero = a .
Axiom neu_plus_l : plus zero a = a .
Axiom neu_mult_r : mult a one = a .
Axiom neu_mult_l : mult one a = a .
Axiom opp_right : plus a ( opp a ) = zero .
Axiom opp_left : plus (opp a) a = zero .
End Commutative_ring .

Lemma x3 : forall a b : R, (mult (plus one one) (plus a b)) = (plus (plus a a) (plus b b)).
Proof.
 intros.
 rewrite dis_left.
 rewrite dis_right.
 rewrite com_plus.
 rewrite dis_right.
 rewrite neu_mult_l.
 rewrite neu_mult_l.
 rewrite com_plus.
 reflexivity.
Qed.

Require Export List.
Open Scope list_scope.
Import ListNotations.

Inductive is_rev : list nat -> list nat -> Prop :=
| is_rev_nil : is_rev nil nil
| is_rev_cons : forall (n : nat) (l1 l2 : list nat), 
is_rev l1 l2 -> is_rev (n :: l1) (l2 ++ [n]).

Fixpoint rev (l : list nat) {struct l} : list nat :=
match l with
 | nil => nil
 | e::q => (rev q) ++ (e:: nil)
end.

Lemma x4 : forall l1 l2 : list nat, (rev l1) = l2 -> (is_rev l1 l2).
Proof.
 intros.
 induction l1.
 induction l2.
 apply is_rev_nil.
 rewrite <- H.
 simpl.
 apply is_rev_nil.
 rewrite <- H.
 simpl.
 apply is_rev_cons.
 elim l1.
 apply is_rev_nil.
 intros.
 simpl.
 apply is_rev_cons.
 apply H0.
Qed.










