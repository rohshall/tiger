#ifndef ZTRANSLATE
#define ZTRANSLATE

#include <cassert>
#include "abstract_syntax.h"
#include "ir_tree.h"
#include "frame.h"
#include "cat_driver.h"
#include <iostream>

T_stm *IRNode2T_stm(IRNode *root);
T_exp *IRnode2T_exp(IRNode *root);
T_expList* IRNode2T_expList(IRNode *root);
T_binOp getBinOperator(A_oper op);
T_relOp getRelOperator(A_oper op);

#endif