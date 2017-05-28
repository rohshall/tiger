#ifndef IR_TREE
#define IR_TREE

#include <memory>
#include <string>
#include <cassert>

struct T_expList;
struct T_stmList;
struct T_exp;

enum class T_binOp{
  T_plus, T_minus, T_mul, T_div,T_and, T_or, T_lshift, T_rshift, T_arshift, T_xor
};

enum class T_relOp{
  T_eq, T_ne, T_lt, T_gt, T_le, T_ge,T_ult, T_ule, T_ugt, T_uge
};

T_relOp T_notRel(T_relOp r);
T_relOp T_commute(T_relOp r);

struct IRNode{
  virtual ~IRNode() = default;
};

/**
 * T_stm 
 */
struct T_stm : IRNode{
  enum class Kind{
    T_seq, 
    T_label,
    T_jump,
    T_cjump,
    T_move,
    T_stm_exp
  };
  Kind kind;
};

struct T_seq : T_stm{
  std::unique_ptr<T_stm> left, right;
  T_seq(T_stm* _left,T_stm* _right);
};

struct T_label : T_stm{
  std::string id;
  T_label(const std::string& _id);
};

struct T_jump : T_stm{
  std::unique_ptr<T_exp> exp;
  std::unique_ptr<T_label> label;
  T_jump(T_exp* _exp,T_label* _label);
};

struct T_cjump : T_stm{
  T_relOp op;
  std::unique_ptr<T_exp> left,right;
  std::unique_ptr<T_label> label_true,label_false;
  T_cjump(T_relOp _op,T_exp* _left,T_exp* _right,T_label* _true,T_label* _false);
};

struct T_move : T_stm{
  std::unique_ptr<T_exp> dst,src;
  T_move(T_exp* _dst,T_exp* _src);
};

struct T_stm_exp : T_stm{
  std::unique_ptr<T_exp> exp;
  T_stm_exp(T_exp* _exp);
};

/**
 * T_exp
 */

struct T_exp : IRNode{
  enum class Kind{
    T_binop,
    T_mem,
    T_temp,
    T_eseq,
    T_name,
    T_const,
    T_call
  };
  Kind kind;
};


struct T_binop : T_exp{
  T_binOp op;
  std::unique_ptr<T_exp> left,right;
  T_binop(T_binOp _op,T_exp* _left,T_exp* _right);
};

struct T_mem : T_exp{
  std::unique_ptr<T_exp> exp;
  T_mem(T_exp* _exp);
};

struct T_temp : T_exp{
  std::string id;
  T_temp(const std::string& _id);
};

struct T_eseq : T_exp{
  std::unique_ptr<T_stm> stm;
  std::unique_ptr<T_exp> exp;
  T_eseq(T_stm* _stm, T_exp* _exp);
};

struct T_name : T_exp{
  std::string id;
  T_name(const std::string& _id);
};

struct T_const : T_exp{
  int val;
  T_const(int _val);
};

struct T_call : T_exp{
  std::unique_ptr<T_exp> func;
  std::unique_ptr<T_expList> args;
  T_call(T_exp* _func,T_expList* _args);
};

struct T_expList : IRNode{
  std::unique_ptr<T_exp> head;
  std::unique_ptr<T_expList> tail;
  T_expList(T_exp* _head, T_expList* _tail);
};

struct T_stmList : IRNode{
  std::unique_ptr<T_stm> head;
  std::unique_ptr<T_stmList> tail;
  T_stmList(T_stm* _head, T_stmList* _tail);
};




#endif //IR_TREE_H