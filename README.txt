

current problems:

1. yylex() has a memory leak from the scanner
   idk how to fix

2. ast structures do not have proper destroy() functions
   they currently cause a memory leak
