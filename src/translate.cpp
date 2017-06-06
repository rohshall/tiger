<<<<<<< HEAD
#include "translate.h"
const int wordsize = 4;
FrameControl fcc;

T_stm *IRNode2T_stm(IRNode *root){
    if(root == NULL)
        return NULL;
    if(root->nodeKind == IRNode::NodeKind::T_stm){
        T_stm *newptr = dynamic_cast<T_stm *>(root);        
        if(newptr == NULL)
            std::cout << "IRNode2T_stm Wrong!\n";
        return newptr;
    }
    else{
        T_stm_exp *newptr = new T_stm_exp(IRnode2T_exp(root)); 
        if(newptr == NULL)
            std::cout << "IRNode2T_stm Wrong!\n";
        return newptr;
    }

}

T_exp *IRnode2T_exp(IRNode *root){
    if(root == NULL)
        return NULL;
    if(root->nodeKind == IRNode::NodeKind::T_exp){
        T_exp *newptr = dynamic_cast<T_exp *>(root);        
        if(newptr == NULL)
            std::cout << "IRNode2T_stm Wrong!\n";
        return newptr;
    }
    else{
        T_exp_stm *newptr = new T_exp_stm(IRNode2T_stm(root)); 
        if(newptr == NULL)
            std::cout << "IRNode2T_stm Wrong!\n";
        return newptr;
    }
}

T_expList* IRNode2T_expList(IRNode *root){
    T_expList *newptr = dynamic_cast<T_expList *>(
        IRnode2T_exp(root)
    );
    if(newptr != NULL)
        return newptr;
    else{
        std::cout << "IRNode2expList Wrong!\n";
        return NULL;
    }
}

T_binOp getBinOperator(A_oper op){
    switch(op){
        case A_oper::plusOp: return T_binOp::T_plus;
        case A_oper::minusOp: return T_binOp::T_minus;
        case A_oper::timesOp: return T_binOp::T_mul;
        case A_oper::divideOp: return T_binOp::T_div;
    }
}

T_relOp getRelOperator(A_oper op){
    switch(op){
        case A_oper::eqOp: return T_relOp::T_eq;
        case A_oper::neqOp: return T_relOp::T_ne;
        case A_oper::ltOp: return T_relOp::T_lt;
        case A_oper::leOp: return T_relOp::T_le;
        case A_oper::gtOp: return T_relOp::T_gt;
        case A_oper::geOp: return T_relOp::T_ge;
    }
}

IRNode* A_Program::translate(){
    fcc.openFrame();
    IRNode* root = NULL;
    if(exp!=nullptr)
        root = exp->translate();
    fcc.closeFrame();
    return root;
}

IRNode* A_simpleVar::translate(){
    T_mem *root = fcc.MEM(id, -1, NULL);
    if(root == NULL){
        root = new T_mem(
            new T_binop(
                T_binOp::T_plus, new T_temp("fp"),new T_const(0)
            )
        );
    }
    return root;
}

IRNode* A_fieldVar::translate(){
    if(var == nullptr)
        return NULL;
    T_mem *root = new T_mem(
        new T_binop{
            T_binOp::T_plus,
            IRnode2T_exp(var->translate()),  /*field base*/
            new T_binop(
                T_binOp::T_mul,
                new T_const(this->offset - 1), /*field offset*/
                new T_const(wordsize)  /*word size*/
            )
        }
    );
    return root;
}
 
IRNode* A_subscriptVar::translate(){
    if(var == nullptr || exp == nullptr)
        return NULL;
    T_mem *root = new T_mem(
        new T_binop{
            T_binOp::T_plus,
            new T_temp("1"),  /*array base*/
            new T_binop(
                T_binOp::T_mul,
                new T_eseq(
                    nullptr, 
                    IRnode2T_exp(exp->translate())
                ),
                new T_const(wordsize)  /*word size*/
            )
        }
    );
    return root;
}

