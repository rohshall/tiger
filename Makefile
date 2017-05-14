DIR_SRC = ./src
DIR_TAR = ./build

ccfiles = $(DIR_SRC)/lex.yy.c $(DIR_SRC)/cat.tab.cc
hhfiles = $(DIR_SRC)/cat.tab.hh $(DIR_SRC)/location.hh $(DIR_SRC)/position.hh $(DIR_SRC)/stack.hh

cppfiles := $(wildcard $(DIR_SRC)/*.cpp)
hfiles := $(wildcard $(DIR_SRC)/*.h)

$(DIR_TAR)/cat.out: $(cppfiles) $(hfiles) $(hhfiles) $(ccfiles)
	g++ $(cppfiles) $(ccfiles) -o $@ -std=c++11

clean:
	rm $(DIR_TAR)/cat.out

