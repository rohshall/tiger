#include "ir_tree.h"
#include <iostream>

void print(std::string s){
    std::cout << s;
}

void printRelOp(T_relOp op){
    switch(op){
        case T_relOp::T_eq : print("EQ"); break;
        case T_relOp::T_ge : print("GE"); break;
        case T_relOp::T_gt : print("GT"); break;
        case T_relOp::T_le : print("LE"); break;
        case T_relOp::T_lt : print("LT"); break;
        case T_relOp::T_ne : print("NE"); break;
    }
}

void printBinOp(T_binOp op){
    switch(op){
        case T_binOp::T_plus : print("PLUS"); break;
        case T_binOp::T_minus : print("MINUS"); break;
        case T_binOp::T_mul : print("MUL"); break;
        case T_binOp::T_div : print("DIV"); break;
    }
}

void T_stm::printirt(){
    switch(stmKind){
        case T_stm::StmKind::T_cjump :{
            T_cjump* newptr = dynamic_cast<T_cjump *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_stm::StmKind::T_jump :{
            T_jump* newptr = dynamic_cast<T_jump *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_stm::StmKind::T_label :{
            T_label* newptr = NULL;dynamic_cast<T_label *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_stm::StmKind::T_move :{
            T_move* newptr = dynamic_cast<T_move *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_stm::StmKind::T_seq :{
            T_seq* newptr = dynamic_cast<T_seq *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_stm::StmKind::T_stm_exp :{
            T_stm_exp* newptr = dynamic_cast<T_stm_exp *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
    }
}

void T_seq::printirt(){
    print("T_SEQ(");
    if(left != nullptr)
        left->printirt();
    else
        print("NULL");
    print(",");
    if(right != nullptr)
        right->printirt();
    else
        print("NULL");
    print(")");        
}

void T_label::printirt(){
    print("T_LABEL(" + id + ")");
}

void T_jump::printirt(){
    print("T_JUMP(");
    if(exp != nullptr)
        exp->printirt();
    else
        print("NULL");
    print(",");
    label->printirt();
    print(")");
}

void T_cjump::printirt(){
    print("T_CJUMP(");
    printRelOp(op);
    print(",");
    if(left != NULL)
        left->printirt();
    else
        print("NULL");
    print(",");
    if(right != NULL)
        right->printirt();
    else
        print("NULL");
    print(",");
    if(label_true != NULL)
        label_true->printirt();
    else
        print("NULL");
    print(",");
    if(label_false != NULL)
        label_false->printirt();
    else
        print("NULL");
    print(")");
}

void T_move::printirt(){
    print("T_MOVE(");
    if(dst != NULL)
        dst->printirt();
    else
        print("NULL");
    print(",");
    if(src != NULL)
        src->printirt();
    else
        print("NULL");
    print(")");
}

void T_stm_exp::printirt(){
    exp->printirt();
}

void T_exp::printirt(){
    switch(expKind){
        case T_exp::ExpKind::T_binop :{
            T_binop* newptr = dynamic_cast<T_binop *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_exp::ExpKind::T_call :{
            T_call* newptr = dynamic_cast<T_call *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_exp::ExpKind::T_const :{
            T_const* newptr = dynamic_cast<T_const *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_exp::ExpKind::T_eseq :{
            T_eseq* newptr = dynamic_cast<T_eseq *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_exp::ExpKind::T_mem :{
            T_mem* newptr = dynamic_cast<T_mem *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_exp::ExpKind::T_name :{
            T_name* newptr = dynamic_cast<T_name *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
        case T_exp::ExpKind::T_temp :{
            T_temp* newptr = dynamic_cast<T_temp *>(this);
            if(newptr != NULL)
                newptr->printirt();
            break;
        }
    }
}

void T_binop::printirt(){
    print("T_BINOP(");
    printBinOp(op);
    print(",");
    left->printirt();
    print(",");
    right->printirt();
    print(")");
}

void T_mem::printirt(){
    print("T_MEM(");
    exp->printirt();
    print(")");
}

void T_temp::printirt(){
    print("T_TEMP(" + id + ")");
}

void T_eseq::printirt(){
    print("T_ESEQ(");
    if(stm != nullptr)
        stm->printirt();
    else
        print("NULL");
    print(",");
    if(exp != nullptr)
        exp->printirt();
    else
        print("NULL");
    print(")");
}

void T_name::printirt(){
    print("T_NAME(" + id + ")");
}

void T_const::printirt(){
    print("T_CONST(" + std::to_string(val) + ")");
}

void T_call::printirt(){
    print("T_CALL(");
    funcName->printirt();
    print(",");
    if(args != nullptr)
        args->printirt();
    else
        print("NULL");
    print(")");
}

void T_exp_stm::printirt(){
    stm->printirt();
} 

void T_expList::check(){
    if(tail != nullptr)
        tail->check();
    
    if(tail == nullptr)
        return;
    
    if(tail->head == nullptr && tail->tail == nullptr)
        tail.reset(nullptr);
}

void T_expList::printirt(){
    print("T_EXPLIST(");
    if(head != nullptr)
        head->printirt();
    else
        print("NULL");
    print(",");
    if(tail != nullptr)
        tail->printirt();
    else
        print("NULL");
    print(")");
}

void T_stmList::check(){
    if(tail != nullptr)
        tail->check();
    
    if(tail == nullptr)
        return;

    if(tail->head == nullptr && tail->tail == nullptr)
        tail.reset(nullptr);    // auto delete, release do not delete the pointer
}

void T_stmList::printirt(){
    print("T_STMLIST(");
    if(head != nullptr)
        head->printirt();
    else
        print("NULL");
    print(",");
    if(tail != nullptr)
        tail->printirt();
    else
        print("NULL");
    print(")");
}
