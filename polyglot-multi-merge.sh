#! /bin/sh

g++ polyglot-multi-merge.cpp -Ofast -o polyglot-multi-merge
rm out.bin ; ./polyglot-multi-merge -in `echo *bin | sed -e 's/ / -in /g'` -min-n 40
