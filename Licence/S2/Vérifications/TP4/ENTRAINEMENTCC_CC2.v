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

Lemma ex1_3 : A /\ B -> A.
Proof.
 intros.
 elim H.
 intros.
 assumption.
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
 intros.
 apply H0. 
 assumption.
 intros.
 apply H1.
 assumption.
Qed.

Lemma ex1_6 : A -> False -> ~A.
Proof.
 intros.
 intro.
 assumption.
Qed.

Lemma ex1_7 : False -> A.
Proof.
 intros.
 elimtype False.
 assumption.
Qed.

Lemma ex1_8 : (A <-> B) -> A -> B.
Proof.
 intros.
 elim H.
 intros.
 apply H1.
 assumption.
Qed.

Lemma ex1_9 : (A <-> B) -> B -> A.
Proof.
 intros.
 elim H.
 intros.
 apply H2.
 assumption.
Qed.

Lemma ex1_10 : (A -> B) -> (B -> A) -> (A <-> B).
Proof.
 intros.
 split.
 intros.
 apply H.
 assumption.
 intros.
 apply H0.
 assumption.
Qed.

(*Exercice 2*)

Parameter E : Set .
Parameters Q P :E-> Prop. 

Lemma ex2_1 : (forall x : E, P(x) -> (exists y : E, P(y) \/ Q(y))).
Proof.
 intros.
 exists x.
 left.
 assumption.
Qed.

Lemma ex2_2 : (exists x : E, P(x) \/ Q(x)) -> (exists x : E, P(x)) \/ (exists x : E, Q(x)).
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

Lemma ex2_3 : (forall x : E, P(x)) /\ (forall x : E, Q(x)) -> (forall x : E, P(x) /\ Q(x)).
Proof.
 intros.
 split.
 elim H.
 intros.
 apply H0.
 elim H.
 intros.
 apply H1.
Qed.

Lemma ex2_4 : (forall x : E, P(x) /\ Q(x)) -> (forall x : E, P(x)) /\ (forall x : E, Q(x)).
Proof.
 intros.
 split.
 intro.
 apply H.
 apply H.
Qed.

Lemma ex2_5 : (forall x : E, ~P(x)) -> ~(exists x : E, P(x)).
Proof.
 intros.
 intro.
 elim H0.
 apply H.
Qed.

Require Import Classical.

Lemma ex2_6 : ~(forall x : E, P(x)) -> (exists x : E, ~P(x)).
Proof.
 intros.
 apply NNPP.
 intro.
 elimtype False.
 apply H.
 intro.
 apply NNPP.
 intro.
 elimtype False.
 apply H0.
 exists x.
 assumption.
Qed.

Open Scope type_scope.
Section Iso_axioms.
Variables A B C : Set.
Axiom Com : A * B = B * A.
Axiom Ass : A * (B * C) = (A * B) * C.
Axiom Cur : ((A * B) -> C) = (A -> (B * C)).
Axiom Dis : (A -> (B * C)) = (A -> B) * (A -> C).
Axiom P_unit : A * unit = A.
Axiom AR_unit : (A -> unit) = unit.
Axiom AL_unit : (unit -> A) = A.

End Iso_axioms.

Ltac simplifie :=
 intros;
 repeat
  (rewrite P_unit || rewrite AR_unit || rewrite AL_unit);
 try reflexivity.

Lemma ex1 : forall A B :Set, A * (B -> unit) = A.
Proof.
 intro.
 intro.
 rewrite AR_unit.
 rewrite P_unit.
 reflexivity.
Qed.

Lemma ex2 : forall A B :Set, ((A * unit) * B) = (B * (unit * A)).
Proof.
 intros.
 rewrite <- Com.
 rewrite (Com unit A0).
 reflexivity.
Qed.

Section Peano.
Parameter N : Set.
Parameter o : N.
Parameter s : N -> N.
Parameters plus mult : N -> N -> N.
Variables x y : N.
Axiom ax1 : ~((s x) = o).
Axiom ax2 : exists z, ~(x = o) -> (s z) = x.
Axiom ax3 : (s x) = (s y) -> x = y.
Axiom ax4 : (plus x o) = x.
Axiom ax5 : (plus x (s y)) = s (plus x y).
Axiom ax6 : (mult x o) = o.
Axiom ax7 : (mult x (s y)) = (plus (mult x y) x).
End Peano.

Ltac simplifie2 :=
 intros;
 repeat
  (rewrite ax7 || rewrite ax6 || rewrite ax5 || rewrite ax4);
 try reflexivity.

Lemma ex4 : (plus (s o) (s (s o))) = (s (s (s o))).
Proof.
 rewrite ax5.
 rewrite ax5.
 rewrite ax4.
 reflexivity.
Qed.

Lemma ex5 : (plus (s (s o)) (s (s o))) = (s (s (s (s o)))).
Proof.
 rewrite ax5.
 rewrite ax5.
 rewrite ax4.
 reflexivity.
Qed.

Lemma ex6 : (mult (s (s o)) (s (s o))) = (s (s (s (s o)))).
Proof.
 rewrite ax7.
 rewrite ax7.
 rewrite ax5.
 rewrite ax5.
 rewrite ax4.
 rewrite ax5.
 rewrite ax6.
 rewrite ax5.
 rewrite ax4.
 reflexivity.
Qed.





 