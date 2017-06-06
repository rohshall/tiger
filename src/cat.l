%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include "cat_driver.h"
#include "cat.tab.hh"

#undef yywrap
#define yywrap() 1

// The location of the current token.
static yy::location loc;
%}

%option noyywrap nounput batch debug noinput

Id      [a-zA-Z][a-zA-Z_0-9]*
Int     [0-9]+ 
String  \"([^\\\"\t\r\n]|\\n|\\r|\\t|\\\"|\\\\|\\[0-9]{3,3}|\\[ \t\r\n]*\\)*\"
Blank   [ \t]
Comment \/\*(([^\*]*(\*[^\/])?)*)\*\/
%{
// Code run each time a pattern is matched.
#define YY_USER_ACTION  loc.columns(yyleng);
%}

%%

%{
// Code run each time yylex is called.
loc.step ();
%}

{Blank}+    loc.step();
[\n]+       loc.lines(yyleng); loc.step();
","         return yy::cat_parser::make_COMMA(loc);
":"         return yy::cat_parser::make_COLON(loc);
";"         return yy::cat_parser::make_SEMICOLON(loc);
"("         return yy::cat_parser::make_LPAREN(loc);
")"         return yy::cat_parser::make_RPAREN(loc);
"["         return yy::cat_parser::make_LBRACK(loc);
"]"         return yy::cat_parser::make_RBRACK(loc);
"{"         return yy::cat_parser::make_LBRACE(loc);
"}"         return yy::cat_parser::make_RBRACE(loc);
"."         return yy::cat_parser::make_DOT(loc);
"+"         return yy::cat_parser::make_PLUS(loc);
"-"         return yy::cat_parser::make_MINUS(loc);
"*"         return yy::cat_parser::make_TIMES(loc);
"/"         return yy::cat_parser::make_DIVIDE(loc);
"="         return yy::cat_parser::make_EQ(loc);
"<>"        return yy::cat_parser::make_NEQ(loc);
"<"         return yy::cat_parser::make_LT(loc);
"<="        return yy::cat_parser::make_LE(loc);
">"         return yy::cat_parser::make_GT(loc);
">="        return yy::cat_parser::make_GE(loc);
"&"         return yy::cat_parser::make_AND(loc);
"|"         return yy::cat_parser::make_OR(loc);
":="        return yy::cat_parser::make_ASSIGN(loc);
"array"     return yy::cat_parser::make_ARRAY(loc);
"if"        return yy::cat_parser::make_IF(loc);
"then"      return yy::cat_parser::make_THEN(loc);
"else"      return yy::cat_parser::make_ELSE(loc);
"while"     return yy::cat_parser::make_WHILE(loc);
"for"       return yy::cat_parser::make_FOR(loc);
"to"        return yy::cat_parser::make_TO(loc);
"do"        return yy::cat_parser::make_DO(loc);
"let"       return yy::cat_parser::make_LET(loc);
"in"        return yy::cat_parser::make_IN(loc);
"end"       return yy::cat_parser::make_END(loc);
"of"        return yy::cat_parser::make_OF(loc);
"break"     return yy::cat_parser::make_BREAK(loc);
"function"  return yy::cat_parser::make_FUNCTION(loc);  
"var"       return yy::cat_parser::make_VAR(loc);
"type"      return yy::cat_parser::make_TYPE(loc); 
"nil"       return yy::cat_parser::make_NIL(loc);  
<<EOF>>     return yy::cat_parser::make_EOF(loc);

{Comment} {
  int num = 0;//number of \n
  for(int i=0;yytext[i]!='\0';++i){
    if(yytext[i]=='\n') num++;
  }
  loc.lines(num); loc.step();
}
{Int} {
  errno = 0;
  long n = strtol (yytext, NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    driver.error (loc, "integer is out of range");
  return yy::cat_parser::make_INT(n, loc);
}
{String}  {
  int len = strlen(yytext);
  if(len<2){
    driver.error (loc, "string match error");
  }
  char* tem = strdup(yytext);
  tem[len-1] = tem[len];
  return yy::cat_parser::make_STRING(tem+1,loc);
}
{Id} {
  return yy::cat_parser::make_ID(yytext, loc);    
}
.  {
  driver.error (loc, "invalid character");
}       
%%

//Because the scanner-related driver’s member-functions depend on the scanner’s data,
//it is simpler to implement them in this file.
void cat_driver::scan_begin(){
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-"){
    yyin = stdin;
  }else if (!(yyin = fopen (file.c_str (), "r"))){
    error("cannot open " + file + ": " + strerror(errno));
    exit(EXIT_FAILURE);
  }
}
void cat_driver::scan_end (){
  fclose (yyin);
}

