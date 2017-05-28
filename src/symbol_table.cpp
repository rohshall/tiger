#include "symbol_table.h"
#include <cassert>
#include <memory>

std::unordered_map<std::string, A_varDec*>& DeclarationTable::vars(){
  return varTable[curDepth];
}

std::unordered_map<std::string, A_typeDec*>& DeclarationTable::types(){
  return typeTable[curDepth];
}

std::unordered_map<std::string, A_functionDec*>& DeclarationTable::funcs(){
  return funcTable[curDepth];
}

DeclarationTable::DeclarationTable(){
  expand(varTable);
  expand(typeTable);
  expand(funcTable);
  addStandardLib();
}

void DeclarationTable::addStandardLib(){
  // add type
  {
    static std::unique_ptr<A_typeDec> intType(new A_typeDec("int",new A_nameTy("int")));
    intType->type = "int";
    addType("int",intType.get());
  }
  {
    static std::unique_ptr<A_typeDec> stringType(new A_typeDec("string",new A_nameTy("string")));
    stringType->type = "string";
    addType("string",stringType.get());
  }

  // add function
  {
    static std::unique_ptr<A_functionDec> printFunc(new A_functionDec(
      "print",
      new A_fieldList(new A_field("s","string"),nullptr),
      "",
      nullptr
    ));
    addFunc("print",printFunc.get());
  }
  {
    static std::unique_ptr<A_functionDec> flushFunc(new A_functionDec(
      "flush",
      nullptr,
      "",
      nullptr
    ));
    addFunc("flush",flushFunc.get());
  }
  {
    static std::unique_ptr<A_functionDec> getcharFunc(new A_functionDec(
      "getchar",
      nullptr,
      "string",
      nullptr
    ));
    addFunc("getchar",getcharFunc.get());
  }
  {
    static std::unique_ptr<A_functionDec> ordFunc(new A_functionDec(
      "ord",
      new A_fieldList(new A_field("i","int"),nullptr),
      "int",
      nullptr
    ));
    addFunc("ord",ordFunc.get());
  }
  {
    static std::unique_ptr<A_functionDec> chrFunc(new A_functionDec(
      "chr",
      new A_fieldList(new A_field("i","int"),nullptr),
      "string",
      nullptr      
    ));
    addFunc("chr",chrFunc.get());
  }
  {
    static std::unique_ptr<A_functionDec> sizeFunc(new A_functionDec(
      "size",
      new A_fieldList(new A_field("s","string"),nullptr),
      "int",
      nullptr      
    ));
    addFunc("size",sizeFunc.get());
  }
  {
    static std::unique_ptr<A_functionDec> substringFunc(new A_functionDec(
      "substring",
      new A_fieldList(new A_field("s","string"),
        new A_fieldList(new A_field("first","int"),
          new A_fieldList(new A_field("n","int"),nullptr)
        )
      ),
      "string",
      nullptr
    ));
    addFunc("substring",substringFunc.get());
  }
  {
    static std::unique_ptr<A_functionDec> concatFunc(new A_functionDec(
      "concat",
      new A_fieldList(new A_field("s1","string"),
        new A_fieldList(new A_field("s2","string"),nullptr)
      ),
      "string",
      nullptr
    ));
    addFunc("concat",concatFunc.get());
  }
  {
    static std::unique_ptr<A_functionDec> notFunc(new A_functionDec(
      "not",
      new A_fieldList(new A_field("i","int"),nullptr),
      "int",
      nullptr      
    ));
    addFunc("not",notFunc.get());
  }
  {
    static std::unique_ptr<A_functionDec> exitFunc(new A_functionDec(
      "exit",
      new A_fieldList(new A_field("i","int"),nullptr),
      "",
      nullptr
    ));
    addFunc("exit",exitFunc.get());
  }
}

bool DeclarationTable::addVar(const std::string& name, A_varDec* dec){
  if(vars().find(name) != vars().end()) return false;
  vars().emplace(name,dec);
  return true;
}

bool DeclarationTable::addType(const std::string& name, A_typeDec* dec){
  if(types().find(name) != types().end()) return false;
  types().emplace(name,dec);
  return true;
}

bool DeclarationTable::addFunc(const std::string& name, A_functionDec* dec){
  if(funcs().find(name) != funcs().end()) return false;
  funcs().emplace(name,dec);
  return true;
}

A_varDec* DeclarationTable::retrieveVar(const std::string& name){
  int depth;
  return retrieve(name,varTable,depth);
}

A_typeDec* DeclarationTable::retrieveType(const std::string& name){
  int depth;
  return retrieve(name,typeTable,depth);
}

A_functionDec* DeclarationTable::retrieveFunc(const std::string& name){
  int depth;
  return retrieve(name,funcTable,depth);
}

void DeclarationTable::beginScope(){
  curDepth++;
  expand(varTable);
  expand(typeTable);
  expand(funcTable);
}

void DeclarationTable::endScope(){
  curDepth--;
  assert(curDepth>=0);
}


