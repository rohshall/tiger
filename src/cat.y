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
  #include <memory>
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

%type <A_exp*> exp seq funcall record array
%type <A_var*> lvalue 
%type <A_efieldList*> refields
%type <A_decList*> decs
%type <A_dec*> dec tydec vardec fundec
%type <A_Ty*> ty id
%type <A_fieldList*> typefields
%type <A_expList*> explist args

//%printer { code } symbols
%printer { yyoutput << $$; } <*>;

%%
%start code;
code: 
  %empty  { /*the unique_ptr is nulllptr by default.*/ }
  | exp   { driver.ast.reset(new A_Program(@1,$1)); }
;

exp:  
    lvalue                { $$ = new A_varExp(@1,$1); }
  | funcall               { $$ = $1; }
  | lvalue ":=" exp       { $$ = new A_assignExp(@1,$1,$3); }
  | NIL                   { $$ = new A_nilExp(@1,); }
  | seq                   { $$ = $1; }
  | INT                   { $$ = new A_intExp(@1,$1); }
  | STRING                { $$ = new A_stringExp(@1,$1); }
  | exp "+" exp           { $$ = new A_opExp(@1,A_oper::plus,$1,$3); }
  | exp "-" exp           { $$ = new A_opExp(@1,A_oper::minus,$1,$3); }
  | exp "*" exp           { $$ = new A_opExp(@1,A_oper::times,$1,$3); }
  | exp "/" exp           { $$ = new A_opExp(@1,A_oper::divide,$1,$3); }
  | exp "=" exp           { $$ = new A_opExp(@1,A_oper::eq,$1,$3); }
  | exp "<>" exp          { $$ = new A_opExp(@1,A_oper::neq,$1,$3); }
  | exp "<" exp           { $$ = new A_opExp(@1,A_oper::lt,$1,$3); }
  | exp "<=" exp          { $$ = new A_opExp(@1,A_oper::le,$1,$3); }
  | exp ">" exp           { $$ = new A_opExp(@1,A_oper::gt,$1,$3); }
  | exp ">=" exp          { $$ = new A_opExp(@1,A_oper::ge,$1,$3); }
  | exp "&" exp           { $$ = new A_opExp(@1,A_oper::and,$1,$3); }
  | exp "|" exp           { $$ = new A_opExp(@1,A_oper::or,$1,$3); }
  | "-" exp %prec UMINUS  { $$ = new A_opExp(@1,A_oper::minus,new A_intExp(@1,0),$2); }
  | record                { $$ = $1; }
  | array                 { $$ = $1; }
  | "if" exp "then" exp "else" exp      { $$ = new A_ifExp(@1,$2,$4,$6); }
  | "if" exp "then" exp                 { $$ = new A_ifExp(@1,$2,$4); }
  | "while" exp "do" exp                { $$ = new A_whileExp(@1,$2,$4); }
  | "for" id ":=" exp "to" exp "do" exp { $$ = new forExp(@1,$2,$4,$6); }
  | "break"                             { $$ = new A_breakExp(@1); }
  | "let" decs "in" explist "end"       { $$ = new A_letExp(@1,$2,$4); }
;
seq: 
    "(" explist ")" { $$ = new A_seqExp(@1,$2); }
;
record: 
    id "{" refields "}" { $$ = new A_recordExp(@1,$1,$3); }
;
refields: 
    id "=" exp "," refields { $$ = new A_efieldList(@1,new A_efield(@1,$1,$3),$5); }
  | id "=" exp              { $$ = new A_efieldList(@1,new A_efield(@1,$1,$3),nullptr); }
  | %empty                  { /*do nothing*/ }
;
array: 
    id "[" exp "]" "of" exp { $$ = new A_arrayExp(@1,$1,$3,$6); }
;
decs: 
    dec decs  { $$ = new A_decList(@1,$1,$2); }
  | %empty    { $$ = nullptr; }
;
dec: 
    tydec   { $$ = $1; }
  | vardec  { $$ = $1; }
  | fundec  { $$ = $1; }
;  
tydec: 
    "type" id "=" ty { $$ = new A_typeDec(@1,$2,$4); }
;
ty:  
    id { $$ = $1; }
  | "{" typefields "}" { $$ = new A_recordTy(@1,$2); }
  | "array" "of" id  { $$ = new A_arrayTy(@1,$3); }
;
typefields: 
    id ":" id "," typefields { $$ = new A_fieldList(@1,new A_field(@1,$1,$3),$5); }
  | id ":" id { $$ = new A_fieldList(@1,new A_field(@1,$1,$3),nullptr); }
  | %empty { $$ = nullptr; }
;
vardec: 
    "var" id ":=" exp         { $$ = new A_varDec(@1,$2,"",$4); }
  | "var" id ":" id ":=" exp  { $$ = new A_varDec(@1,$2,$4,$6); }
;
fundec: 
    "function" id "(" typefields ")" "=" exp { $$ = new A_functionDec(@1,$2,$4,"",$7); }
  | "function" id "(" typefields ")" ":" id "=" exp { $$ = new A_functionDec(@1,$2,$4,$7,$9); }
;
explist: 
    exp ";" explist { $$ = new A_expList(@1,$1,$3); }
  | exp { $$ = new A_expList(@1,$1,nullptr); }
  | %empty { $$ = nullptr; }
;
lvalue: 
    id { $$ = new A_simpleVar(@1,$1); }
  | lvalue "." id { $$ = new A_fieldVar(@1,$1,$3); }
  | lvalue "[" exp "]" { $$ = new A_subscriptVar(@1,$1,$3); }
;
funcall: 
    id "(" args ")" { $$ = new A_callExp(@1,$1,$3); }
;
args: 
    exp "," args { $$ = new A_expList(@1,$1,$3); }
  | exp { $$ = new A_expList(@1,$1,nullptr); }
  | %empty { $$ = nullptr; }
;
id: 
    ID { $$ = new A_nameTy(@1,$1); }
;  
%%

void yy::cat_parser::error (const location_type& loc,const std::string& msg){
  driver.error (loc, msg);
}
