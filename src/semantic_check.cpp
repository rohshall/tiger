#include <cassert>
#include "abstract_syntax.h"
#include "cat_driver.h"
#include "symbol_table.h"
/**
 * the semanticCheck will set itself type
 * type including: void, nil, int, string and user-defined type
 */
int fieldCount = 0;
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
  fieldCount = 0;
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
    fieldCount++;
    if (cur->head->id == id) {
      break;
    }
    cur = cur->tail.get();
  }
  if (cur == nullptr) {
    cat_driver::error(var->loc,
                      "A_fieldVar: no such field in record " + var->type);
  } else {
    this->type = cur->head->type_id;
    this->offset = fieldCount;
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
  this->type = dec->type_id;
  // check type of function parameters
  A_fieldList* cur_type = dec->params.get();
  A_expList* cur_exp = args.get();
  while (cur_exp != nullptr && cur_type != nullptr) {
    cur_exp->head->semanticCheck(table);
    if (cur_type->head->type_id != cur_exp->head->type) {
      cat_driver::error(
          cur_exp->loc,
          "A_callExp: " + cur_type->head->type_id + " type required");
    }
    cur_exp = cur_exp->tail.get();
    cur_type = cur_type->tail.get();
  }
  if (cur_exp != nullptr || cur_type != nullptr) {
    cat_driver::error(this->loc,
                      "A_callExp: function params isn't match its declaration");
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
      this->type = "int";
      if (left->type != "int") {
        cat_driver::error(left->loc, "A_opExp: left int type required");
        break;
      }
      if (right->type != "int") {
        cat_driver::error(right->loc, "A_opExp: right int type required");
        break;
      }
      break;
    case A_oper::eqOp:
    case A_oper::neqOp:
      this->type = "int";    
      if (left->type == "nil" && right->type != "nil") {
        A_typeDec* dec = table.retrieveType(right->type);
        if (dec != nullptr && dec->ty->kind != A_ty::Kind::A_recordTy) {
          cat_driver::error(this->loc, "A_opExp: the same type required");
        }
      } else if (left->type != "nil" && right->type == "nil") {
        A_typeDec* dec = table.retrieveType(left->type);
        if (dec != nullptr && dec->ty->kind != A_ty::Kind::A_recordTy) {
          cat_driver::error(this->loc, "A_opExp: the same type required");
        }
      } else if (left->type != right->type) {
        cat_driver::error(this->loc, "A_opExp: the same type required");
      } else if (left->type == "void" || right->type == "void") {
        cat_driver::error(this->loc, "A_opExp: non-void type required");
      }
      break;
    case A_oper::andOp:
    case A_oper::orOp:
      this->type = "int";
      if (left->type == "void") {
        cat_driver::error(left->loc, "A_opExp: non-void type required");
        break;
      }
      if (right->type == "void") {
        cat_driver::error(right->loc, "A_opExp: non-void type required");
        break;
      }
      break;
  }
}

