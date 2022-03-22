Require Import Arith.
Require Import Omega.
Require Export List.
Open Scope list_scope.
Import ListNotations.
Require Import FunInd.

Inductive is_sorted : list nat ->  Prop :=
 | is_sorted_nil : is_sorted nil
 | is_sorted_un : forall e : nat, is_sorted (e::nil)
 | is_sorted_S : forall e : nat, forall h : nat, forall q : list nat, e<=h -> is_sorted(h::q) -> is_sorted(e::h::q).

Inductive is_perm : list nat -> list nat -> Prop :=
 | is_perm_reflex : forall l : list nat, is_perm l l
 | is_perm_S : forall e : nat, forall l l' : list nat, is_perm l l' -> is_perm (e::l) (e::l')
 | is_perm_S_app : forall e : nat, forall l l' : list nat, is_perm l l' -> is_perm (e::l) (l' ++ (e::nil))
 | is_perm_indu : forall e : nat, forall l1 l2 l3 : list nat, is_perm l1 (l2++l3) -> is_perm (e::l1) (l2++(e::l3))
 | is_perm_S_sym : forall l l' : list nat, is_perm l l' -> is_perm l' l
 | is_perm_trans : forall l1 l2 l3 : list nat, is_perm l1 l2 -> is_perm l2 l3 -> is_perm l3 l2.

Fixpoint sort (l : list nat) {struct l} : list nat :=
 match l with
 | nil => nil
 | (a :: nil) => a :: nil 
 | (a :: l) => app (sort l) (a :: nil)
 end. 

Lemma x2 : is_sorted [1; 2; 3].
Proof.
 apply is_sorted_S.
 auto.
 apply is_sorted_S. 
 auto.
 apply is_sorted_un.
Qed.

Lemma x1 : is_perm [1; 2; 3] [3; 2; 1].
Proof.
 apply (is_perm_S_app 1 [2;3] [3;2]).
 apply (is_perm_S_app 2 [3] [3]).
 apply is_perm_reflex.
Qed.

(*-----------------Exercice 2-----------------*)

Check le_dec.

Print sumbool.

Fixpoint le_10 (n : nat) : bool :=
 match (le_dec n 10) with 
  | left _ => true
  | right _ => false
 end.

Eval compute in (le_10 5). (* True *)
Eval compute in (le_10 15). (* False *)

Fixpoint insert (e : nat) (l : list nat) {struct l} : list nat :=
 match l with 
  | nil => [e]
  | h::q => 
   match (le_dec e h) with
    | left _ => e::l  (*e<=h*)
    | right _ => h::(insert e q) 
   end.
 end.

Fixpoint 
 match l with 
  | nil => nil
  | h::q => insert h (insert q)
 end.

Eval compute in (insert 5 [1;2;3]).




 

