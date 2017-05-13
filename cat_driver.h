#ifndef CAT_DRIVER
#define CAT_DRIVER

#include <string>
#include <map>
#include "cat.tab.hh"

class cat_driver;
// Tell Flex the lexer's prototype.
#define YY_DECL \
  yy::cat_parser::symbol_type yylex (cat_driver& driver)
// Declare it for the parser's sake.
YY_DECL;

class cat_driver{
public:
  cat_driver();
  virtual ~cat_driver();
  
  std::map<std::string,int>  int_var;
  std::map<std::string,string> string_var;
  int result;

  // Handling the scanner.
  void scan_begin();
  void scan_end();
  bool trace_scanning;

  // Run the parser on file f.
  // Return 0 on success.
  int parse (const std::string& f);
  // The name of the file being parsed.
  // Used later to pass the file name to the location tracker.
  std::string file;
  // Whether parser traces should be generated.
  bool trace_parsing;

  // Error handling.
  void error (const yy::location& loc, const std::string& msg);
  void error (const std::string& msg);
};


#endif //CAT_DRIVER