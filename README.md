This processes https://database.lichess.org/ into a book for Dog.

First download the .pgn.zst-files, then pre-process them using collect.sh (this takes a while - can be parallelized) and after that use combine.sh to convert the intermidiate .bin-files into a polyglot opening book ('out.bin').


-- Folkert van Heusden <folkert@vanheusden.com>
