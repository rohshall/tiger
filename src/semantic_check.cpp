#include <cassert>
#include "abstract_syntax.h"
#include "cat_driver.h"
#include "symbol_table.h"
/**
 * the semanticCheck will set itself type
 * type including: void, nil, int, string and user-defined type
 */

void A_Program::semanticCheck(DeclarationTable& table) {
  exp->semanticCheck(table);
}

void A_simpleVar::semanticCheck(DeclarationTable& table) {
  A_varDec* dec = table.retrieveVar(id);
  if (dec == nullptr) {
    cat_driver::error(this->loc, "A_simpleVar: undefined variabe " + id);
    return;
  }
  this->type = dec->type_id;
}

void A_fieldVar::semanticCheck(DeclarationTable& table) {
  var->semanticCheck(table);
  A_typeDec* dec = table.retrieveType(var->type);
  if (dec == nullptr) return;
  if (dec->ty->kind != A_ty::Kind::A_recordTy) {
    cat_driver::error(var->loc, "A_fieldVar: record type required");
    return;
  }
  A_recordTy* ty_rec = dynamic_cast<A_recordTy*>(dec->ty.get());
  assert(ty_rec != nullptr);
  A_fieldList* cur = ty_rec->record.get();
  while (cur != nullptr) {
    if (cur->head->id == id) {
      break;
    }
    cur = cur->tail.get();
  }
  if (cur == nullptr) {
    cat_driver::error(var->loc, "A_fieldVar: no such field in record " + var->type);
  } else {
    this->type = cur->head->type_id;
  }
}

void A_subscriptVar::semanticCheck(DeclarationTable& table) {
  var->semanticCheck(table);
  exp->semanticCheck(table);
  A_typeDec* dec = table.retrieveType(var->type);
  if (dec == nullptr) return;
  if (dec->ty->kind != A_ty::Kind::A_arrayTy) {
    cat_driver::error(var->loc, "A_subscriptVar: array type required");
    return;
  }
  if (exp->type != "int") {
    cat_driver::error(exp->loc, "A_subscriptVar: int type required");
  }
  A_arrayTy* ty_arr = dynamic_cast<A_arrayTy*>(dec->ty.get());
  assert(ty_arr != nullptr);
  this->type = ty_arr->id;
}

void A_varExp::semanticCheck(DeclarationTable& table) {
  var->semanticCheck(table);
  this->type = var->type;
}

void A_nilExp::semanticCheck(DeclarationTable& table) { this->type = "nil"; }

void A_intExp::semanticCheck(DeclarationTable& table) { this->type = "int"; }

void A_stringExp::semanticCheck(DeclarationTable& table) {
  this->type = "string";
}

void A_callExp::semanticCheck(DeclarationTable& table) {
  A_functionDec* dec = table.retrieveFunc(this->id);
  if (dec == nullptr) {
    cat_driver::error(this->loc, "A_callExp: undefined function " + this->id);
    return;
  }
  // return type
  if (dec->type_id.empty()) {
    this->type = "void";
  } else {
    this->type = dec->type_id;
  }
  // check type of function parameters
  A_fieldList* cur_type = dec->params.get();
  A_expList* cur_exp = args.get();
  while (cur_exp != nullptr && cur_type != nullptr) {
    if (cur_type->head->type_id != cur_exp->head->type) {
      cat_driver::error(cur_exp->loc,"A_callExp: "+ cur_type->head->type_id + " type required");
    }
    cur_exp = cur_exp->tail.get();
    cur_type = cur_type->tail.get();
  }
  if (cur_exp != nullptr || cur_type != nullptr) {
    cat_driver::error(this->loc, "A_callExp: function params isn't match its declaration");
  }
}

