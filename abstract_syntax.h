#include <cassert>
#include <memory>
#include "location.hh"

struct AstNode {
  yy::location loc;
  virtual ~AstNode() = default;
};

enum class A_oper { plus minus, times, divide, eq, neq, lt, le, gt, ge };

/*
 * lvalue
 */
struct A_var : AstNode {
  enum class Kind { A_simpleVar, A_fieldVar, A_subscriptVar };
  Kind kind;
};
struct A_simpleVar : A_var {
  std::string id;
  A_simpleVar(const std::string& _id);
};
struct A_fieldVar : A_var {
  std::unique_ptr<A_var> var;
  std::string id;
  A_fieldVar(A_var* _var, const std::string& _id);
};
struct A_subscriptVar : A_var {
  std::unique_ptr<A_var> var;
  std::unique_ptr<A_exp> exp;
  A_subscriptVar(A_var* _var, A_exp* _exp);
};

/*
 * exp
 */
struct A_exp : AstNode {
  enum class Kind {
    A_varExp,
    A_nilExp,
    A_intExp,
    A_stringExp,
    A_callExp,
    A_opExp,
    A_recordExp,
    A_seqExp,
    A_assignExp,
    A_ifExp,
    A_whileExp,
    A_forExp,
    A_breakExp,
    A_letExp,
    A_arrayExp
  };
  Kind kind;
};
struct A_varExp : A_exp {
  std::unique_ptr<A_var> var;
  A_varExp(A_var* _var);
};
struct A_nilExp : A_exp {};
struct A_intExp : A_exp {
  int val;
  A_intExp(int _val);
};
struct A_stringExp : A_exp {
  std::string val;
  A_stringExp(const std::string& _val);
};
struct A_callExp : A_exp {
  std::string id;
  std::unique_ptr<A_expList> args;
  A_callExp(const std::string& _id, A_expList* _args);
};
struct A_opExp : A_exp {
  A_oper oper;
  std::unique_ptr<A_exp> left;
  std::unique_ptr<A_exp> right;
  A_opExp(A_exp* _left, A_exp* _right);
};
struct A_recordExp : A_exp {
  std::string id;
  std::unique_ptr<A_efieldList> fields;
  A_recordExp(const std::string& _id, A_efieldList* _fields);
};
struct A_seqExp : A_exp {
  std::unique_ptr<A_expList> seq;
  A_seqExp(A_expList* seq);
};
struct A_assignExp : A_exp {
  std::unique<A_var> var;
  std::unique_ptr<A_exp> exp;
  A_assignExp(A_var* _var, A_exp* _exp);
};
struct A_ifExp : A_exp {
  std::unique_ptr<A_exp> eif, ethen, eelse;
  A_ifExp(A_exp* _eif, A_exp* _ethen, A_exp* _eelse);
};
struct A_whileExp : A_exp {
  std::unique_ptr<A_exp> test, body;
  A_whileExp(A_exp* _test, A_exp* _body);
};
struct A_forExp : A_exp {
  std::string id;
  std::unique_ptr<A_exp> low, high, body;
  bool escape;
  A_forExp(A_exp* _low, A_exp* _high, A_exp* _body);
};
struct A_breakExp : A_exp {};
struct A_letExp : A_exp {
  std::unique_ptr<A_decList> decs;
  std::unique_ptr<A_exp> body;
  A_letExp(A_decList* _decs, A_exp* _body)
};
struct A_arrayExp : A_exp {
  std::string id;
  std::unique_ptr<A_exp> size, init;
  A_arrayExp(const std::string& _id, A_exp* size, A_exp* _init);
};

/*
 * dec
 */