IRNode* A_varExp::translate(){
    if(var == nullptr)
        return NULL; 
    IRNode* tmp = var->translate();
    T_eseq *root = new T_eseq{
        NULL,
        IRnode2T_exp(var->translate())
    };
    return root;
}

IRNode* A_callExp::translate(){
    T_call* root;
    if(args == nullptr)
        root = new T_call(new T_name(id), NULL);
    else
        root = new T_call(
            new T_name(id),
            IRNode2T_expList(args->translate())
        );
    return root;
}

IRNode* A_assignExp::translate(){
    if(var == nullptr || exp == nullptr)
        return NULL;
    T_move* root = new T_move(
        IRnode2T_exp(var->translate()),
        IRnode2T_exp(exp->translate())
    );
    return root;
}

IRNode* A_nilExp::translate(){
    return NULL;
}

IRNode* A_seqExp::translate(){
    if(seq == nullptr)
        return NULL;
    T_eseq* root = new T_eseq(
        NULL,
        IRnode2T_exp(seq->translate())
    );
    return root;
}

IRNode* A_intExp::translate(){
    T_const* root = new T_const(val);
    return root;
}

IRNode* A_stringExp::translate(){
    T_mem* root = new T_mem(
        new T_temp(val) /*从栈帧获取string对应的地址*/
    );
    return root;
}

IRNode* A_opExp::translate(){
    if(left == nullptr || right == nullptr)
        return NULL;
    if(op >= A_oper::plusOp && op <= A_oper::divideOp){
        T_binOp moperator = getBinOperator(op); 
        T_binop* root = new T_binop(
            moperator,
            IRnode2T_exp(left->translate()),
            IRnode2T_exp(right->translate())
        );
        return root;
    }
    else if (op >=A_oper::eqOp && op <= A_oper::geOp){
        T_relOp moperator = getRelOperator(op);
        T_cjump* root = new T_cjump(
            moperator,
            IRnode2T_exp(left->translate()),
            IRnode2T_exp(right->translate()),
            new T_label("t"),
            new T_label("f")
        );
        return root;
    }
    else{
        //and 和 or 单独进行赋值处理
    }
}
int recordCount = 0;
IRNode* A_recordExp::translate(){
    recordCount = 0;
    T_stmList* varRoot = new T_stmList(NULL, NULL);
    T_stmList* varCurList = varRoot;
    A_efieldList* cur_var = fields.get();

    while (cur_var != nullptr) {
        recordCount ++;
        T_stm* cur_vart = IRNode2T_stm(cur_var->head->translate());
        if(cur_vart == NULL)
            std::cout << "Record wrong!" << std::endl;
        varCurList->head.reset(cur_vart);
        varCurList->tail.reset(new T_stmList(NULL, NULL));
        varCurList = varCurList->tail.get();
        cur_var = cur_var->tail.get();
    }
    T_eseq* root = new T_eseq(
        new T_seq(
            new T_move(
                new T_temp("r"),
                new T_call(
                    new T_name("malloc"),
                    new T_expList(
                        new T_const(recordCount * wordsize),
                        NULL
                    )
                )
            ),
            varRoot
        ),
        new T_temp("r") //获取新的temp表达式
    );
    return root;
}