void A_opExp::semanticCheck(DeclarationTable& table) {
  left->semanticCheck(table);
  right->semanticCheck(table);
  switch (op) {
    case A_oper::plusOp:
    case A_oper::minusOp:
    case A_oper::timesOp:
    case A_oper::divideOp:
    case A_oper::ltOp:
    case A_oper::leOp:
    case A_oper::gtOp:
    case A_oper::geOp:
      if (left->type != "int") {
        cat_driver::error(left->loc, "A_opExp: int type required");
        break;
      }
      if (right->type != "int") {
        cat_driver::error(right->loc, "A_opExp: int type required");
        break;
      }
      this->type = "int";
      break;
    case A_oper::eqOp:
    case A_oper::neqOp:
      if (left->type != right->type) {
        cat_driver::error(this->loc, "A_opExp: the same type required");
      } else if (left->type == "void" || right->type == "void") {
        cat_driver::error(this->loc, "A_opExp: non-void type required");
      } else {
        this->type = left->type;
      }
      break;
    case A_oper::andOp:
    case A_oper::orOp:
      if (left->type == "void") {
        cat_driver::error(left->loc, "A_opExp: non-void type required");
        break;
      }
      if (right->type == "void") {
        cat_driver::error(right->loc, "A_opExp: non-void type required");
        break;
      }
      this->type = "int";
      break;
  }
}

void A_recordExp::semanticCheck(DeclarationTable& table) {
  A_typeDec* dec = table.retrieveType(type_id);
  if (dec == nullptr) {
    cat_driver::error(this->loc, "A_recordExp: undefine record type " + type_id);
    return;
  }
  if (dec->ty->kind != A_ty::Kind::A_recordTy) {
    cat_driver::error(this->loc, "A_recordExp: record type required");
    return;
  }
  this->type = type_id;
  A_recordTy* ty_rec = dynamic_cast<A_recordTy*>(dec->ty.get());
  assert(ty_rec != nullptr);
  A_efieldList* cur_var = fields.get();
  A_fieldList* cur_def = ty_rec->record.get();
  while (cur_var != nullptr && cur_def != nullptr) {
    if (cur_def->head->id != cur_var->head->id) {
      cat_driver::error(cur_var->loc,
                        "A_recordExp: no such field of record type " + type_id);
      break;
    }
    cur_var->head->exp->semanticCheck(table);
    if (cur_var->head->exp->type != cur_def->head->type_id) {
      cat_driver::error(cur_var->loc, "A_recordExp: unmatched type of this variable");
      break;
    }
    cur_var = cur_var->tail.get();
    cur_def = cur_def->tail.get();
  }
}

void A_seqExp::semanticCheck(DeclarationTable& table) {
  A_expList* cur = seq.get();
  while (cur != nullptr) {
    cur->head->semanticCheck(table);
    this->type = cur->head->type;
    cur = cur->tail.get();
  }
}

void A_assignExp::semanticCheck(DeclarationTable& table) {
  var->semanticCheck(table);
  exp->semanticCheck(table);
  if (var->type == "void") {
    cat_driver::error(var->loc, "A_seqExp: the type cannot be void");
  }
  if (exp->type == "void") {
    cat_driver::error(exp->loc, "A_seqExp: the type cannnot be void");
  }
  if (var->type != exp->type) {
    cat_driver::error(this->loc, "A_seqExp: required the same type");
  }
}

void A_ifExp::semanticCheck(DeclarationTable& table) {
  test->semanticCheck(table);
  tbody->semanticCheck(table);
  if (fbody != nullptr) {
    fbody->semanticCheck(table);
    if (tbody->type != fbody->type) {
      cat_driver::error(this->loc, "A_ifExp: the type of two branch aren't the same");
    }
  }
  this->type = tbody->type;
}

void A_whileExp::semanticCheck(DeclarationTable& table) {
  test->semanticCheck(table);
  if (test->type != "int") {
    cat_driver::error(test->loc, "A_whileExp: int type required");
  }
  body->semanticCheck(table);
  if(body->type != "void"){
    cat_driver::error(test->loc, "A_whileExp: void type required");    
  }
}

void A_forExp::semanticCheck(DeclarationTable& table) {
  low->semanticCheck(table);
  high->semanticCheck(table);
  if (low->type != "int" || high->type != "int") {
    cat_driver::error(low->loc, "A_forExp: int type required");
  }
  table.beginScope();
  A_varDec intDec(id, "int", nullptr);
  table.addVar(id, &intDec);
  body->semanticCheck(table);
  table.endScope();
}

void A_breakExp::semanticCheck(DeclarationTable& table) { return; }

