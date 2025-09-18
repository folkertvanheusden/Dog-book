#! /bin/sh

FILE="$1"
TAGS="/tmp/_tags.$$"

echo 'WhiteElo >= "2400"' > $TAGS
echo 'BlackElo >= "2400"' >> $TAGS

TARGET=`basename "$FILE" .pgn.zst`.bin

if [ ! -e "$TARGET" ] ; then
	pzstd -d -k -c $FILE | pgn-extract --quiet -t$TAGS -C /dev/stdin | polyglot make-book -pgn /dev/stdin -bin "$TARGET"
else
	echo skip $TARGET
fi

rm -f $TAGS
