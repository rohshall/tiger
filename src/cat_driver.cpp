#include "cat_driver.h"
#include "cat.tab.hh"

cat_driver::cat_driver() : trace_scanning(false), trace_parsing(false) {}
cat_driver::~cat_driver() {}

void cat_driver::parse(const std::string& f) {
  file = f;
  scan_begin();
  yy::cat_parser parser(*this);
  parser.set_debug_level(trace_parsing);
  parser.parse();
  scan_end();
}

void cat_driver::semantic() {
  ast->semanticCheck(decTable);
}

void cat_driver::printast() {
  std::cout << "Print AbstractTree Node." << std::endl;
  ast->printast();
  std::cout << "\nFinish." << std::endl;
}

void cat_driver::translate() {
//  irt.reset(ast->translate());
  irt = ast->translate();
//  irt = ast->translate(decTablet);
}

void cat_driver::printirt(){
  std::cout << "Print IRTree Nodes." << std::endl;
  switch(irt->nodeKind){
    case IRNode::NodeKind::T_exp : IRnode2T_exp(irt)->printirt(); break;
    case IRNode::NodeKind::T_stm : IRNode2T_stm(irt)->printirt(); break;
  }
  openFile();
  switch(irt->nodeKind){
    case IRNode::NodeKind::T_exp : std::cout << IRnode2T_exp(irt)->printirtDot() << std::endl; break;
    case IRNode::NodeKind::T_stm : std::cout << IRNode2T_stm(irt)->printirtDot() << std::endl; break;
  }
  closeFile();
  std::cout << "\nFinish." << std::endl;
}

void cat_driver::error(const yy::location& loc, const std::string& msg) {
  std::cout << loc << " : " << msg << std::endl;
}
void cat_driver::error(const std::string& msg) {
  std::cout << msg << std::endl;
}