void A_recordExp::semanticCheck(DeclarationTable& table) {
  A_typeDec* dec = table.retrieveType(type_id);
  if (dec == nullptr) {
    cat_driver::error(this->loc,
                      "A_recordExp: undefine record type " + type_id);
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
    A_typeDec* tres = table.retrieveType(cur_def->head->type);

    if (tres != nullptr && cur_var->head->exp->type == "nil" &&
        tres->ty->kind == A_ty::Kind::A_recordTy) {
      // do nothing, just skip
    } else if (cur_var->head->exp->type != cur_def->head->type_id) {
      cat_driver::error(cur_var->loc,
                        "A_recordExp: unmatched type of this variable");
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
    cat_driver::error(var->loc, "A_assignExp: the type cannot be void");
  }
  if (exp->type == "void") {
    cat_driver::error(exp->loc, "A_assignExp: the type cannnot be void");
  }
  A_typeDec* dec = table.retrieveType(var->type);

  if (dec != nullptr && dec->ty->kind == A_ty::Kind::A_recordTy &&
      exp->type == "nil") {
    // do nothing, right
  } else if (var->type != exp->type) {
    cat_driver::error(this->loc, "A_assignExp: required the same type");
  }
  this->type = "void";
}

void A_ifExp::semanticCheck(DeclarationTable& table) {
  test->semanticCheck(table);
  tbody->semanticCheck(table);
  if (fbody != nullptr) {
    fbody->semanticCheck(table);
    if (tbody->type != fbody->type) {
      cat_driver::error(this->loc,
                        "A_ifExp: the type of two branch("+tbody->type +" and "+fbody->type+") aren't the same");
    }
    this->type = tbody->type;
  } else {
    if (tbody->type != "void") {
      cat_driver::error(tbody->loc, "A_ifExp: if then must be non value");
    }
    this->type = "void";
  }
}

void A_whileExp::semanticCheck(DeclarationTable& table) {
  test->semanticCheck(table);
  if (test->type != "int") {
    cat_driver::error(test->loc, "A_whileExp: int type required");
  }
  body->semanticCheck(table);
  if (body->type != "void") {
    cat_driver::error(test->loc, "A_whileExp: void type required");
  }
  this->type = "void";
}

void A_forExp::semanticCheck(DeclarationTable& table) {
  low->semanticCheck(table);
  high->semanticCheck(table);
  if (low->type != "int" || high->type != "int") {
    cat_driver::error(low->loc, "A_forExp: int type required");
  }
  table.beginScope();
  A_varDec intDec(id, "int", nullptr);
  intDec.type = "int";
  table.addVar(id, &intDec);
  body->semanticCheck(table);
  table.endScope();
  this->type = "void";
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
    this->type = cur_exp->head->type;
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
  A_typeDec* dec2 = table.retrieveType(dec->type);
  if (dec2 == nullptr) {
    cat_driver::error(this->loc, "A_arrayExp: undefined type " + dec->type);
    return;
  }
  if (dec2->ty->kind != A_ty::Kind::A_arrayTy) {
    cat_driver::error(this->loc, "A_arrayExp: array type required");
    return;
  }
  size->semanticCheck(table);
  if (size->type != "int") {
    cat_driver::error(size->loc, "A_arrayExp: int type required");
  }
  init->semanticCheck(table);
  A_arrayTy* ty_arr = dynamic_cast<A_arrayTy*>(dec2->ty.get());
  assert(ty_arr != nullptr);
  A_typeDec* dec3 = table.retrieveType(ty_arr->id);
  if (dec3!=nullptr && dec3->type != init->type){
    cat_driver::error(init->loc, "A_arrayExp: unmatched array type");
  }
  this->type = type_id;
}

void A_varDec::semanticCheck(DeclarationTable& table) {
  init->semanticCheck(table);
  if (type_id.empty()) {
    if (init->type == "nil" || init->type == "void") {
      cat_driver::error(
          init->loc, "A_varDec: init should not be nil without type in " + id);
    }
    table.addVar(id, this);
    this->type_id = this->type = init->type;
  } else {
    A_typeDec* dec = table.retrieveType(type_id);
    if (dec == nullptr) {
      cat_driver::error(this->loc, "A_varDec: undefined type " + type_id);
    } else {
      if (init->type == "nil") {
        A_typeDec* dec2 = table.retrieveType(dec->type);
        if (dec2 != nullptr && dec2->ty->kind != A_ty::Kind::A_recordTy) {
          cat_driver::error(init->loc, "A_varDec: init should not be nil");
        }
      } else if (init->type == "void") {
        cat_driver::error(init->loc, "A_varDec: init should not be void");
      } else {
        A_typeDec* dec2 = table.retrieveType(init->type);
        if (dec2 != nullptr && dec->type != init->type &&
            dec->type != dec2->type) {
          cat_driver::error(this->loc,
                            "A_varDec: unmatched type between " + type_id +
                                " and " + init->type);
        }
      }
      table.addVar(id, this);
      this->type = this->type_id;
    }
  }
}

// dec->type point to the actual type regardless of nameTy
void A_typeDec::semanticCheck(DeclarationTable& table) {
  bool res = table.addType(type_id, this);
  if (!res) {
    cat_driver::error(this->loc, "A_typeDec: the type has existed");
    return;
  }
  ty->semanticCheck(table);
  if (ty->kind == A_ty::Kind::A_nameTy) {
    this->type = ty->type;
  } else {
    this->type = type_id;
  }
}

void A_functionDec::semanticCheck(DeclarationTable& table) {
  A_functionDec* fdec = table.retrieveFunc(id);
  if (fdec != nullptr) {
    cat_driver::error(this->loc,
                      "A_functionDec: the function " + id + " has existed");
    return;
  }
  bool res = table.addFunc(id, this);

  A_fieldList* cur = params.get();
  while (cur != nullptr) {
    A_typeDec* dec = table.retrieveType(cur->head->type_id);
    if (dec == nullptr) {
      cat_driver::error(cur->loc,
                        "A_functionDec: undefined type " + cur->head->type_id);
    }
    {
      static std::unique_ptr<A_varDec> tvar(
          new A_varDec(cur->head->id, cur->head->type_id, nullptr));
      table.addVar(cur->head->id, tvar.get());
    }
    cur = cur->tail.get();
  }
  
  this->type = this->type_id;
  body->semanticCheck(table);
  if (body->type != this->type_id) {
    cat_driver::error(this->loc,
                      "A_functionDec: unmatched return type " + type_id);
  }
}

void A_nameTy::semanticCheck(DeclarationTable& table) {
  A_typeDec* dec = table.retrieveType(id);
  if (dec == nullptr) {
    cat_driver::error(this->loc, "A_nameTy: undefined type " + id);
    return;
  }
  this->type = dec->type;
}

void A_recordTy::semanticCheck(DeclarationTable& table) {
  A_fieldList* cur = record.get();
  while (cur != nullptr) {
    A_typeDec* dec = table.retrieveType(cur->head->type_id);
    if (dec == nullptr) {
      cat_driver::error(cur->loc,
                        "A_recordTy: undefined type " + cur->head->type_id);
    }
    cur = cur->tail.get();
  }
}

void A_arrayTy::semanticCheck(DeclarationTable& table) {
  A_typeDec* dec = table.retrieveType(id);
  if (dec == nullptr) {
    cat_driver::error(this->loc, "A_arrayTy: undefined type " + id);
    return;
  }
}
