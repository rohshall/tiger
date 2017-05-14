DIR_SRC = ./src
DIR_TAR = ./build

cppfiles := $(wildcard ${DIR_SRC}/*.cpp)  
hfiles := $(wildcard ${DIR_SRC}/*.h)  

ccfiles := $(wildcard ${DIR_SRC}/*.cc)
hhfiles := $(wildcard ${DIR_SRC}/*.hh)


${DIR_TAR}/cat.out:
 ${cppfiles} ${hfiles}
	g++ -std=c++11 -o $@ ${cppfiles}  ${ccfiles}

lex.yy.cc: cat.l
	flex -+ cat.l

cat.tab.cc cat.tab.hh location.hh position.hh stack.hh: cat.y
	bison cat.y

.PHONY clean
clean:
	rm ${DIR_SRC}/*.hh
	rm ${DIR_SRC}/*.cc
	rm ${DIR_TAR}/*