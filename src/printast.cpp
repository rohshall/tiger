#ifndef PRINTAST
#define PRINTAST

#include <cassert>
#include "abstract_syntax.h"
#include "cat_driver.h"
#include "symbol_table.h"
#include <iostream>

FILE *fp;
static int cc = 0;

int A_Program::printast(){
    int num,t;
    num = cc;
    cc++;
    if((fp = fopen("ast.dot","w"))==NULL)
        std::cout<<"open file ast.dot failed"<<std::endl;
    fprintf(fp,"digraph ast {\nnode[shape = record];\n");
    std::cout<<"A_Program(";
    fprintf(fp,"a%d[label = \"<f0> | <f1> Program |<f2> \"];\n",num);
    if(exp!=nullptr){
        t = exp->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
    }
    std::cout<<")";
    fprintf(fp,"}");
    fclose(fp);
    return 0;
}

int A_simpleVar::printast(){
    int num,t;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> simpleVar |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,id.c_str());
    fprintf(fp,"a%d:f1:s -> a%d:f1 [label = ID];\n",num, t);
    std::cout<<"A_simpleVar("<<id<<")";
    return num;
}

int A_fieldVar::printast(){
    int num,t,p;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> fieldVar |<f2> \"];\n",num);
    std::cout<<"A_fieldVar(";
    if(var!=nullptr){
        p = var->printast();
        fprintf(fp,"a%d:f0:sw -> a%d:f1;\n",num, p);
    }
    std::cout<<"."<<id<<")";
    fprintf(fp,"a%d[label = \"<f0> | <f1> id:%s |<f2> \"];\n",t,id.c_str());
    fprintf(fp,"a%d:f2:se -> a%d:f1;\n",num, t);
    return num;
}

int A_subscriptVar::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> subscriptVar |<f2> \"];\n",num);
    std::cout<<"A_subscriptVar(";
    if(var!=nullptr){
        t = var->printast();
        fprintf(fp,"a%d:f0:sw -> a%d:f1;\n",num, t);
    }
    std::cout<<"[";
    if(exp!=nullptr){
        t = exp->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1;\n",num, t);
    }
    std::cout<<"])";
    return num;
}

int A_varExp::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> varExp |<f2> \"];\n",num);
    std::cout<<"A_varExp(";
    if(var!=nullptr){
        t = var->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
    }
    std::cout<<")";
    return num;
}

int A_callExp::printast(){
    int num,t,p;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> callExp |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,id.c_str());
    fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = ID];\n",num, t);
    std::cout<<"A_callExp("<<id<<",";
    if(args!=nullptr){
        p = args->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1;\n",num, p);
    }
    std::cout<<")";
    return num;
}

int A_assignExp::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> assignExp |<f2> \"];\n",num);
    std::cout<<"A_assignExp(";
    if(var!=nullptr){
        t = var->printast();
        fprintf(fp,"a%d:f0:sw -> a%d:f1;\n",num, t);
    }
    std::cout<<",";
    if(exp!=nullptr){
        t = exp->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1;\n",num, t);
    }
    std::cout<<")";
    return num;
}

int A_nilExp::printast(){
    int num;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> nilExp |<f2> \"];\n",num);
    std::cout<<"A_nilExp(nil)";
    return num;
}

int A_seqExp::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> seqExp |<f2> \"];\n",num);
    std::cout<<"A_seqExp(";
    if(seq!=nullptr){
        t = seq->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
    }
    std::cout<<")";
    return num;
}

int A_intExp::printast(){
    int num,t;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> intExp |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %d |<f2> \"];\n",t,val);
    fprintf(fp,"a%d:f1:s -> a%d:f1 [label = VAL];\n",num, t);
    std::cout<<"A_intExp("<<val<<")";
    return num;
}

int A_stringExp::printast(){
    int num,t;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> intExp |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,val.c_str());
    fprintf(fp,"a%d:f1:s -> a%d:f1 [label = VAL];\n",num, t);
    std::cout<<"A_stringExp("<<val<<")";
    return num;
}

