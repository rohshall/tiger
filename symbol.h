#include <string>
#include <cassert>
#include <unordered_map>

class SymbolTable{
 public:
  void pushVar(const string& name,);
  void pushType();
  void pushFunc();
  void pop
 private:
  std::vector<std::unordered_multimap<std::string,>> varTable;
  std::unordered_multimap<std::string,> typeTable;
  std::unordered_multimap<std::string,> funcTable;
  std::stack<std::string> tokenSeq;
}