IRNode* A_arrayExp::translate(){
    if(size == nullptr || init == nullptr)
        return NULL;
    T_call* root = new T_call(
        new T_name("initArray"),
        new T_expList(
            IRnode2T_exp(size->translate()),
            new T_expList(
                IRnode2T_exp(init->translate()),
                NULL
            )
        )
    );
    return root;
}
//这里暂定二值跳转都是无值表达式
IRNode* A_ifExp::translate(){
    if(fbody == nullptr){
        T_seq* root = new T_seq(
            new T_seq(
                IRNode2T_stm(test->translate()),
                new T_seq(
                    new T_label("t"),
                    IRNode2T_stm(tbody->translate())
                )
            ),
            new T_label("f")
        );
        return root;
    }
    else{
        T_move* ttbody = new T_move(
            new T_temp("t"),
            IRnode2T_exp(tbody->translate())
        );
        T_move* ffbody = new T_move(
            new T_temp("t"),
            IRnode2T_exp(fbody->translate())
        );
        T_eseq* root = new T_eseq(
            new T_seq(
                IRNode2T_stm(test->translate()),
                new T_seq(
                    new T_label("t"),
                    new T_seq(
                        ttbody,
                        new T_seq(
                            new T_jump(NULL, new T_label("e")),
                            new T_seq(
                                new T_label("f"),
                                ffbody
                            )
                        )
                    )
                )
            ),
            new T_eseq(
                new T_label("e"),
                new T_temp("t")
            )
        );
        return root;
    }
}

IRNode* A_whileExp::translate(){
    if(test == nullptr || body == nullptr)
        return NULL;
    T_seq* root = new T_seq(
        new T_seq(
            new T_seq(
                new T_label("test"),
                IRNode2T_stm(test->translate())
            ),
            new T_seq(
                IRNode2T_stm(body->translate()),
                new T_jump(NULL, new T_label("done"))
            )
        ),
        new T_label("done")
    );
    return root;
}

IRNode* A_forExp::translate(){
    if(low == nullptr || high == nullptr || body == nullptr)
        return NULL;
    T_seq* root = 
    new T_seq(
        new T_seq(
            new T_seq(
                new T_move(
                    new T_temp(id),
                    IRnode2T_exp(low->translate())
                ),
                new T_move(
                    new T_temp("limit"),
                    IRnode2T_exp(high->translate())
                )
            ),
            new T_seq(
                new T_seq(
                    new T_label("test"),
                    new T_cjump(
                        T_relOp::T_le,
                        new T_temp(id),
                        new T_temp("limit"),
                        new T_label("t"),
                        new T_label("done")
                    )
                ),
                new T_seq(
                    new T_label("t"),
                    new T_seq(
                        IRNode2T_stm(body->translate()),
                        new T_jump(NULL, new T_label("test"))    
                    )
                )
            )
        ),
        new T_label("done")
    );
    return root;
}

IRNode* A_breakExp::translate(){
    T_jump* root = new T_jump(NULL, new T_label("done"));
    return root;
}

IRNode* A_letExp::translate(){
    if(decs == nullptr || body == nullptr)
        return NULL;
    

    T_stmList* decRoot = new T_stmList(NULL, NULL);
    T_stmList* decCurList = decRoot;
    A_decList* cur_dec = decs.get();

    while (cur_dec != nullptr) {
        T_stm* cur_dect = IRNode2T_stm(cur_dec->head->translate());
        // if(cur_dect == NULL)
        //     std::cout << "Let wrong!" << std::endl;
        decCurList->head.reset(cur_dect);
        decCurList->tail.reset(new T_stmList(NULL, NULL));
        decCurList = decCurList->tail.get();
        cur_dec = cur_dec->tail.get();
    }

    T_expList* expRoot = new T_expList(NULL, NULL);
    T_expList* expCurList = expRoot;
    A_expList* cur_exp = body.get();
    while (cur_exp != nullptr) {
        T_exp* cur_expt = IRnode2T_exp(cur_exp->head->translate());
        // if(cur_expt == NULL)
        //     std::cout << "Let wrong!" << std::endl;      
        expCurList->head.reset(cur_expt);
        expCurList->tail.reset(new T_expList(NULL, NULL));
        expCurList = expCurList->tail.get();
        cur_exp = cur_exp->tail.get();
    }

    T_eseq* root = new T_eseq(
        decRoot,
        expRoot
    );

    return root;
}