int A_opExp::printast(){
    int num,t,p;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> opExp |<f2> \"];\n",num);
    std::cout<<"A_opExp(";
    switch(op){
        case A_oper::plusOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> PLUS |<f2> \"];\n",t);std::cout<<"PLUS";break;
        case A_oper::minusOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> MINUS |<f2> \"];\n",t);std::cout<<"MINUS";break;
        case A_oper::timesOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> TIMES |<f2> \"];\n",t);std::cout<<"TIMES";break;
        case A_oper::divideOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> DIVIDE |<f2> \"];\n",t);std::cout<<"DIVIDE";break;
        case A_oper::ltOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> LT |<f2> \"];\n",t);std::cout<<"LT";break;
        case A_oper::leOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> LE |<f2> \"];\n",t);std::cout<<"LE";break;
        case A_oper::gtOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> GT |<f2> \"];\n",t);std::cout<<"GT";break;
        case A_oper::geOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> GE |<f2> \"];\n",t);std::cout<<"GE";break;
        case A_oper::eqOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> EQ |<f2> \"];\n",t);std::cout<<"EQ";break;
        case A_oper::neqOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> NEQ |<f2> \"];\n",t);std::cout<<"NEQ";break;
        case A_oper::andOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> AND |<f2> \"];\n",t);std::cout<<"AND";break;
        case A_oper::orOp:
            fprintf(fp,"a%d[label = \"<f0> | <f1> OR |<f2> \"];\n",t);std::cout<<"OR";break; 
    }
    fprintf(fp,"a%d:f0:sw -> a%d:f1;\n",num, t);
    std::cout<<",";
    if(left!=nullptr){
        p = left->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, p);
    }
    std::cout<<",";
    if(right!=nullptr){  
        p = right->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1;\n",num, p);
    }
    std::cout<<")";
    return num;
}

int A_recordExp::printast(){
    int num,t,p;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> recordExp |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,type_id.c_str());
    fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = TYPE_ID];\n",num, t);
    std::cout<<"A_recordExp(TYPE("<<type_id<<"),";
    if(fields!=nullptr){
        p = fields->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1;\n",num, p);
    }
    std::cout<<")";
    return num;
}

int A_arrayExp::printast(){
    int num,t,p;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> arrayExp |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,type_id.c_str());
    fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = TYPE_ID];\n",num, t);
    std::cout<<"A_arrayExp(TYPE("<<type_id<<"),SIZE(";
    if(size!=nullptr){
        p = size->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1 [label = SIZE];\n",num, p);
    }
    std::cout<<"),INIT(";
    if(init!=nullptr){   
        p = init->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1 [label = INIT];\n",num, p);
    }
    std::cout<<"))";
    return num;
}

int A_ifExp::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> ifExp |<f2> \"];\n",num);
    std::cout<<"A_ifExp(IF(";
    if(test!=nullptr){
        t = test->printast();
        fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = IF];\n",num, t);
    }
    std::cout<<"),THEN(";
    if(tbody!=nullptr){
        t = tbody->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1 [label = THEN];\n",num, t);
    }
    std::cout<<"),ELSE(";
    if(fbody!=nullptr){
        t = fbody->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1 [label = ELSE];\n",num, t);
    }
    std::cout<<"))";
    return num;
}

int A_whileExp::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> whileExp |<f2> \"];\n",num);
    std::cout<<"A_whileExp(WHILE(";
    if(test!=nullptr){
        t = test->printast();
        fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = WHILE];\n",num, t);
    }
    std::cout<<"),DO(";
    if(body!=nullptr){
        t = body->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1 [label = DO];\n",num, t);
    }
    std::cout<<"))";
    return num;
}

int A_forExp::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> forExp |<f2> \"];\n",num);
    std::cout<<"A_forExp(LOW(";
    if(low!=nullptr){
        t = low->printast();
        fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = LOW];\n",num, t);
    }
    std::cout<<"),HIGH(";
    if(high!=nullptr){
        t = high->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1 [label = HIGH];\n",num, t);
    }
    std::cout<<"),BODY(";
    if(body!=nullptr){   
        t = body->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1 [label = BODY];\n",num, t);
    }
    std::cout<<"))";
    return num;
}

int A_breakExp::printast(){
    int num;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> breakExp |<f2> \"];\n",num);
    std::cout<<"A_breakExp()";
    return num;
}

int A_letExp::printast(){
    int num, t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> letExp |<f2> \"];\n",num);
    std::cout<<"A_letExp(DECS(";
    if(decs!=nullptr){
        t = decs->printast();
        fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = DECS];\n",num, t);
    }
    std::cout<<"),BODY(";
    if(body!=nullptr){
        t = body->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1 [label = BODY];\n",num, t);
    }
    std::cout<<"))";
    return num;
}

int A_varDec::printast(){
    int num,t,p,q;
    num = cc;
    t = cc+1;
    p = cc+2;
    cc+=3;
    fprintf(fp,"a%d[label = \"<f0> | <f1> varDec |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,id.c_str());
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",p,type_id.c_str());
    fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = ID];\n",num, t);
    fprintf(fp,"a%d:f1:s -> a%d:f1 [label = TYPE_ID];\n",num, p);
    std::cout<<"A_varDec(ID("<<id<<"),TYPE("<<type_id<<"),INIT(";
    if(init!=nullptr){   
        q = init->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1 [label = INIT];\n",num, q);
    }
    std::cout<<"))";
    return num;
}

int A_typeDec::printast(){
    int num,t,p;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> typeDec |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,type_id.c_str());  
    fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = TYPE_ID];\n",num, t);
    std::cout<<"A_typeDec(ID("<<type_id<<"),TYPE(";
    if(ty!=nullptr){
        p = ty->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1 [label = TYPE];\n",num, p);
    }
    std::cout<<"))";
    return num;
}

