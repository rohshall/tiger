#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

class SymbolTable {
 public:
  SymbolTable();
  void add(const string& name, A_dec* dec);
  void enterScope();
  void exitScope();
 private:
  void initialize();
 private:
  // var type function's declaration infomation
  // get the kind: table["name"]->kind
  std::vector<std::unordered_map<std::string, A_dec*>> table;
  // current scope depth
  size_t curDepth;
}