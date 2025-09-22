#! /bin/bash

for i in *.pgn.zst
do
	OUT="`basename "$i" .pgn.zst`".bin
	echo working on $i '->' $OUT
	pzstd -d -k -c "$i" | explode-pgn/build/explode-pgn "$OUT"
done
