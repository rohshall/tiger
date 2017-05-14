#ifndef ABSTRACT_SYNTAX
#define ABSTRACT_SYNTAX

#include <cassert>
#include <memory>
#include "location.hh"

struct AstNode {
  yy::location loc;
  AstNode() = default;
  AstNode(const yy::location& _loc) : loc(_loc) {}
  virtual ~AstNode() = default;
};

struct A_exp;
struct A_ty;
struct A_fieldList;
struct A_expList;
struct A_decList;
struct A_efieldList;

struct A_Program : AstNode {
  std::unique_ptr<A_exp> exp;
  A_Program(const yy::location& _loc, A_exp* _exp);
};

/*
 * lvalue
 */
struct A_var : AstNode {
  enum class Kind { A_simpleVar, A_fieldVar, A_subscriptVar };
  Kind kind;
};
// id
struct A_simpleVar : A_var {
  std::string id;
  A_simpleVar(const yy::location& _loc, const std::string& _id);
};
// field
struct A_fieldVar : A_var {
  std::unique_ptr<A_var> var;
  std::string id;
  A_fieldVar(const yy::location& _loc, A_var* _var, const std::string& _id);
};
// array subscript
struct A_subscriptVar : A_var {
  std::unique_ptr<A_var> var;
  std::unique_ptr<A_exp> exp;
  A_subscriptVar(const yy::location& _loc, A_var* _var, A_exp* _exp);
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
// lvalue
struct A_varExp : A_exp {
  std::unique_ptr<A_var> var;
  A_varExp(const yy::location& _loc, A_var* _var);
};
// funcall
struct A_callExp : A_exp {
  std::string id;
  std::unique_ptr<A_expList> args;
  A_callExp(const yy::location& _loc, const std::string& _id, A_expList* _args);
};
// lvalue ":=" exp
struct A_assignExp : A_exp {
  std::unique_ptr<A_var> var;
  std::unique_ptr<A_exp> exp;
  A_assignExp(const yy::location& _loc, A_var* _var, A_exp* _exp);
};
// NIL
struct A_nilExp : A_exp {
  A_nilExp(const yy::location& _loc);
};
// seq
struct A_seqExp : A_exp {
  std::unique_ptr<A_expList> seq;
  A_seqExp(const yy::location& _loc, A_expList* _seq);
};
// INT
struct A_intExp : A_exp {
  int val;
  A_intExp(const yy::location& _loc, int _val);
};
// STRING
struct A_stringExp : A_exp {
  std::string val;
  A_stringExp(const yy::location& _loc, const std::string& _val);
};
// exp op exp
enum class A_oper {
  plusOp,
  minusOp,
  timesOp,
  divideOp,
  eqOp,
  neqOp,
  ltOp,
  leOp,
  gtOp,
  geOp,
  andOp,
  orOp
};
struct A_opExp : A_exp {
  A_oper op;
  std::unique_ptr<A_exp> left;
  std::unique_ptr<A_exp> right;
  A_opExp(const yy::location& _loc, A_oper _op, A_exp* _left, A_exp* _right);
};
// record
struct A_recordExp : A_exp {
  std::string type_id;
  std::unique_ptr<A_efieldList> fields;
  A_recordExp(const yy::location& _loc, const std::string& _typ,A_efieldList* _fields);
};
// array
struct A_arrayExp : A_exp {
  std::string type_id;
  std::unique_ptr<A_exp> size, init;
  A_arrayExp(const yy::location& _loc, const std::string& _typ, A_exp* _size,A_exp* _init);
};
// if
struct A_ifExp : A_exp {
  std::unique_ptr<A_exp> test, tbody, fbody;
  A_ifExp(const yy::location& _loc, A_exp* _test, A_exp* _tbody, A_exp* _fbody);
};
// while
struct A_whileExp : A_exp {
  std::unique_ptr<A_exp> test, body;
  A_whileExp(const yy::location& _loc, A_exp* _test, A_exp* _body);
};
// for
struct A_forExp : A_exp {
  std::string id; 
  std::unique_ptr<A_exp> low, high, body;
  bool escape;
  A_forExp(const yy::location& _loc,const std::string& _id, A_exp* _low, A_exp* _high, A_exp* _body);
};
// break
struct A_breakExp : A_exp {
  A_breakExp(const yy::location& _loc);
};
// let
struct A_letExp : A_exp {
  std::unique_ptr<A_decList> decs;
  std::unique_ptr<A_expList> body;
  A_letExp(const yy::location& _loc, A_decList* _decs, A_expList* _body);
};

/*
 * dec
 */
struct A_dec : AstNode {
  enum class Kind { A_functionDec, A_varDec, A_typeDec };
  Kind kind;
};
// vardec
struct A_varDec : A_dec {
  std::string id;
  std::string type_id;
  std::unique_ptr<A_exp> init;
  bool escape;
  A_varDec(const yy::location& _loc, const std::string& _id,const std::string& _typ, A_exp* _init);
};
// tydec
struct A_typeDec : A_dec {
  std::string type_id;
  std::unique_ptr<A_ty> ty;
  A_typeDec(const yy::location& _loc, const std::string& _typ, A_ty* _ty);
};
// fundec
struct A_functionDec : A_dec {
  std::string id;
  std::unique_ptr<A_fieldList> params;
  std::string type_id;
  std::unique_ptr<A_exp> body;
  A_functionDec(const yy::location& _loc, const std::string& _id,A_fieldList* _params, const std::string& _typ, A_exp* _body);
};

/*
 * ty
 */
struct A_ty : AstNode {
  enum class Kind { A_nameTy, A_recordTy, A_arrayTy };
  Kind kind;
};
// id
struct A_nameTy : A_ty {
  std::string id;
  A_nameTy(const yy::location& _loc,const std::string& _id);
};
// record
struct A_recordTy : A_ty {
  std::unique_ptr<A_fieldList> record;
  A_recordTy(const yy::location& _loc,A_fieldList* _record);
};
// array
struct A_arrayTy : A_ty {
  std::string id;
  A_arrayTy(const yy::location& _loc,const std::string& _id);
};

/*
 * tyfields
 */
struct A_field : AstNode {
  std::string id, type_id;
  bool escape;
  A_field(const yy::location& _loc,const std::string& _id, const std::string& _typ);
};
struct A_fieldList : AstNode {
  std::unique_ptr<A_field> head;
  std::unique_ptr<A_fieldList> tail;
  A_fieldList(const yy::location& _loc,A_field* _head, A_fieldList* _tail);
};

/*
 * explist/args
 */
struct A_expList : AstNode {
  std::unique_ptr<A_exp> head;
  std::unique_ptr<A_expList> tail;
  A_expList(const yy::location& _loc,A_exp* _head, A_expList* _tail);
};

/*
 * decs
 */
struct A_decList : AstNode {
  std::unique_ptr<A_dec> head;
  std::unique_ptr<A_decList> tail;
  A_decList(const yy::location& _loc,A_dec* _head, A_decList* _tail);
};

/*
 * refields
 */
struct A_efield : AstNode{
  std::string id;
  std::unique_ptr<A_exp> exp;
  A_efield(const yy::location& _loc,const std::string& _id, A_exp* _exp);
};
struct A_efieldList : AstNode{
  std::unique_ptr<A_efield> head;
  std::unique_ptr<A_efieldList> tail;
  A_efieldList(const yy::location& _loc,A_efield* _head, A_efieldList* _tail);
};

#endif //ABSTRACT_SYNTAX