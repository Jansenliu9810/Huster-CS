chcp 65001
win_flex lex.l
win_bison -d parser.y
gcc -o parser lex.yy.c parser.tab.c ast.c
parser test.c