int A_functionDec::printast(){
    int num,t,p,q;
    num = cc;
    t = cc+1;
    p = cc+2;
    cc+=3;
    fprintf(fp,"a%d[label = \"<f0> | <f1> functionDec |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,id.c_str());
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",p,type_id.c_str());
    fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = ID];\n",num, t);
    fprintf(fp,"a%d:f1:s -> a%d:f1 [label = TYPE_ID];\n",num, p);
    std::cout<<"A_functionDec(ID("<<id<<"),TYPE("<<type_id<<"),PARAM(";
    if(params!=nullptr){
        q = params->printast();
        fprintf(fp,"a%d:f1:sw -> a%d:f1 [label = PARAM];\n",num, q);
    }
    std::cout<<"),BODY(";
    if(body!=nullptr){
        q = body->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1 [label = BODY];\n",num, q);
    }
    std::cout<<"))";
    return num;
}

int A_nameTy::printast(){
    int num,t;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> nameTy |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> id:%s |<f2> \"];\n",t,id.c_str());
    fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
    std::cout<<"A_nameTy("<<id<<")";
    return num;
}

int A_recordTy::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> nameTy |<f2> \"];\n",num);
    std::cout<<"A_recordTy(";
    if(record!=nullptr){
        t = record->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
    }
    std::cout<<")";
    return num;
}

int A_arrayTy::printast(){
    int num,t;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> arrayTy |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,id.c_str());
    fprintf(fp,"a%d:f1:s -> a%d:f1 [label = ID];\n",num, t);
    std::cout<<"A_arrayTy("<<id<<")";
    return num;
}

int A_field::printast(){
    int num,t,p;
    num = cc;
    t = cc+1;
    p = cc+2;
    cc+=3;
    fprintf(fp,"a%d[label = \"<f0> | <f1> field |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,id.c_str());
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",p,type_id.c_str());
    fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = ID];\n",num, t);
    fprintf(fp,"a%d:f2:se -> a%d:f1 [label = TYPE];\n",num, p);
    std::cout<<"A_field(ID("<<id<<"),TYPE("<<type_id<<"))";
    return num;
}

int A_fieldList::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> fieldList |<f2> \"];\n",num);
    std::cout<<"A_fieldList(";
    t = head->printast();
    fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
    A_fieldList* tmp = tail.get();
    while(tmp!=nullptr){
        std::cout<<",";
        t = tmp->head->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
        tmp = tmp->tail.get();
    }
    std::cout<<")";
    return num;
}

int A_expList::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> expList |<f2> \"];\n",num);
    std::cout<<"A_expList(";
    t = head->printast();
    fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
    A_expList* tmp = tail.get();
    while(tmp!=nullptr){
        std::cout<<",";
        t = tmp->head->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
        tmp = tmp->tail.get();
    }
    std::cout<<")";
    return num;
}

int A_decList::printast(){
    int num ,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> decList |<f2> \"];\n",num);
    std::cout<<"A_decList(";
    t = head->printast();
    fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
    A_decList* tmp = tail.get();
    while(tmp!=nullptr){
        std::cout<<",";
        t = tmp->head->printast();
        fprintf(fp,"a%d:f1:s -> a%d:f1;\n",num, t);
        tmp = tmp->tail.get();
    }
    std::cout<<")";
    return num;
}

int A_efield::printast(){
    int num,t,p;
    num = cc;
    t = cc+1;
    cc+=2;
    fprintf(fp,"a%d[label = \"<f0> | <f1> efield |<f2> \"];\n",num);
    fprintf(fp,"a%d[label = \"<f0> | <f1> %s |<f2> \"];\n",t,id.c_str());
    fprintf(fp,"a%d:f0:sw -> a%d:f1 [label = ID];\n",num, t);
    std::cout<<"A_efield(ID("<<id<<"),";
    if(exp!=nullptr){
        p = exp->printast();
        fprintf(fp,"a%d:f2:se -> a%d:f1;\n",num, p);
    }
    std::cout<<")";
    return num;
}

int A_efieldList::printast(){
    int num,t;
    num = cc;
    cc++;
    fprintf(fp,"a%d[label = \"<f0> | <f1> efieldList |<f2> \"];\n",num);
    std::cout<<"A_efieldList(";
    t = head->printast();
    fprintf(fp,"a%d:f0:s -> a%d:f1;\n",num, t);
    A_efieldList* tmp = tail.get();
    while(tmp!=nullptr){
        std::cout<<",";
        t = tmp->head->printast();
        fprintf(fp,"a%d:f2:s -> a%d:f1;\n",num, t);
        tmp = tmp->tail.get();
    }
    std::cout<<")";
    return num;
}

#endif