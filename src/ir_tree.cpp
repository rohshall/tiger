#include "ir_tree.h"
#include <algorithm>

T_seq::T_seq(T_stm* _left, T_stm* _right) : left(_left), right(_right) {
  kind = Kind::T_seq;
}

T_label::T_label(const std::string& _id) : id(std::move(_id)) {
  kind = Kind::T_label;
}

T_jump::T_jump(T_exp* _exp, T_label* _label) : exp(_exp), label(_label) {
  kind = Kind::T_jump;
}

T_cjump::T_cjump(T_relOp _op, T_exp* _left, T_exp* _right, T_label* _true,
                 T_label* _false)
    : op(_op),
      left(_left),
      right(_right),
      label_true(_true),
      label_false(_false) {
  kind = Kind::T_cjump;
}

T_move::T_move(T_exp* _dst, T_exp* _src) : dst(_dst), src(_src) {
  kind = Kind::T_move;
}

T_stm_exp::T_stm_exp(T_exp* _exp) : exp(_exp) { kind = Kind::T_stm_exp; }

T_binop::T_binop(T_binOp _op, T_exp* _left, T_exp* _right)
    : op(_op), left(_left), right(_right) {
  kind = Kind::T_binop;
}

T_mem::T_mem(T_exp* _exp) : exp(_exp) { kind = Kind::T_mem; }

T_temp::T_temp(const std::string& _id) : id(std::move(_id)) {
  kind = Kind::T_temp;
}

T_eseq::T_eseq(T_stm* _stm, T_exp* _exp) : stm(_stm),exp(_exp) {
  kind = Kind::T_eseq;
}

T_name::T_name(const std::string& _id) : id(std::move(_id)) {
  kind = Kind::T_name;
}

T_const::T_const(int _val) : val(_val) { kind = Kind::T_const; }

T_call::T_call(T_exp* _func, T_expList* _args) : func(_func), args(_args) {
  kind = Kind::T_call;
}

T_expList::T_expList(T_exp* _head, T_expList* _tail)
    : head(_head), tail(_tail) {}
T_stmList::T_stmList(T_stm* _head, T_stmList* _tail)
    : head(_head), tail(_tail) {}

T_relOp T_notRel(T_relOp r) {
  switch (r) {
    case T_relOp::T_eq:
      return T_relOp::T_ne;
    case T_relOp::T_ne:
      return T_relOp::T_eq;
    case T_relOp::T_lt:
      return T_relOp::T_ge;
    case T_relOp::T_ge:
      return T_relOp::T_lt;
    case T_relOp::T_gt:
      return T_relOp::T_le;
    case T_relOp::T_le:
      return T_relOp::T_gt;
    case T_relOp::T_ult:
      return T_relOp::T_uge;
    case T_relOp::T_uge:
      return T_relOp::T_ult;
    case T_relOp::T_ule:
      return T_relOp::T_ugt;
    case T_relOp::T_ugt:
      return T_relOp::T_ule;
    default:
      assert(false);
  }
  return T_relOp::T_eq;  // won't go here
}

T_relOp T_commute(T_relOp r) {
  switch (r) {
    case T_relOp::T_eq:
      return T_relOp::T_eq;
    case T_relOp::T_ne:
      return T_relOp::T_ne;
    case T_relOp::T_lt:
      return T_relOp::T_gt;
    case T_relOp::T_ge:
      return T_relOp::T_le;
    case T_relOp::T_gt:
      return T_relOp::T_lt;
    case T_relOp::T_le:
      return T_relOp::T_ge;
    case T_relOp::T_ult:
      return T_relOp::T_ugt;
    case T_relOp::T_uge:
      return T_relOp::T_ule;
    case T_relOp::T_ule:
      return T_relOp::T_uge;
    case T_relOp::T_ugt:
      return T_relOp::T_ult;
    default:
      assert(false);
  }
  return T_relOp::T_eq;  // won't go here
}
