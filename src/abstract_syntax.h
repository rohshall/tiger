<<<<<<< HEAD
#ifndef ABSTRACT_SYNTAX
#define ABSTRACT_SYNTAX

#include <cassert>
#include <functional>
#include <memory>
#include "location.hh"
#include "ir_tree.h"

struct DeclarationTable;

struct A_exp;
struct A_ty;
struct A_fieldList;
struct A_expList;
struct A_decList;
struct A_efieldList;
struct A_dec;

struct AstNode {
  yy::location loc;
  std::string type;
  AstNode():type("void") {} 
  AstNode(const yy::location& _loc) : loc(_loc), type("void") {}
  virtual ~AstNode() = default;
  virtual void semanticCheck(DeclarationTable& table) { assert(false); }
  virtual int printast() {};
  virtual IRNode* translate() {};
//  virtual IRNode* translate(DeclarationTable& table){ assert(false); }
};

struct A_Program : AstNode {
  std::unique_ptr<A_exp> exp;
  A_Program(const yy::location& _loc, A_exp* _exp);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};

/*
 * lvalue
 */
struct A_var : AstNode {
  enum class Kind { A_simpleVar, A_fieldVar, A_subscriptVar };
  Kind kind;
  virtual void printast() {};
  virtual IRNode* translate() {};
};
// id
struct A_simpleVar : A_var {
  std::string id;
  A_simpleVar(const yy::location& _loc, const std::string& _id);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// field
struct A_fieldVar : A_var {
  std::unique_ptr<A_var> var;
  std::string id;
  int offset = -1;
  A_fieldVar(const yy::location& _loc, A_var* _var, const std::string& _id);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// array subscript
struct A_subscriptVar : A_var {
  std::unique_ptr<A_var> var;
  std::unique_ptr<A_exp> exp;
  A_subscriptVar(const yy::location& _loc, A_var* _var, A_exp* _exp);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
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
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// funcall
struct A_callExp : A_exp {
  std::string id;
  std::unique_ptr<A_expList> args;
  A_callExp(const yy::location& _loc, const std::string& _id, A_expList* _args);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// lvalue ":=" exp
struct A_assignExp : A_exp {
  std::unique_ptr<A_var> var;
  std::unique_ptr<A_exp> exp;
  A_assignExp(const yy::location& _loc, A_var* _var, A_exp* _exp);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// NIL
struct A_nilExp : A_exp {
  A_nilExp(const yy::location& _loc);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// seq
struct A_seqExp : A_exp {
  std::unique_ptr<A_expList> seq;
  A_seqExp(const yy::location& _loc, A_expList* _seq);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// INT
struct A_intExp : A_exp {
  int val;
  A_intExp(const yy::location& _loc, int _val);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// STRING
struct A_stringExp : A_exp {
  std::string val;
  A_stringExp(const yy::location& _loc, const std::string& _val);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
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

  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// record
struct A_recordExp : A_exp {
  std::string type_id;
  std::unique_ptr<A_efieldList> fields;
  A_recordExp(const yy::location& _loc, const std::string& _typ,
              A_efieldList* _fields);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// array
struct A_arrayExp : A_exp {
  std::string type_id;
  std::unique_ptr<A_exp> size, init;
  A_arrayExp(const yy::location& _loc, const std::string& _typ, A_exp* _size,
             A_exp* _init);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// if
struct A_ifExp : A_exp {
  std::unique_ptr<A_exp> test, tbody, fbody;
  A_ifExp(const yy::location& _loc, A_exp* _test, A_exp* _tbody, A_exp* _fbody);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// while
struct A_whileExp : A_exp {
  std::unique_ptr<A_exp> test, body;
  A_whileExp(const yy::location& _loc, A_exp* _test, A_exp* _body);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// for
struct A_forExp : A_exp {
  std::string id;
  std::unique_ptr<A_exp> low, high, body;
  A_forExp(const yy::location& _loc, const std::string& _id, A_exp* _low,
           A_exp* _high, A_exp* _body);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// break
struct A_breakExp : A_exp {
  A_breakExp(const yy::location& _loc);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// let
struct A_letExp : A_exp {
  std::unique_ptr<A_decList> decs;
  std::unique_ptr<A_expList> body;
  A_letExp(const yy::location& _loc, A_decList* _decs, A_expList* _body);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
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
  A_varDec(const std::string& _id, const std::string& _typ, A_exp* _init);
  A_varDec(const yy::location& _loc, const std::string& _id,
           const std::string& _typ, A_exp* _init);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// tydec
struct A_typeDec : A_dec {
  std::string type_id;
  std::unique_ptr<A_ty> ty;
  A_typeDec(const std::string& _typ, A_ty* _ty);
  A_typeDec(const yy::location& _loc, const std::string& _typ, A_ty* _ty);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// fundec
struct A_functionDec : A_dec {
  std::string id;
  std::unique_ptr<A_fieldList> params;
  std::string type_id;
  std::unique_ptr<A_exp> body;
  A_functionDec(const std::string& _id, A_fieldList* _params,
                const std::string& _typ, A_exp* _body);
  A_functionDec(const yy::location& _loc, const std::string& _id,
                A_fieldList* _params, const std::string& _typ, A_exp* _body);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
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
  A_nameTy(const std::string& _id);
  A_nameTy(const yy::location& _loc, const std::string& _id);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// record
struct A_recordTy : A_ty {
  std::unique_ptr<A_fieldList> record;
  A_recordTy(const yy::location& _loc, A_fieldList* _record);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// array
struct A_arrayTy : A_ty {
  std::string id;
  A_arrayTy(const yy::location& _loc, const std::string& _id);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};

/*
 * tyfields
 */
struct A_field : AstNode {
  std::string id, type_id;
  A_field(const std::string& _id, const std::string& _typ);
  A_field(const yy::location& _loc, const std::string& _id,
          const std::string& _typ);
  int printast() override;
};
struct A_fieldList : AstNode {
  std::unique_ptr<A_field> head;
  std::unique_ptr<A_fieldList> tail;
  A_fieldList(A_field* _head, A_fieldList* _tail);
  A_fieldList(const yy::location& _loc, A_field* _head, A_fieldList* _tail);
  int printast() override;
};

/*
 * explist/args
 */
struct A_expList : AstNode {
  std::unique_ptr<A_exp> head;
  std::unique_ptr<A_expList> tail;
  A_expList(const yy::location& _loc, A_exp* _head, A_expList* _tail);
  int printast() override;
  IRNode* translate() override;
};

/*
 * decs
 */
struct A_decList : AstNode {
  std::unique_ptr<A_dec> head;
  std::unique_ptr<A_decList> tail;
  A_decList(const yy::location& _loc, A_dec* _head, A_decList* _tail);
  int printast() override;
  IRNode* translate() override;
};

/*
 * refields
 */
struct A_efield : AstNode {
  int offset = -1;
  std::string id;
  std::unique_ptr<A_exp> exp;
  A_efield(const yy::location& _loc, const std::string& _id, A_exp* _exp);
  int printast() override;
  IRNode* translate() override;
};

struct A_efieldList : AstNode {
  std::unique_ptr<A_efield> head;
  std::unique_ptr<A_efieldList> tail;
  A_efieldList(const yy::location& _loc, A_efield* _head, A_efieldList* _tail);
  int printast() override;
};

=======
#ifndef ABSTRACT_SYNTAX
#define ABSTRACT_SYNTAX

#include <cassert>
#include <functional>
#include <memory>
#include "location.hh"
#include "ir_tree.h"

struct DeclarationTable;

struct A_exp;
struct A_ty;
struct A_fieldList;
struct A_expList;
struct A_decList;
struct A_efieldList;
struct A_dec;

struct AstNode {
  yy::location loc;
  std::string type;
  AstNode():type("void") {} 
  AstNode(const yy::location& _loc) : loc(_loc), type("void") {}
  virtual ~AstNode() = default;
  virtual void semanticCheck(DeclarationTable& table) { assert(false); }
  virtual int printast() {};
  virtual IRNode* translate() {};
//  virtual IRNode* translate(DeclarationTable& table){ assert(false); }
};

struct A_Program : AstNode {
  std::unique_ptr<A_exp> exp;
  A_Program(const yy::location& _loc, A_exp* _exp);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};

/*
 * lvalue
 */
struct A_var : AstNode {
  enum class Kind { A_simpleVar, A_fieldVar, A_subscriptVar };
  Kind kind;
  virtual int printast() {};
};
// id
struct A_simpleVar : A_var {
  std::string id;
  A_simpleVar(const yy::location& _loc, const std::string& _id);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// field
struct A_fieldVar : A_var {
  std::unique_ptr<A_var> var;
  std::string id;
  int offset = -1;
  A_fieldVar(const yy::location& _loc, A_var* _var, const std::string& _id);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// array subscript
struct A_subscriptVar : A_var {
  std::unique_ptr<A_var> var;
  std::unique_ptr<A_exp> exp;
  A_subscriptVar(const yy::location& _loc, A_var* _var, A_exp* _exp);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
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
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// funcall
struct A_callExp : A_exp {
  std::string id;
  std::unique_ptr<A_expList> args;
  A_callExp(const yy::location& _loc, const std::string& _id, A_expList* _args);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// lvalue ":=" exp
struct A_assignExp : A_exp {
  std::unique_ptr<A_var> var;
  std::unique_ptr<A_exp> exp;
  A_assignExp(const yy::location& _loc, A_var* _var, A_exp* _exp);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// NIL
struct A_nilExp : A_exp {
  A_nilExp(const yy::location& _loc);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// seq
struct A_seqExp : A_exp {
  std::unique_ptr<A_expList> seq;
  A_seqExp(const yy::location& _loc, A_expList* _seq);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// INT
struct A_intExp : A_exp {
  int val;
  A_intExp(const yy::location& _loc, int _val);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// STRING
struct A_stringExp : A_exp {
  std::string val;
  A_stringExp(const yy::location& _loc, const std::string& _val);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
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

  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// record
struct A_recordExp : A_exp {
  std::string type_id;
  std::unique_ptr<A_efieldList> fields;
  A_recordExp(const yy::location& _loc, const std::string& _typ,
              A_efieldList* _fields);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// array
struct A_arrayExp : A_exp {
  std::string type_id;
  std::unique_ptr<A_exp> size, init;
  A_arrayExp(const yy::location& _loc, const std::string& _typ, A_exp* _size,
             A_exp* _init);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// if
struct A_ifExp : A_exp {
  std::unique_ptr<A_exp> test, tbody, fbody;
  A_ifExp(const yy::location& _loc, A_exp* _test, A_exp* _tbody, A_exp* _fbody);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// while
struct A_whileExp : A_exp {
  std::unique_ptr<A_exp> test, body;
  A_whileExp(const yy::location& _loc, A_exp* _test, A_exp* _body);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// for
struct A_forExp : A_exp {
  std::string id;
  std::unique_ptr<A_exp> low, high, body;
  A_forExp(const yy::location& _loc, const std::string& _id, A_exp* _low,
           A_exp* _high, A_exp* _body);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// break
struct A_breakExp : A_exp {
  A_breakExp(const yy::location& _loc);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// let
struct A_letExp : A_exp {
  std::unique_ptr<A_decList> decs;
  std::unique_ptr<A_expList> body;
  A_letExp(const yy::location& _loc, A_decList* _decs, A_expList* _body);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
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
  A_varDec(const std::string& _id, const std::string& _typ, A_exp* _init);
  A_varDec(const yy::location& _loc, const std::string& _id,
           const std::string& _typ, A_exp* _init);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// tydec
struct A_typeDec : A_dec {
  std::string type_id;
  std::unique_ptr<A_ty> ty;
  A_typeDec(const std::string& _typ, A_ty* _ty);
  A_typeDec(const yy::location& _loc, const std::string& _typ, A_ty* _ty);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// fundec
struct A_functionDec : A_dec {
  std::string id;
  std::unique_ptr<A_fieldList> params;
  std::string type_id;
  std::unique_ptr<A_exp> body;
  A_functionDec(const std::string& _id, A_fieldList* _params,
                const std::string& _typ, A_exp* _body);
  A_functionDec(const yy::location& _loc, const std::string& _id,
                A_fieldList* _params, const std::string& _typ, A_exp* _body);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
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
  A_nameTy(const std::string& _id);
  A_nameTy(const yy::location& _loc, const std::string& _id);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// record
struct A_recordTy : A_ty {
  std::unique_ptr<A_fieldList> record;
  A_recordTy(const yy::location& _loc, A_fieldList* _record);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};
// array
struct A_arrayTy : A_ty {
  std::string id;
  A_arrayTy(const yy::location& _loc, const std::string& _id);
  void semanticCheck(DeclarationTable& table) override;
  int printast() override;
  IRNode* translate() override;
  // IRNode* translate(DeclarationTable& table) override;
};

/*
 * tyfields
 */
struct A_field : AstNode {
  std::string id, type_id;
  A_field(const std::string& _id, const std::string& _typ);
  A_field(const yy::location& _loc, const std::string& _id,
          const std::string& _typ);
  int printast() override;
};
struct A_fieldList : AstNode {
  std::unique_ptr<A_field> head;
  std::unique_ptr<A_fieldList> tail;
  A_fieldList(A_field* _head, A_fieldList* _tail);
  A_fieldList(const yy::location& _loc, A_field* _head, A_fieldList* _tail);
  int printast() override;
};

/*
 * explist/args
 */
struct A_expList : AstNode {
  std::unique_ptr<A_exp> head;
  std::unique_ptr<A_expList> tail;
  A_expList(const yy::location& _loc, A_exp* _head, A_expList* _tail);
  int printast() override;
  IRNode* translate() override;
};

/*
 * decs
 */
struct A_decList : AstNode {
  std::unique_ptr<A_dec> head;
  std::unique_ptr<A_decList> tail;
  A_decList(const yy::location& _loc, A_dec* _head, A_decList* _tail);
  int printast() override;
  IRNode* translate() override;
};

/*
 * refields
 */
struct A_efield : AstNode {
  int offset = -1;
  std::string id;
  std::unique_ptr<A_exp> exp;
  A_efield(const yy::location& _loc, const std::string& _id, A_exp* _exp);
  int printast() override;
  IRNode* translate() override;
};

struct A_efieldList : AstNode {
  std::unique_ptr<A_efield> head;
  std::unique_ptr<A_efieldList> tail;
  A_efieldList(const yy::location& _loc, A_efield* _head, A_efieldList* _tail);
  int printast() override;
};

>>>>>>> 6aa2918ba6b0e3a9e3a1c5f8adaa604259dc4638
#endif  // ABSTRACT_SYNTAX
