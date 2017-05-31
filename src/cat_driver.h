#ifndef CAT_DRIVER
#define CAT_DRIVER

#include <string>
#include <memory>
#include "symbol_table.h"
#include "cat.tab.hh"
#include "ir_tree.h"
class cat_driver;
// Tell Flex the lexer's prototype.
#define YY_DECL \
  yy::cat_parser::symbol_type yylex (cat_driver& driver)
// Declare it for the parser's sake.
YY_DECL;

class cat_driver{
public:
  // The name of the file being parsed.
  // Used later to pass the file name to the location tracker.
  std::string file;
  // Type SymbolTable
  DeclarationTable decTable;
  // abstract syntax tree root
  std::unique_ptr<A_Program> ast;
  std::unique_ptr<T_stm> irt;
  // Whether parser traces should be generated.
  bool trace_scanning;
  bool trace_parsing;
  //-------------------------------------------------------
  cat_driver();
  virtual ~cat_driver();
  // Handling the scanner.
  void scan_begin();
  void scan_end();
  // Run the parser on file f.
  // Return 0 on success.
  void parse (const std::string& f);
  // Semantic check
  void semantic();
  // Translate ast to irt
  void translate();
  // Error handling.
  static void error (const yy::location& loc, const std::string& msg);
  static void error (const std::string& msg);
};


#endif //CAT_DRIVER