IRNode* A_varDec::translate(){
    if(init == nullptr)
        return NULL;
    std::cout << "varDec Start" << std::endl;
    fcc.addLocalVar(id);
    T_move* root = new T_move(
        fcc.MEM(id, -1, NULL),
        IRnode2T_exp(init->translate())
    );
    std::cout << "varDec Finsh" << std::endl;
    return root;
}

IRNode* A_typeDec::translate(){
    return NULL;
}

IRNode* A_functionDec::translate(){
    if(body == nullptr)
        return NULL;
    fcc.openFrame();
    // 应该进入新的环境以及保护变量
    T_eseq* root = new T_eseq(
        new T_seq(
            NULL, IRNode2T_stm(new T_name(id))
        ),
        IRnode2T_exp(body->translate())
    );
    fcc.closeFrame();
    return root;
}

IRNode* A_nameTy::translate(){
    return NULL;
}

IRNode* A_recordTy::translate(){
    return NULL;
}

IRNode* A_arrayTy::translate(){
    return NULL;
}

IRNode* A_expList::translate(){
    if(head == nullptr)
        return NULL;
    
    T_expList* expRoot = new T_expList(NULL, NULL);
    T_expList* expCurList = expRoot;
    A_expList* cur_exp = this;
    while (cur_exp != nullptr) {
        T_exp* cur_expt = IRnode2T_exp(cur_exp->head->translate());
        if(cur_expt == NULL)
            std::cout << "Let wrong!" << std::endl;
        
        expCurList->head.reset(cur_expt);
        expCurList->tail.reset(new T_expList(NULL, NULL));
        expCurList = expCurList->tail.get();
        cur_exp = cur_exp->tail.get();
    }

    return expRoot;
}

IRNode* A_decList::translate(){
    return NULL;
}

IRNode* A_efield::translate(){
    if(offset == -1)
        offset = recordCount;
    T_move* root = new T_move(
        new T_mem(
            new T_binop(
                T_binOp::T_plus,
                new T_temp("t"),
                new T_const((offset - 1) * wordsize)
            )
        ),
        IRnode2T_exp(exp->translate())
    );
    return root;
}

=======
#include "translate.h"
const int wordsize = 4;
FrameControl fcc;

T_stm *IRNode2T_stm(IRNode *root){
    if(root == NULL)
        return NULL;
    if(root->nodeKind == IRNode::NodeKind::T_stm){
        T_stm *newptr = dynamic_cast<T_stm *>(root);        
        if(newptr == NULL)
            std::cout << "IRNode2T_stm Wrong!\n";
        return newptr;
    }
    else{
        T_stm_exp *newptr = new T_stm_exp(IRnode2T_exp(root)); 
        if(newptr == NULL)
            std::cout << "IRNode2T_stm Wrong!\n";
        return newptr;
    }

}

T_exp *IRnode2T_exp(IRNode *root){
    if(root == NULL)
        return NULL;
    if(root->nodeKind == IRNode::NodeKind::T_exp){
        T_exp *newptr = dynamic_cast<T_exp *>(root);        
        if(newptr == NULL)
            std::cout << "IRNode2T_stm Wrong!\n";
        return newptr;
    }
    else{
        T_exp_stm *newptr = new T_exp_stm(IRNode2T_stm(root)); 
        if(newptr == NULL)
            std::cout << "IRNode2T_stm Wrong!\n";
        return newptr;
    }
}

T_expList* IRNode2T_expList(IRNode *root){
    T_expList *newptr = dynamic_cast<T_expList *>(
        IRnode2T_exp(root)
    );
    if(newptr != NULL)
        return newptr;
    else{
        std::cout << "IRNode2expList Wrong!\n";
        return NULL;
    }
}

T_binOp getBinOperator(A_oper op){
    switch(op){
        case A_oper::plusOp: return T_binOp::T_plus;
        case A_oper::minusOp: return T_binOp::T_minus;
        case A_oper::timesOp: return T_binOp::T_mul;
        case A_oper::divideOp: return T_binOp::T_div;
    }
}

