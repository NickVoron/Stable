SET PATH=..\..\..\..\Tools\bin\gnuwin32\bin;%PATH%
bison.exe --defines=parser/parser.h -oparser/parser.cpp parser/parser.yacc 
flex.exe -Sparser/flex.skl -oparser/lexer.cpp parser/lexer.lex
