#include "cat_driver.h"
#include "cat.tab.hh"

cat_driver::cat_driver() : trace_scanning(false), trace_parsing(false) {}
cat_driver::~cat_driver() {}

int cat_driver::parse(const std::string& f) {
  file = f;
  scan_begin();
  yy::cat_parser parser(*this);
  parser.set_debug_level(trace_parsing);
  int res = parser.parse();
  scan_end();
  return res;
}

void cat_driver::error(const yy::location& loc, const std::string& msg) {
  std::cout << loc << " : " << msg << std::endl;
}
void cat_driver::error(const std::string& msg) {
  std::cout << msg << std::endl;
}