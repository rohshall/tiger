#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "abstract_syntax.h"

class DeclarationTable {
 public:
  DeclarationTable();
  bool addVar(const std::string& name, A_varDec* dec);
  bool addType(const std::string& name, A_typeDec* dec);
  bool addFunc(const std::string& name, A_functionDec* dec);
  A_varDec* retrieveVar(const std::string& name);
  A_typeDec* retrieveType(const std::string& name);
  A_functionDec* retrieveFunc(const std::string& name);

  void beginScope();
  void endScope();

 private:
  // add basic type:int string nil
  // add standard library function
  void addStandardLib();
  // just for convenient to access the table;
  std::unordered_map<std::string, A_varDec*>& vars();
  std::unordered_map<std::string, A_typeDec*>& types();
  std::unordered_map<std::string, A_functionDec*>& funcs();
  // expand the size of table
  template <typename T>
  void expand(T& table);
  template <typename T>
  auto retrieve(const std::string& name, std::vector<T>& table, int& depth)
      -> typename T::mapped_type;

 private:
  // var type function's declaration infomation
  // get the kind: table["name"]->kind
  std::vector<std::unordered_map<std::string, A_varDec*>> varTable;
  std::vector<std::unordered_map<std::string, A_typeDec*>> typeTable;
  std::vector<std::unordered_map<std::string, A_functionDec*>> funcTable;
  // current scope depth
  // depth 0 contains the define of the standard library.
  size_t curDepth = 0;
};

template <typename T>
void DeclarationTable::expand(T& table) {
  if (table.size() <= curDepth) {
    assert(table.size() == curDepth);
    table.emplace_back();
    table[curDepth].clear();
  } else {
    table[curDepth].clear();
  }
}

template <typename T>
auto DeclarationTable::retrieve(const std::string& name, std::vector<T>& table,int& depth) -> typename T::mapped_type {
  for (depth = curDepth; depth >= 0; --depth) {
    if (table[depth].find(name) != table[depth].end()) {
      return table[depth][name];
    }
  }
  depth = -1;
  return nullptr;
}

#endif  // SYMBOL_TABLE