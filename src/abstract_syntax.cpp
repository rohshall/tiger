
#include "abstract_syntax.h"
#include "symbol_table.h"

A_Program(const yy::location& _loc, A_exp* _exp) : AstNode(_loc), exp(_exp) {}

A_simpleVar(const yy::location& _loc, const std::string& _id)
    : id(std::move(_id)) {
  loc = _loc;
  kind = Kind::A_simpleVar;
}

A_fieldVar(const yy::location& _loc, A_var* _var, const std::string& _id)
    : var(_var), id(std::move(_id)) {
  loc = _loc;
  kind = Kind::A_fieldVar;
}

A_subscriptVar(const yy::location& _loc, A_var* _var, A_exp* _exp)
    : var(_var), exp(_exp) {
  loc = _loc;
  kind = Kind::A_subscriptVar;
}

A_varExp(const yy::location& _loc, A_var* _var) : var(_var) {
  loc = _loc;
  kind = Kind::A_varExp;
}

A_callExp(const yy::location& _loc, const std::string& _id, A_expList* _args)
    : id(std::move(_id)), args(_args) {
  loc = _loc;
  kind = Kind::A_callExp;
}

A_assignExp(const yy::location& _loc, A_var* _var, A_exp* _exp)
    : var(_var), exp(_exp) {
  loc = _loc;
  kind = Kind::A_assignExp;
}

A_nilExp(const yy::location& _loc) {
  loc = _loc;
  kind = Kind::A_nilExp;
}

A_seqExp(const yy::location& _loc, A_expList* _seq) : seq(_seq) {
  loc = _loc;
  kind = Kind::A_seqExp;
}

A_intExp(const yy::location& _loc, int _val) : val(_val) {
  loc = _loc;
  kind = Kind::A_intExp;
}

A_stringExp(const yy::location& _loc, const std::string& _val)
    : val(std::move(_val)) {
  loc = _loc;
  kind = Kind::A_stringExp;
}

A_opExp(const yy::location& _loc, A_oper _op, A_exp* _left, A_exp* _right)
    : op(_op), left(_left), right(_right) {
  loc = _loc;
  kind = Kind::A_opExp;
}

A_recordExp(const yy::location& _loc, const std::string& _id,
            A_efieldList* _fields)
    : id(std::move(_id)), fields(_fields) {
  loc = _loc;
  kind = Kind::A_recordExp;
}

A_arrayExp(const yy::location& _loc, const std::string& _id, A_exp* _size,
           A_exp* _init)
    : id(std::move(_id)), size(_size), init(_init) {
  loc = _loc;
  kind = Kind::A_arrayExp;
}

A_ifExp(const yy::location& _loc, A_exp* _test, A_exp* _tbody, A_exp* _fbody)
    : test(_test), tbody(_tbody), fbody(_fbody) {
  loc = _loc;
  kind = Kind::A_ifExp;
}

A_whileExp(const yy::location& _loc, A_exp* _test, A_exp* _body)
    : test(_test), body(_body) {
  loc = _loc;
  kind = Kind::A_whileExp;
}

A_forExp(const yy::location& _loc, A_exp* _low, A_exp* _high, A_exp* _body)
    : low(_low), high(_high), body(_body) {
  loc = _loc;
  kind = Kind::A_forExp;
}

A_breakExp(const yy::location& _loc) {
  loc = _loc;
  kind = Kind::A_breakExp;
}

A_letExp(const yy::location& _loc, A_decList* _decs, A_exp* _body)
    : decs(_decs), body(_body) {
  loc = _loc;
  kind = Kind::A_letExp;
}

A_varDec(const yy::location& _loc, const std::string& _id,
         const std::string& _typ, A_exp* _init)
    : id(std::move(_id)), type_id(std::move(_typ)), init(_init) {
  loc = _loc;
  kind = Kind::A_varDec;
}

A_typeDec(const yy::location& _loc, const std::string& _typ, A_ty* _ty)
    : type_id(std::move(_typ)), ty(_ty) {
  loc = _loc;
  kind = Kind::A_typeDec;
}

A_functionDec(const yy::location& _loc, const string& _id, A_fieldList* _params,
              const string& _typ, A_exp* _body)
    : id(std::move(_id)),
      params(_params),
      type_id(std::move(_typ)),
      body(_body) {
  loc = _loc;
  kind = Kind::A_functionDec;
}

A_nameTy(const yy::location& _loc, const string& _id) : id(std::move(_id)) {
  loc = _loc;
  kind = Kind::
}

A_recordTy(const yy::location& _loc, A_recordTy* _record) : record(_record) {
  loc = _loc;
  kind = Kind::A_nameTy;
}

A_arrayTy(const yy::location& _loc, const std::string& _id)
    : id(std::move(_id)) {
  loc = _loc;
  kind = Kind::A_arrayTy;
}

A_field(const yy::location& _loc, const std::string& _id,
        const std::string& _typ)
    : id(std::move(_id)), type_id(std::move(_typ)) {
  loc = _loc;
}

A_fieldList(const yy::location& _loc, A_field* _head, A_fieldList* _tail)
    : head(_head), tail(_tail) {
  loc = _loc;
}

A_expList(const yy::location& _loc, A_exp* _head, A_exp* _tail)
    : head(_head), tail(_tail) {
  loc = _loc;
}

A_decList(const yy::location& _loc, A_dec* _head, A_decList* _tail)
    : head(_head), tail(_tail) {
  loc = _loc;
}

A_efield(const yy::location& _loc, const std::string& _id, A_exp* _exp)
    : id(std::move(_id)), exp(_exp) {
  loc = _loc;
}

A_efieldList(const yy::location& _loc, A_efield* _head, A_efieldList* _tail)
    : head(_head), tail(_tail) {
  loc = _loc;
}
