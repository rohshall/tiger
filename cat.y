%language "C++"
%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"


%defines
%define parser_class_name {cat_parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert

//The %code directive inserts code verbatim into the output parser source 
//at any of a predefined set of locations. 
//An alternative to the traditional Yacc prologue, %{code%}.
%code requires {
  #include <string>
  class cat_driver;
}

// The parsing context.
%param { cat_driver& driver }

%locations
%initial-action {
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};

//enable parser tracing
%define parse.trace
//enable verbose error messages
%define parse.error verbose

//code that is output in the *.cc file
%code {
  #include "cat_driver.h"
}


//the following line allows for nicer error messages
%define api.token.prefix {TOK_}
%token
  END  0    "end of file"
  COMMA     ","
  COLON     ":"
  SEMICOLON ";"
  LPAREN    "("
  RPAREN    ")"
  LBRACK    "["
  RBRACK    "]"
  LBRACE    "{"
  RBRACE    "}"
  DOT       "."
  PLUS      "+"
  MINUS     "-"
  TIMES     "*"
  DIVIDE    "/"
  EQ        "="
  NEQ       "<>"
  LT        "<"
  LE        "<="
  GT        ">"
  GE        ">="
  AND       "&"
  OR        "|"
  ASSIGN    ":="
  ARRAY     "array"
  IF        "if"
  THEN      "then"
  ELSE      "else"
  WHILE     "while"
  FOR       "for"
  TO        "to"
  DO        "do"
  LET       "let"
  IN        "in"
  END       "end"
  OF        "of"
  BREAK     "break"
  FUNCTION  "function" 
  VAR       "var"
  TYPE      "type"
  NIL       "nil"
;

//use variant-based semantic values
%token <std::string> ID "identifier"
%token <std::string> STRING "string"
%token <int> INT "integer"

%right ASSIGN
%left AND OR
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS

//TODO set the type of nonterminal
%type  <int> exp

//TODO: %printer { code } symbols
%printer { yyoutput << $$; } <*>;

%%
%start code;
code: 
  %empty 
  | exp 
;

exp:  
    lvalue 
  | funcall 
  | lvalue ":=" exp 
  | NIL 
  | seq
  | INT
  | STRING
  | "let" decs "in" explist "end"
  | exp "+" exp 
  | exp "-" exp 
  | exp "*" exp 
  | exp "/" exp 
  | exp "=" exp
  | exp "<>" exp
  | exp "<" exp
  | exp "<=" exp
  | exp ">" exp
  | exp ">=" exp
  | exp "&" exp
  | exp "|" exp
  | "-" exp %prec UMINUS 
  | record
  | array
  | "if" exp "then" exp "else" exp 
  | "while" exp "do" exp 
  | "for" id ":=" exp "to" exp "do" exp 
  | "break" 
;
seq: 
    "(" explist ")"
;
record: 
    id "{" refields "}"
;
refields: 
    id "=" exp "," refields
  | id "=" exp
  | %empty
;
array: 
    id "[" exp "]" "of" exp 
;
decs: 
    dec decs
  | %empty
;
dec: 
    tydec
  | vardec
  | fundec
;  
tydec: 
    "type" id "=" ty
;
ty:	
    id
  | "{" typefields "}" 
  | "array" "of" id 
;
typefields: 
    id ":" id "," typefields
  | id ":" id
	| %empty
;
vardec: 
    "var" id ":=" exp 
  | "var" id ":" id ":=" exp 
;
fundec: 
    "function" id "(" typefields ")" "=" exp
  | "function" id "(" typefields ")" ":" id "=" exp
;
explist: 
    exp ";" explist
	| exp
	| %empty
;
lvalue: 
    id /*{F("");}*/
  | lvalue "." id 
  | lvalue "[" exp "]" 
;
funcall: 
    id "(" args ")" 
;
args: 
    exp "," args
  | exp
	| %empty
;
id: 
    ID 
;  
%%

void yy::cat_parser::error (const location_type& loc,const std::string& msg){
  driver.error (loc, msg);
}
