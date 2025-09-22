#! /bin/sh

rm out.bin ; ./explode-pgn/build/multi-merge-to-polyglot `echo -n '-in ' ; echo lichess_db_standard_rated_20*bin | sed -e 's/ / -in /g'` -min-n 768