T_relOp getRelOperator(A_oper op){
    switch(op){
        case A_oper::eqOp: return T_relOp::T_eq;
        case A_oper::neqOp: return T_relOp::T_ne;
        case A_oper::ltOp: return T_relOp::T_lt;
        case A_oper::leOp: return T_relOp::T_le;
        case A_oper::gtOp: return T_relOp::T_gt;
        case A_oper::geOp: return T_relOp::T_ge;
    }
}

IRNode* A_Program::translate(){
    fcc.openFrame();
    IRNode* root = NULL;
    if(exp!=nullptr)
        root = exp->translate();
    fcc.closeFrame();
    return root;
}

IRNode* A_simpleVar::translate(){
    T_mem *root = fcc.MEM(id, -1, NULL);
    // T_mem *root = new T_mem(
    //     new T_binop(
    //         T_binOp::T_plus, new T_temp(id),new T_const(1)
    //         // have to check the temp
    //     )
    // );
    return root;
}

IRNode* A_fieldVar::translate(){
    if(var == nullptr)
        return NULL;
    T_mem *root = new T_mem(
        new T_binop{
            T_binOp::T_plus,
            IRnode2T_exp(var->translate()),  /*field base*/
            new T_binop(
                T_binOp::T_mul,
                new T_const(this->offset - 1), /*field offset*/
                new T_const(wordsize)  /*word size*/
            )
        }
    );
    return root;
}

IRNode* A_subscriptVar::translate(){
    if(var == nullptr || exp == nullptr)
        return NULL;
    T_mem *root = new T_mem(
        new T_binop{
            T_binOp::T_plus,
            new T_temp("1"),  /*array base*/
            new T_binop(
                T_binOp::T_mul,
                new T_eseq(
                    nullptr, 
                    IRnode2T_exp(exp->translate())
                ),
                new T_const(wordsize)  /*word size*/
            )
        }
    );
    return root;
}

IRNode* A_varExp::translate(){
    if(var == nullptr)
        return NULL;
    T_eseq *root = new T_eseq{
        NULL,
        IRnode2T_exp(var->translate())
    };
    return root;
}

IRNode* A_callExp::translate(){
    T_call* root;
    if(args == nullptr)
        root = new T_call(new T_name(id), NULL);
    else
        root = new T_call(
            new T_name(id),
            IRNode2T_expList(args->translate())
        );
    return root;
}

IRNode* A_assignExp::translate(){
    if(var == nullptr || exp == nullptr)
        return NULL;
    T_move* root = new T_move(
        IRnode2T_exp(var->translate()),
        IRnode2T_exp(exp->translate())
    );
    return root;
}

IRNode* A_nilExp::translate(){
    return NULL;
}

IRNode* A_seqExp::translate(){
    if(seq == nullptr)
        return NULL;
    T_eseq* root = new T_eseq(
        NULL,
        IRnode2T_exp(seq->translate())
    );
    return root;
}

IRNode* A_intExp::translate(){
    T_const* root = new T_const(val);
    return root;
}

IRNode* A_stringExp::translate(){
    T_mem* root = new T_mem(
        new T_temp(val) /*从栈帧获取string对应的地址*/
    );
    return root;
}

