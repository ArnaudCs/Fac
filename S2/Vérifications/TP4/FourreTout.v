Fixpoint mon_exp (n m : nat) : nat := (* met en exposant*)
  match m with
    | 0 => 1
    | S m' => n * mon_exp n m'
  end.
Eval compute in (mon_exp 2 5).

Inductive formula : Type :=
  MonTrue : formula
| MonFalse : formula
| MonEt : formula -> formula -> formula
| MonOu : formula -> formula -> formula
| MonNon : formula -> formula.

Fixpoint eval (F : formula) : bool :=
  match F with
    | MonTrue => true
    | MonFalse => false
    | MonEt F1 F2 => if eval F1 then eval F2 else false
    | MonOu F1 F2 => if eval F1 then true else eval F2
    | MonNon F' => negb (eval F')
  end.
Eval compute in (eval (MonOu (MonEt MonTrue MonFalse) (MonTrue))).

(*------------------------------------------------------------------------------------*)

Fixpoint plus (n m : nat) : nat := (*fonction plus*)
  match n with
    | O => m
    | S n' => S (plus n' m)
  end.

Lemma plus0 (n : nat) : plus n O = n.
Proof.
 induction n.
 reflexivity.
 simpl.
 rewrite IHn.
 reflexivity.
Qed.

Lemma plusS (n p : nat) : plus n (S p) = S (plus n p).
Proof.
 induction n.
 simpl.
 reflexivity.
 simpl.
 rewrite IHn.
 reflexivity.
Qed.

Lemma com : forall n m : nat, (plus n m) = (plus m n).
Proof.
 intros.
 elim n.
 elim m.
 reflexivity.
 intros.
 simpl.
 rewrite <- H.
 simpl.
 reflexivity.
 intros.
 simpl.
 rewrite H.
 elim m.
 simpl.
 reflexivity.
 intros.
 simpl.
 rewrite H0.
 reflexivity.
Qed.

Theorem symetrie (n m : nat) : plus n m = plus m n.
Proof.
 induction n.
 rewrite com.
 reflexivity.
 rewrite com.
 reflexivity.
Qed.

Theorem associativite (n m p : nat) : plus n (plus m p) = plus (plus n m) p.
Proof.
 induction n.
 simpl.
 reflexivity.
 simpl.
 rewrite IHn.
 reflexivity.
Qed.

(*------------------------------------------------------------------------------------*)

Fixpoint double (n:nat) := (*fonction qui double son argument*)
  match n with
  | O => O
  | S n' => S (S (double n'))
  end.

(*------------------------------------------------------------------------------------*)









 