void A_letExp::semanticCheck(DeclarationTable& table) {
  table.beginScope();
  A_decList* cur_dec = decs.get();
  while (cur_dec != nullptr) {
    cur_dec->head->semanticCheck(table);
    cur_dec = cur_dec->tail.get();
  }
  A_expList* cur_exp = body.get();
  while (cur_exp != nullptr) {
    cur_exp->head->semanticCheck(table);
    cur_exp = cur_exp->tail.get();
  }
  table.endScope();
}

void A_arrayExp::semanticCheck(DeclarationTable& table) {
  A_typeDec* dec = table.retrieveType(type_id);
  if (dec == nullptr) {
    cat_driver::error(this->loc, "A_arrayExp: undefined type " + type_id);
    return;
  }
  if (dec->ty->kind != A_ty::Kind::A_arrayTy) {
    cat_driver::error(this->loc, "A_arrayExp: array type required");
    return;
  }
  size->semanticCheck(table);
  if (size->type != "int") {
    cat_driver::error(size->loc, "A_arrayExp: int type required");
  }
  init->semanticCheck(table);
  A_arrayTy* ty_arr = dynamic_cast<A_arrayTy*>(dec->ty.get());
  assert(ty_arr != nullptr);
  if (ty_arr->type != init->type) {
    cat_driver::error(init->loc, "A_arrayExp: unmatched array type");
  }
  this->type = type_id;
}

void A_varDec::semanticCheck(DeclarationTable& table) {
  init->semanticCheck(table);
  if (type_id.empty()) {
    if (init->type == "nil" || init->type == "void") {
      cat_driver::error(init->loc,
                        "A_varDec: init should not be nil without type in " + id);
    }
    table.addVar(id, this);
    this->type = init->type;
  } else {
    A_typeDec* dec = table.retrieveType(type_id);
    if (dec == nullptr) {
      cat_driver::error(this->loc, "undefined type " + type_id);
    } else {
      if (type_id != init->type) {
        cat_driver::error(
            this->loc,
            "A_varDec: unmatched type between " + type_id + " and " + init->type);
      }
      table.addVar(id, this);
      this->type = type_id;
    }
  }
}

void A_typeDec::semanticCheck(DeclarationTable& table) {
  bool res = table.addType(type_id, this);
  if (!res) {
    cat_driver::error(this->loc, "A_typeDec: the type has existed");
    return;
  }
  ty->semanticCheck(table);
  this->type = ty->type;
}

void A_functionDec::semanticCheck(DeclarationTable& table) {
  A_functionDec* fdec = table.retrieveFunc(id);
  if (fdec != nullptr) {
    cat_driver::error(this->loc, "A_functionDec: the function " + id + " has existed");
    return;
  }
  A_fieldList* cur = params.get();
  while (cur != nullptr) {
    A_typeDec* dec = table.retrieveType(cur->head->type_id);
    if (dec == nullptr) {
      cat_driver::error(cur->loc, "A_functionDec: undefined type " + cur->head->type_id);
    }
    cur = cur->tail.get();
  }
  body->semanticCheck(table);
  if (this->type_id.empty()) {
    this->type = "void";
  } else {
    if (body->type != this->type_id) {
      cat_driver::error(this->loc, "A_functionDec: unmatched return type " + type_id);
    }
    this->type = this->type_id;
  }
  table.addFunc(id, this);
}

void A_nameTy::semanticCheck(DeclarationTable& table) {
  A_typeDec* dec = table.retrieveType(id);
  if (dec == nullptr) {
    cat_driver::error(this->loc, "A_nameTy: undefined type " + id);
    return ;
  }
  this->type = dec->type;
}

void A_recordTy::semanticCheck(DeclarationTable& table) {
  A_fieldList* cur = record.get();
  while (cur != nullptr) {
    A_typeDec* dec = table.retrieveType(cur->head->type_id);
    if (dec == nullptr) {
      cat_driver::error(cur->loc, "A_recordTy: undefined type " + cur->head->type_id);
    }
    cur = cur->tail.get();
  }
}

void A_arrayTy::semanticCheck(DeclarationTable& table) {
  A_typeDec* dec = table.retrieveType(id);
  if (dec == nullptr) {
    cat_driver::error(this->loc, "A_arrayTy: undefined type " + id);
    return ;
  }
  this->type = dec->type;
}