IRNode* A_opExp::translate(){
    if(left == nullptr || right == nullptr)
        return NULL;
    if(op >= A_oper::plusOp && op <= A_oper::divideOp){
        T_binOp moperator = getBinOperator(op);
        T_binop* root = new T_binop(
            moperator,
            IRnode2T_exp(left->translate()),
            IRnode2T_exp(right->translate())
        );
        return root;
    }
    else if (op >=A_oper::eqOp && op <= A_oper::geOp){
        T_relOp moperator = getRelOperator(op);
        T_cjump* root = new T_cjump(
            moperator,
            IRnode2T_exp(left->translate()),
            IRnode2T_exp(right->translate()),
            new T_label("t"),
            new T_label("f")
        );
        return root;
    }
    else{
        //and 和 or 单独进行赋值处理
    }
}
int recordCount = 0;
IRNode* A_recordExp::translate(){
    recordCount = 0;
    T_stmList* varRoot = new T_stmList(NULL, NULL);
    T_stmList* varCurList = varRoot;
    A_efieldList* cur_var = fields.get();

    while (cur_var != nullptr) {
        recordCount ++;
        T_stm* cur_vart = IRNode2T_stm(cur_var->head->translate());
        if(cur_vart == NULL)
            std::cout << "Record wrong!" << std::endl;
        varCurList->head.reset(cur_vart);
        varCurList->tail.reset(new T_stmList(NULL, NULL));
        varCurList = varCurList->tail.get();
        cur_var = cur_var->tail.get();
    }
    T_eseq* root = new T_eseq(
        new T_seq(
            new T_move(
                new T_temp("r"),
                new T_call(
                    new T_name("malloc"),
                    new T_expList(
                        new T_const(recordCount * wordsize),
                        NULL
                    )
                )
            ),
            varRoot
        ),
        new T_temp("r") //获取新的temp表达式
    );
    return root;
}

IRNode* A_arrayExp::translate(){
    if(size == nullptr || init == nullptr)
        return NULL;
    T_call* root = new T_call(
        new T_name("initArray"),
        new T_expList(
            IRnode2T_exp(size->translate()),
            new T_expList(
                IRnode2T_exp(init->translate()),
                NULL
            )
        )
    );
    return root;
}
//这里暂定二值跳转都是无值表达式
IRNode* A_ifExp::translate(){
    if(fbody == nullptr){
        T_seq* root = new T_seq(
            new T_seq(
                IRNode2T_stm(test->translate()),
                new T_seq(
                    new T_label("t"),
                    IRNode2T_stm(tbody->translate())
                )
            ),
            new T_label("f")
        );
        return root;
    }
    else{
        T_seq* root = new T_seq(
            new T_seq(
                IRNode2T_stm(test->translate()),
                new T_seq(
                    new T_label("t"),
                    new T_seq(
                        IRNode2T_stm(tbody->translate()),
                        new T_seq(
                            new T_jump(NULL, new T_label("e")),
                            new T_seq(
                                new T_label("f"),
                                IRNode2T_stm(fbody->translate())
                            )
                        )
                    )
                )
            ),
            new T_label("e")
        );
        return root;
    }
}

IRNode* A_whileExp::translate(){
    if(test == nullptr || body == nullptr)
        return NULL;
    T_seq* root = new T_seq(
        new T_seq(
            new T_seq(
                new T_label("test"),
                IRNode2T_stm(test->translate())
            ),
            new T_seq(
                IRNode2T_stm(body->translate()),
                new T_jump(NULL, new T_label("done"))
            )
        ),
        new T_label("done")
    );
    return root;
}

IRNode* A_forExp::translate(){
    if(low == nullptr || high == nullptr || body == nullptr)
        return NULL;
    T_seq* root = 
    new T_seq(
        new T_seq(
            new T_seq(
                new T_move(
                    new T_temp(id),
                    IRnode2T_exp(low->translate())
                ),
                new T_move(
                    new T_temp("limit"),
                    IRnode2T_exp(high->translate())
                )
            ),
            new T_seq(
                new T_seq(
                    new T_label("test"),
                    new T_cjump(
                        T_relOp::T_le,
                        new T_temp(id),
                        new T_temp("limit"),
                        new T_label("t"),
                        new T_label("done")
                    )
                ),
                new T_seq(
                    new T_label("t"),
                    new T_seq(
                        IRNode2T_stm(body->translate()),
                        new T_jump(NULL, new T_label("test"))    
                    )
                )
            )
        ),
        new T_label("done")
    );
    return root;
}

