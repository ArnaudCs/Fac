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

Lemma ex1_1 : forall A B :Set, A * (B -> unit) = A.
Proof.
 intro.
 intro.
 rewrite AR_unit.
 rewrite P_unit.
 reflexivity.
Qed.

Lemma ex1_2 : forall A B :Set, ((A * unit) * B) = (B * (unit * A)).
Proof.
 intros.
 rewrite <- Com.
 rewrite (Com unit A).
 reflexivity.
Qed.

Lemma ex1_3 : forall A B C : Set, (A * unit -> B * (C * unit)) = (A * unit -> (C -> unit) * C) * (unit -> A -> B). 
Proof.
 intros.
 rewrite P_unit. 
 rewrite P_unit.
 rewrite AR_unit.
 rewrite AL_unit.
 rewrite Dis.
 rewrite (Com unit C).
 rewrite P_unit.
 rewrite Com.
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
Lemma ex2_1 : (plus (s o) (s (s o))) = (s (s (s o))).
Proof.
 rewrite ax5.
 rewrite ax5.
 rewrite ax4.
 reflexivity.
Qed.

Lemma ex2_2 : (plus (s (s o)) (s (s o))) = (s (s (s (s o)))).
Proof.
 rewrite ax5.
 rewrite ax5.
 rewrite ax4.
 reflexivity.
Qed.

Lemma ex2_3 : (mult (s (s o)) (s (s o))) = (s (s (s (s o)))).
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


Lemma ex2_12 : (plus (s o) (s (s o))) = (s (s (s o))).
Proof.
 simplifie2.
Qed.

Lemma ex2_22 : (plus (s (s o)) (s (s o))) = (s (s (s (s o)))).
Proof.
 simplifie2.
Qed.

Lemma ex2_32 : (mult (s (s o)) (s (s o))) = (s (s (s (s o)))).
Proof.
 simplifie2.
Qed.

Hint Rewrite ax7 ax6 ax5 ax4 : toto.

Lemma isos2_ex1_3 : (plus (s o) (s (s o))) = (s (s (s o))).
Proof.
 autorewrite with toto using try reflexivity.
Qed.

Lemma isos2_ex2_3 : (plus (s (s o)) (s (s o))) = (s (s (s (s o)))).
Proof.
 autorewrite with toto using try reflexivity.
Qed.

Lemma isos2_ex3_3 : (mult (s (s o)) (s (s o))) = (s (s (s (s o)))).
Proof.
 autorewrite with toto using try reflexivity.
Qed.






