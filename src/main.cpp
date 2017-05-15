#include <iostream>
#include "cat_driver.h"

int main(int argc, char *argv[]) {
  int res = 0;
  cat_driver driver;
  if(argc < 2){
    std::cout<<"Usage: "<<argv[0]<<" [options] [filename]\n";
    std::cout<<"       -p : enable trace_parsing\n";
    std::cout<<"       -s : enable trace_scanning"<<std::endl;
    return 0;
  }
  for (int i = 1; i < argc; ++i){
    if (argv[i] == std::string("-s"))
      driver.trace_scanning = true;
    else if(argv[i] == std::string("-p"))
      driver.trace_parsing = true;
    else
      driver.parse(argv[i]);
  }
  std::cout<<"Finish."<<std::endl;  
}