IRNode* A_breakExp::translate(){
    T_jump* root = new T_jump(NULL, new T_label("done"));
    return root;
}

IRNode* A_letExp::translate(){
    if(decs == nullptr || body == nullptr)
        return NULL;
    
    std::cout << "Let Start" << std::endl;

    T_stmList* decRoot = new T_stmList(NULL, NULL);
    T_stmList* decCurList = decRoot;
    A_decList* cur_dec = decs.get();

    std::cout << "Dec Start" << std::endl;

    while (cur_dec != nullptr) {
        T_stm* cur_dect = IRNode2T_stm(cur_dec->head->translate());
        if(cur_dect == NULL)
            std::cout << "Let wrong!" << std::endl;
        decCurList->head.reset(cur_dect);
        decCurList->tail.reset(new T_stmList(NULL, NULL));
        decCurList = decCurList->tail.get();
        cur_dec = cur_dec->tail.get();
    }

    std::cout << "Exp Start" << std::endl;

    T_expList* expRoot = new T_expList(NULL, NULL);
    T_expList* expCurList = expRoot;
    A_expList* cur_exp = body.get();
    while (cur_exp != nullptr) {
        T_exp* cur_expt = IRnode2T_exp(cur_exp->head->translate());
        if(cur_expt == NULL)
            std::cout << "Let wrong!" << std::endl;
        
        expCurList->head.reset(cur_expt);
        expCurList->tail.reset(new T_expList(NULL, NULL));
        expCurList = expCurList->tail.get();
        cur_exp = cur_exp->tail.get();
    }

    std::cout << "Exp Finish" << std::endl;

    T_eseq* root = new T_eseq(
        decRoot,
        expRoot
    );
    std::cout << "Let Finish" << std::endl;
    return root;
}

IRNode* A_varDec::translate(){
    if(init == nullptr)
        return NULL;
    std::cout << "varDec Start" << std::endl;
    fcc.addLocalVar(id);
    T_move* root = new T_move(
        fcc.MEM(id, -1, NULL),
        IRnode2T_exp(init->translate())
    );
    std::cout << "varDec Finsh" << std::endl;
    return root;
}

IRNode* A_typeDec::translate(){
    return NULL;
}

IRNode* A_functionDec::translate(){
    if(body == nullptr)
        return NULL;
    // 应该进入新的环境以及保护变量
    T_eseq* root = new T_eseq(
        NULL, NULL
    );
    return root;
}

IRNode* A_nameTy::translate(){
    return NULL;
}

IRNode* A_recordTy::translate(){
    return NULL;
}

IRNode* A_arrayTy::translate(){
    return NULL;
}

IRNode* A_expList::translate(){
    if(head == nullptr)
        return NULL;
    
    T_expList* expRoot = new T_expList(NULL, NULL);
    T_expList* expCurList = expRoot;
    A_expList* cur_exp = this;
    while (cur_exp != nullptr) {
        T_exp* cur_expt = IRnode2T_exp(cur_exp->head->translate());
        if(cur_expt == NULL)
            std::cout << "Let wrong!" << std::endl;
        
        expCurList->head.reset(cur_expt);
        expCurList->tail.reset(new T_expList(NULL, NULL));
        expCurList = expCurList->tail.get();
        cur_exp = cur_exp->tail.get();
    }

    return expRoot;
}

IRNode* A_decList::translate(){
    return NULL;
}

IRNode* A_efield::translate(){
    if(offset == -1)
        offset = recordCount;
    T_move* root = new T_move(
        new T_mem(
            new T_binop(
                T_binOp::T_plus,
                new T_temp("t"),
                new T_const((offset - 1) * wordsize)
            )
        ),
        IRnode2T_exp(exp->translate())
    );
    return root;
}

>>>>>>> 6aa2918ba6b0e3a9e3a1c5f8adaa604259dc4638
