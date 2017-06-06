<<<<<<< HEAD
#include "ir_tree.h"
#include <algorithm>

T_seq::T_seq(T_stm* _left, T_stm* _right) : left(_left), right(_right) {
  stmKind = StmKind::T_seq;
  nodeKind = NodeKind::T_stm;
}

T_label::T_label(const std::string& _id) : id(std::move(_id)) {
  stmKind = StmKind::T_label;
  nodeKind = NodeKind::T_stm;
}   

T_jump::T_jump(T_exp* _exp, T_label* _label) : exp(_exp), label(_label) {
  stmKind = StmKind::T_jump;
  nodeKind = NodeKind::T_stm;
}

T_cjump::T_cjump(T_relOp _op, T_exp* _left, T_exp* _right, T_label* _true,
                 T_label* _false)
    : op(_op),
      left(_left),
      right(_right),
      label_true(_true),
      label_false(_false) {
  stmKind = StmKind::T_cjump;
  nodeKind = NodeKind::T_stm;
}

T_move::T_move(T_exp* _dst, T_exp* _src) : dst(_dst), src(_src) {
  stmKind = StmKind::T_move;
  nodeKind = NodeKind::T_stm;
}

T_stm_exp::T_stm_exp(T_exp* _exp) : exp(_exp) {
  stmKind = StmKind::T_stm_exp;
  nodeKind = NodeKind::T_stm;
}

T_binop::T_binop(T_binOp _op, T_exp* _left, T_exp* _right)
    : op(_op), left(_left), right(_right) {
  expKind = ExpKind::T_binop;
  nodeKind = NodeKind::T_exp;
}

T_mem::T_mem(T_exp* _exp) : exp(_exp) {
  expKind = ExpKind::T_mem;
  nodeKind = NodeKind::T_exp;
}

T_temp::T_temp(const std::string& _id) : id(std::move(_id)) {
  expKind = ExpKind::T_temp;
  nodeKind = NodeKind::T_exp;
}

T_eseq::T_eseq(T_stm* _stm, T_exp* _exp) : stm(_stm), exp(_exp) {
  expKind = ExpKind::T_eseq;
  nodeKind = NodeKind::T_exp;
}

T_name::T_name(const std::string& _id) : id(std::move(_id)) {
  expKind = ExpKind::T_name;
  nodeKind = NodeKind::T_exp;
}

T_const::T_const(int _val) : val(_val) {
  expKind = ExpKind::T_const;
  nodeKind = NodeKind::T_exp;
}
/*
T_call::T_call(T_exp* _func, T_expList* _args) : func(_func), args(_args) {
  expKind = ExpKind::T_call;
  nodeKind = NodeKind::T_exp;
}

T_call::T_call(std::string _id, T_expList* _args) : id(_id), args(_args){
  expKind = ExpKind::T_call;
  nodeKind = NodeKind::T_exp;
}
*/
T_call::T_call(T_name* _funcName, T_expList* _args) : funcName(_funcName), args(_args){
  expKind = ExpKind::T_call;
  nodeKind = NodeKind::T_exp;
}

T_exp_stm::T_exp_stm(T_stm* _stm) : stm(_stm) {
  expKind = ExpKind::T_exp_stm;
  nodeKind = NodeKind::T_exp;
}

T_expList::T_expList(T_exp* _head, T_expList* _tail)
    : head(_head), tail(_tail) {
      expKind = ExpKind::T_expList;
      nodeKind = NodeKind::T_exp;
    }

T_stmList::T_stmList(T_stm* _head, T_stmList* _tail)
    : head(_head), tail(_tail) {
      stmKind = StmKind::T_stmList;
      nodeKind = NodeKind::T_stm;
    }

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
=======
#include "ir_tree.h"
#include <algorithm>

T_seq::T_seq(T_stm* _left, T_stm* _right) : left(_left), right(_right) {
  stmKind = StmKind::T_seq;
  nodeKind = NodeKind::T_stm;
}

T_label::T_label(const std::string& _id) : id(std::move(_id)) {
  stmKind = StmKind::T_label;
  nodeKind = NodeKind::T_stm;
}   

T_jump::T_jump(T_exp* _exp, T_label* _label) : exp(_exp), label(_label) {
  stmKind = StmKind::T_jump;
  nodeKind = NodeKind::T_stm;
}

T_cjump::T_cjump(T_relOp _op, T_exp* _left, T_exp* _right, T_label* _true,
                 T_label* _false)
    : op(_op),
      left(_left),
      right(_right),
      label_true(_true),
      label_false(_false) {
  stmKind = StmKind::T_cjump;
  nodeKind = NodeKind::T_stm;
}

T_move::T_move(T_exp* _dst, T_exp* _src) : dst(_dst), src(_src) {
  stmKind = StmKind::T_move;
  nodeKind = NodeKind::T_stm;
}

T_stm_exp::T_stm_exp(T_exp* _exp) : exp(_exp) {
  stmKind = StmKind::T_stm_exp;
  nodeKind = NodeKind::T_stm;
}

T_binop::T_binop(T_binOp _op, T_exp* _left, T_exp* _right)
    : op(_op), left(_left), right(_right) {
  expKind = ExpKind::T_binop;
  nodeKind = NodeKind::T_exp;
}

T_mem::T_mem(T_exp* _exp) : exp(_exp) {
  expKind = ExpKind::T_mem;
  nodeKind = NodeKind::T_exp;
}

T_temp::T_temp(const std::string& _id) : id(std::move(_id)) {
  expKind = ExpKind::T_temp;
  nodeKind = NodeKind::T_exp;
}

T_eseq::T_eseq(T_stm* _stm, T_exp* _exp) : stm(_stm), exp(_exp) {
  expKind = ExpKind::T_eseq;
  nodeKind = NodeKind::T_exp;
}

T_name::T_name(const std::string& _id) : id(std::move(_id)) {
  expKind = ExpKind::T_name;
  nodeKind = NodeKind::T_exp;
}

T_const::T_const(int _val) : val(_val) {
  expKind = ExpKind::T_const;
  nodeKind = NodeKind::T_exp;
}
/*
T_call::T_call(T_exp* _func, T_expList* _args) : func(_func), args(_args) {
  expKind = ExpKind::T_call;
  nodeKind = NodeKind::T_exp;
}

T_call::T_call(std::string _id, T_expList* _args) : id(_id), args(_args){
  expKind = ExpKind::T_call;
  nodeKind = NodeKind::T_exp;
}
*/
T_call::T_call(T_name* _funcName, T_expList* _args) : funcName(_funcName), args(_args){
  expKind = ExpKind::T_call;
  nodeKind = NodeKind::T_exp;
}

T_exp_stm::T_exp_stm(T_stm* _stm) : stm(_stm) {
  expKind = ExpKind::T_exp_stm;
  nodeKind = NodeKind::T_exp;
}

T_expList::T_expList(T_exp* _head, T_expList* _tail)
    : head(_head), tail(_tail) {
      expKind = ExpKind::T_expList;
      nodeKind = NodeKind::T_exp;
    }

T_stmList::T_stmList(T_stm* _head, T_stmList* _tail)
    : head(_head), tail(_tail) {
      stmKind = StmKind::T_stmList;
      nodeKind = NodeKind::T_stm;
    }

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
>>>>>>> 6aa2918ba6b0e3a9e3a1c5f8adaa604259dc4638
