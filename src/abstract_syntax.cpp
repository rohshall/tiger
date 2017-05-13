
#include "abstract_syntax.h"
#include "symbol_table.h"

A_Program(const yy::location& _loc, A_exp* _exp)
	:AstNode(_loc) {

}
A_simpleVar(const yy::location& _loc, const std::string& _id) {
	
}
A_fieldVar(const yy::location& _loc, A_var* _var, const std::string& _id) {

}
A_subscriptVar(const yy::location& _loc, A_var* _var, A_exp* _exp) {

}
A_varExp(const yy::location& _loc, A_var* _var) {

}
A_callExp(const yy::location& _loc, const std::string& _id, A_expList* _args) {

}

A_assignExp(const yy::location& _loc, A_var* _var, A_exp* _exp) {

}
A_nilExp(const yy::location& _loc) {

}
A_seqExp(const yy::location& _loc, A_expList* _seq) {

}
A_intExp(const yy::location& _loc, int _val) {

}
A_stringExp(const yy::location& _loc, const std::string& _val) {

}
A_opExp(const yy::location& _loc, A_oper _op, A_exp* _left, A_exp* _right){

}
A_recordExp(const yy::location& _loc, const std::string& _id,A_efieldList* _fields) {

}
A_arrayExp(const yy::location& _loc, const std::string& _id, A_exp* size,A_exp* _init) {

}
A_ifExp(const yy::location& _loc, A_exp* _test, A_exp* _tbody, A_exp* _fbody) {

}
A_whileExp(const yy::location& _loc, A_exp* _test, A_exp* _body) {}
A_forExp(const yy::location& _loc, A_exp* _low, A_exp* _high, A_exp* _body) {

}
A_breakExp(const yy::location& _loc) {

}
A_letExp(const yy::location& _loc, A_decList* _decs, A_exp* _body) {

}
A_varDec(const yy::location& _loc, const std::string& _id,const std::string& _typ, A_exp* _init) {

}
A_typeDec(const yy::location& _loc, const std::string& _typ, A_ty* _ty){

}
A_functionDec(const yy::location& _loc, const string& _id,A_fieldList* _params, const string& _typ, A_exp* _body) {

}
A_nameTy(const yy::location& _loc, const string& _id) {

}
A_recordTy(const yy::location& _loc, A_recordTy* _record) {

}
A_arrayTy(const yy::location& _loc, const std::string& _id) {

}
A_field(const yy::location& _loc, const std::string& _id,const std::string& _typ) {

}
A_fieldList(const yy::location& _loc, A_field* _head, A_fieldList* _tail) {

}
A_expList(const yy::location& _loc, A_exp* _head, A_exp* _tail) {

}
A_decList(const yy::location& _loc, A_dec* _head, A_decList* _tail) {

}
A_efield(const yy::location& _loc, const std::string& _id, A_exp* _exp) {

}
A_efieldList(const yy::location& _loc, A_efield* _head, A_efieldList* _tail) {

}
