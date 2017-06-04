#ifndef PRINTAST
#define PRINTAST

#include <cassert>
#include "abstract_syntax.h"
#include "cat_driver.h"
#include "symbol_table.h"
#include <iostream>

void A_Program::printast(){
  std::cout<<"A_Program(";
  if(exp!=nullptr)
     exp->printast();
  std::cout<<")";
}

void A_simpleVar::printast(){
    std::cout<<"A_simpleVar("<<id<<")";
}

void A_fieldVar::printast(){
    std::cout<<"A_fieldVar(";
    if(var!=nullptr)
       var->printast();
    std::cout<<"."<<id<<")";
}

void A_subscriptVar::printast(){
    std::cout<<"A_subscriptVar(";
    if(var!=nullptr)
       var->printast();
    std::cout<<"[";
    if(exp!=nullptr)
        exp->printast();
    std::cout<<"])";
}

void A_varExp::printast(){
    std::cout<<"A_varExp(";
    if(var!=nullptr)
        var->printast();
    std::cout<<")";
}

void A_callExp::printast(){
    std::cout<<"A_callExp("<<id<<",";
    if(args!=nullptr)
        args->printast();
    std::cout<<")";
}

void A_assignExp::printast(){
    std::cout<<"A_assignExp(";
    if(var!=nullptr)
        var->printast();
    std::cout<<",";
    if(exp!=nullptr)
        exp->printast();
    std::cout<<")";
}

void A_nilExp::printast(){
    std::cout<<"A_nilExp(nil)";
}

void A_seqExp::printast(){
    std::cout<<"A_seqExp(";
    if(seq!=nullptr)
        seq->printast();
    std::cout<<")";
}

void A_intExp::printast(){
    std::cout<<"A_intExp("<<val<<")";
}

void A_stringExp::printast(){
    std::cout<<"A_stringExp("<<val<<")";
}

void A_opExp::printast(){
    std::cout<<"A_opExp(";
    switch(op){
        case A_oper::plusOp:
            std::cout<<"PLUS";break;
        case A_oper::minusOp:
            std::cout<<"MINUS";break;
        case A_oper::timesOp:
            std::cout<<"TIMES";break;
        case A_oper::divideOp:
            std::cout<<"DIVIDE";break;
        case A_oper::ltOp:
            std::cout<<"LT";break;
        case A_oper::leOp:
            std::cout<<"LE";break;
        case A_oper::gtOp:
            std::cout<<"GT";break;
        case A_oper::geOp:
            std::cout<<"GE";break;
        case A_oper::eqOp:
            std::cout<<"EQ";break;
        case A_oper::neqOp:
            std::cout<<"NEQ";break;
        case A_oper::andOp:
            std::cout<<"AND";break;
        case A_oper::orOp:
            std::cout<<"OR";break; 
    }
    std::cout<<",";
    if(left!=nullptr)
        left->printast();
    std::cout<<",";
    if(right!=nullptr)  
        right->printast();
    std::cout<<")";
}

void A_recordExp::printast(){
    std::cout<<"A_recordExp(TYPE("<<type_id<<"),";
    if(fields!=nullptr)
        fields->printast();
    std::cout<<")";
}

void A_arrayExp::printast(){
    std::cout<<"A_arrayExp(TYPE("<<type_id<<"),SIZE(";
    if(size!=nullptr)
        size->printast();
    std::cout<<"),INIT(";
    if(init!=nullptr)   
        init->printast();
    std::cout<<"))";
}

void A_ifExp::printast(){
    std::cout<<"A_ifExp(IF(";
    if(test!=nullptr)
        test->printast();
    std::cout<<"),THEN(";
    if(tbody!=nullptr)
        tbody->printast();
    std::cout<<"),ELSE(";
    if(fbody!=nullptr)
        fbody->printast();
    std::cout<<"))";
}

void A_whileExp::printast(){
    std::cout<<"A_whileExp(WHILE(";
    if(test!=nullptr)
        test->printast();
    std::cout<<"),DO(";
    if(body!=nullptr)
        body->printast();
    std::cout<<"))";
}

void A_forExp::printast(){
    std::cout<<"A_forExp(LOW(";
    if(low!=nullptr)
        low->printast();
    std::cout<<"),HIGH(";
    if(high!=nullptr)
        high->printast();
    std::cout<<"),BODY(";
    if(body!=nullptr)   
        body->printast();
    std::cout<<"))";
}

void A_breakExp::printast(){
    std::cout<<"A_breakExp()";
}

void A_letExp::printast(){
    std::cout<<"A_letExp(DECS(";
    if(decs!=nullptr)
        decs->printast();
    std::cout<<"),BODY(";
    if(body!=nullptr)
        body->printast();
    std::cout<<"))";
}

void A_varDec::printast(){
    std::cout<<"A_varDec(ID("<<id<<"),TYPE("<<type_id<<"),INIT(";
    if(init!=nullptr)   
        init->printast();
    std::cout<<"))";
}

void A_typeDec::printast(){
    std::cout<<"A_typeDec(ID("<<type_id<<"),TYPE(";
    if(ty!=nullptr)
        ty->printast();
    std::cout<<"))";
}

void A_functionDec::printast(){
    std::cout<<"A_functionDec(ID("<<id<<"),TYPE("<<type_id<<"),PARAM(";
    if(params!=nullptr)
        params->printast();
    std::cout<<"),BODY(";
    if(body!=nullptr)
        body->printast();
    std::cout<<"))";
}

void A_nameTy::printast(){
    std::cout<<"A_nameTy("<<id<<")";
}

void A_recordTy::printast(){
    std::cout<<"A_recordTy(";
    if(record!=nullptr)
        record->printast();
    std::cout<<")";
}

void A_arrayTy::printast(){
    std::cout<<"A_arrayTy("<<id<<")";
}

void A_field::printast(){
    std::cout<<"A_field(ID("<<id<<"),TYPE("<<type_id<<"))";
}

void A_fieldList::printast(){
    std::cout<<"A_fieldList(";
    head->printast();
    A_fieldList* tmp = tail.get();
    while(tmp!=nullptr){
        std::cout<<",";
        tmp->head->printast();
        tmp = tmp->tail.get();
    }
    std::cout<<")";
}

void A_expList::printast(){
    std::cout<<"A_expList(";
    head->printast();
    A_expList* tmp = tail.get();
    while(tmp!=nullptr){
        std::cout<<",";
        tmp->head->printast();
        tmp = tmp->tail.get();
    }
    std::cout<<")";
}

void A_decList::printast(){
    std::cout<<"A_decList(";
    head->printast();
    A_decList* tmp = tail.get();
    while(tmp!=nullptr){
        std::cout<<",";
        tmp->head->printast();
        tmp = tmp->tail.get();
    }
    std::cout<<")";
}

void A_efield::printast(){
    std::cout<<"A_efield(ID("<<id<<"),";
    if(exp!=nullptr)
        exp->printast();
    std::cout<<")";
}

void A_efieldList::printast(){
    std::cout<<"A_efieldList(";
    head->printast();
    A_efieldList* tmp = tail.get();
    while(tmp!=nullptr){
        std::cout<<",";
        tmp->head->printast();
        tmp = tmp->tail.get();
    }
    std::cout<<")";
}

#endif