#include "ir_tree.h"
#include <iostream>
#include <fstream>

static std::ofstream outf;
static int cc = 0;
static bool opened = false;

void openFile(){
    outf.open("irt.dot", std::ios::out);
    std::cout << "Open File And output irt." << std::endl;
    if(!outf.is_open()){
        std::cout << "open file irt.dot failed" << std::endl;
        return;
    }
    opened = true;
    outf << "digraph irt {\nnode[shape = record];\n";
}

void closeFile(){
    outf << "}\n";
    outf.close();
    std::cout << "Close File" << std::endl;
    opened = false;
}

void print(int num, std::string nodeName){
//    std::cout << "ir" << num << " [ label = \" "<< nodeName << " \"];\n ";
    outf << "ir" << num << " [ label = \" "<< nodeName << " \"];\n ";
}

void print(int num, int child){
    if(child != -1)
//    std::cout << "ir" << num << " -> " << "ir" << child << ";\n";
        outf << "ir" << num << " -> " << "ir" << child << ";\n";
}

int printSingle(std::string s){
    int num = cc;
    cc++;
    print(num, s);
    return num;
}

int T_stm::printirtDot(){
    switch(stmKind){
        case T_stm::StmKind::T_cjump :{
            T_cjump* newptr = dynamic_cast<T_cjump *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_stm::StmKind::T_jump :{
            T_jump* newptr = dynamic_cast<T_jump *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_stm::StmKind::T_label :{
            T_label* newptr = NULL;dynamic_cast<T_label *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_stm::StmKind::T_move :{
            T_move* newptr = dynamic_cast<T_move *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_stm::StmKind::T_seq :{
            T_seq* newptr = dynamic_cast<T_seq *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_stm::StmKind::T_stm_exp :{
            T_stm_exp* newptr = dynamic_cast<T_stm_exp *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
    }
}

int printRelOpDot(T_relOp op){
    switch(op){
        case T_relOp::T_eq : return printSingle("EQ");
        case T_relOp::T_ge : return printSingle("GE");
        case T_relOp::T_gt : return printSingle("GT");
        case T_relOp::T_le : return printSingle("LE");
        case T_relOp::T_lt : return printSingle("LT");
        case T_relOp::T_ne : return printSingle("NE");
        default : return -1;
    }
}

int printBinOpDot(T_binOp op){
    switch(op){
        case T_binOp::T_plus : return printSingle("PLUS");
        case T_binOp::T_minus : return printSingle("MINUS");
        case T_binOp::T_mul : return printSingle("MUL");
        case T_binOp::T_div : return printSingle("DIV");
        case T_binOp::T_and : return printSingle("AND");
        case T_binOp::T_or : return printSingle("OR");
        case T_binOp::T_xor : return printSingle("XOR");
        default : return -1;
    }
}

int T_seq::printirtDot(){

    if(left != nullptr){
        int num = cc, ret;
        cc++;

        print(num, "T_SEQ");
        
        ret = left->printirtDot();
        print(num, ret);

        if(right != nullptr){
            ret = right->printirtDot();
            print(num, ret);
        }
        return num;
    }
    else if(left == nullptr && right != nullptr)
        return right->printirtDot();
    else
        return -1;
}

int T_label::printirtDot(){
    return printSingle("T_LABEL(" + id + ")");
}

int T_jump::printirtDot(){
    int num = cc, ret;
    cc++;
    print(num, "T_JUMP");
    if(exp != nullptr){
        ret = exp->printirtDot();
        print(num, ret);
    }

    ret = label->printirtDot();
    print(num, ret);
    return num;
}

int T_cjump::printirtDot(){
    int num = cc, ret;
    cc++;
    print(num, "T_CJUMP");
    ret = printRelOpDot(op);
    print(num, ret);
    if(left != NULL)
        ret = left->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    if(right != NULL)
        ret = right->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    if(label_true != NULL)
        ret = label_true->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    if(label_false != NULL)
        ret = label_false->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    return num;
}

int T_move::printirtDot(){
    int num = cc, ret;
    cc++;
    print(num, "T_MOVE");
    if(dst != NULL)
        ret = dst->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    if(src != NULL)
        ret = src->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    return num;
}

int T_stm_exp::printirtDot(){
    return exp->printirtDot();
}

int T_exp::printirtDot(){
    switch(expKind){
        case T_exp::ExpKind::T_binop :{
            T_binop* newptr = dynamic_cast<T_binop *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_exp::ExpKind::T_call :{
            T_call* newptr = dynamic_cast<T_call *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_exp::ExpKind::T_const :{
            T_const* newptr = dynamic_cast<T_const *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_exp::ExpKind::T_eseq :{
            T_eseq* newptr = dynamic_cast<T_eseq *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_exp::ExpKind::T_mem :{
            T_mem* newptr = dynamic_cast<T_mem *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_exp::ExpKind::T_name :{
            T_name* newptr = dynamic_cast<T_name *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
        case T_exp::ExpKind::T_temp :{
            T_temp* newptr = dynamic_cast<T_temp *>(this);
            if(newptr != NULL)
                return newptr->printirtDot();
        }
    }
}

int T_binop::printirtDot(){
    int num = cc, ret;
    cc++;
    print(num, "T_BINOP");
    ret = printBinOpDot(op);
    print(num, ret);
    if(left != nullptr)
        ret = left->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    if(right != nullptr)
        ret = right->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    return num;
}

int T_mem::printirtDot(){
    int num = cc, ret;
    cc++;
    print(num, "T_MEM");
    if(exp != nullptr)
        ret = exp->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    return num;
}

int T_temp::printirtDot(){
    return printSingle("T_TEMP(" + id + ")");
}

int T_eseq::printirtDot(){
    if(stm != nullptr){
        int num = cc, ret;
        cc++;
        print(num, "T_ESEQ");

        ret = stm->printirtDot();
        print(num, ret);
        if(exp != nullptr)
            ret = exp->printirtDot();
        else
            ret = printSingle("NULL");
        print(num, ret);

        return num;
    }
    else if(stm == nullptr && exp != nullptr)
        return exp->printirtDot();
    else
        return -1;
}

int T_name::printirtDot(){
    return printSingle("T_NAME(" + id + ")");
}

int T_const::printirtDot(){
    return printSingle("T_CONST(" + std::to_string(val) + ")");
}

int T_call::printirtDot(){
    int num = cc, ret;
    cc++;
    print(num, "T_CALL");
    ret = funcName->printirtDot();
    print(num, ret);
    if(args != nullptr)
        ret = args->printirtDot();
    else
        ret = printSingle("NULL");
    print(num, ret);
    return num;
}

int T_exp_stm::printirtDot(){
    return stm->printirtDot();
} 

void T_expList::check(){
    if(tail != nullptr)
        tail->check();
    
    if(tail == nullptr)
        return;

    if(tail->head == nullptr && tail->tail == nullptr)
        tail.reset(nullptr);
    
    // if (tail != nullptr && tail->head == nullptr)
    //     tail.reset(tail->tail.get());    
}

int T_expList::printirtDot(){
    if(head != nullptr){
        int num = cc, ret;
        cc++;
        print(num, "T_ESEQ");
        
        ret = head->printirtDot();
        print(num, ret);
        
        if(tail != nullptr){
            ret = tail->printirtDot();
            print(num, ret);
        }
        return num;
    }
    else if(head == nullptr && tail != nullptr)
        return tail->printirtDot();
    else
        return -1;
}

void T_stmList::check(){
    if(tail != nullptr && tail->head != nullptr)
        tail->check();

    if(tail == nullptr)
        return;

    if(tail->head == nullptr && tail->tail == nullptr)
        tail.reset(nullptr);    // auto delete, release do not delete the pointer
    
    // if (tail != nullptr && tail->head == nullptr)
    //     tail.reset(tail->tail.get());
}

int T_stmList::printirtDot(){
    if(head != nullptr){
            int num = cc, ret;
        cc++;
        print(num, "T_SEQ");

        ret = head->printirtDot();
        print(num, ret);

        if(tail != nullptr){
            ret = tail->printirtDot();
            print(num, ret);
        }
        return num;
    }
    else if(head == nullptr && tail != nullptr)
        return tail->printirtDot();
    else
        return -1;
}