# Lab 4 annotations
 
The modified `makeNrun.sh` now allows for arguments `embeddedN.bin` where N
is an integer between 2 and 6.
 
No integer = default, main + ML's asm code
2 = timed run, ML's asm code
3 = timed run, ML's C code
4 = default, main + OP's asm code
5 = timed run, OP's asm code
6 = timed run, OP's C code

Optimiziation flags set to -O0 by default.
