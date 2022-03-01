Parameters A B C : Prop.

Lemma ex1_1 : A -> B -> A.
Proof.
  intros.
  assumption.
Qed.

Lemma ex1_2 : (A -> B -> C) -> (A -> B) -> A -> C.
Proof.
  intros.
  apply H.
  assumption.
  apply H0.
  assumption.
Qed.
  
Lemma ex1_3 : A /\ B -> B. 
Proof.
  intros.
  apply H.
Qed.

Lemma ex1_4 : B -> A \/ B.
Proof.
  intros.
  right.
  assumption.
Qed.

Lemma ex1_5 : (A \/ B) -> (A -> C) -> (B -> C) -> C.
Proof.
  intros.
  elim H.
  apply H0.
  assumption.
Qed.

Lemma ex1_6 : A-> False -> ~ A.
Proof.
  intros.
  auto.
Qed.

Lemma ex1_7 : False -> A.
Proof.
  intro.
  elimtype False.
  assumption.
Qed.

Lemma ex1_8 : (A<->B)-> (A->B).
Proof.
  intros.
  elim H.
  intros.
  apply H.
  assumption.
Qed.

Lemma ex1_9 : (A<->B)-> (B->A).
Proof.
  intros.
  elim H.
  intros.
  apply H2.
  assumption.
Qed.

Lemma ex1_10 : (A->B)-> (B->A) -> (A<->B).
Proof.
  intros.
  split.
  assumption.
  assumption.
Qed.
  
Parameter E :Set .
Parameters Q P :E-> Prop. 

Lemma ex2_1 : (forall x : E, P(x) ->  (exists y : E , P(y) \/ Q(y))).
Proof.
  intros.
  exists x.
  left.
  assumption.
Qed.

Lemma ex2_2 : (exists x : E, (P x) \/ (Q x))-> (exists x : E,(P x) ) \/ ( exists x : E,(Q x)).
Proof.
  intros.
  elim H.
  intros.
  elim H0.
  left.
  exists x.
  assumption.
  right.
  exists x.
  assumption.
Qed.

Lemma ex2_3 : ((forall x : E, (P x)) /\ (forall x : E, Q x))-> (forall x : E, (P x)  /\ (Q x)).
Proof.
  intros.
  elim H.
  intros.
  split.
  apply H0.
  apply H1.
Qed.

Lemma ex2_4 : (forall x : E, (P x)  /\ (Q x)) -> ((forall x : E, (P x)) /\ (forall x : E, (Q x))).
Proof.
  intros.
  split.
  intros.
  apply H.
  apply H.
Qed.

Lemma ex2_5 : (forall x : E, ~(P x))-> ~(exists x : E, (P x) ).
Proof.
  intro.
  intro.
  elim H0.
  apply H.
Qed.
  
  
Require Import Classical.

Lemma ex2_6 : ~(forall x : E, (P x))-> (exists x : E, ~(P x) ).
Proof.
  intros.
  apply NNPP.
  intro.
  elimtype False ;
  apply H.
  intro.
  apply NNPP.
  intro.
  elimtype False ;
  apply H0.
  exists x.
  assumption.
Qed.  