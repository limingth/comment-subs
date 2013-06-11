
FILES=`cat 1line.list`

echo $FILES

#sed 's/\* /\*\n \* /' test.cmt > test.cmt2
#cat test.cmt2
#sed 's/ \*\//\n \*\//' test.cmt2  > test.cmt

for file in $FILES
do
	echo $file
	sed 's/\* /\*\n \* /' $file > tmp
	sed 's/ \*\//\n \*\//' tmp > $file
done
