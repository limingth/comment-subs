#! /bin/bash

function subs_file()
{
	count=`expr ${count} + 1`
	echo "    " $count: subs file $i

	FILE=$1
	cat $FILE > $FILE.tmp

	for cmt_file in $cmt_files
	do
		# echo substitute $cmt_file
		cat $CMT/$PUBCMT $CMT/$cmt_file > cmt.tmp
		sed '/'$cmt_file'/r cmt.tmp' $FILE.tmp > $FILE.tmp2
		sed '/'$cmt_file'/d' $FILE.tmp2 > $FILE.tmp 
		rm cmt.tmp
	done

	# cat $FILE.tmp
	cp $FILE.tmp $FILE
	rm $FILE.tmp
	rm $FILE.tmp2
}

function subs_dir()
{
	all=`ls .`

	for i in $all
	do
		if [ -f "$i" ]
		then 
			# echo "   " \* checking file $i
			cfile=`echo $i | awk -F. '{print $NF}'`
			# echo $cfile
			if [ "$cfile" == "c" ]
			then 
				subs_file $i
			fi 
		fi

		if [ -d "$i" ]
		then
			cd $i
			echo + entering $i
			subs_dir 
			cd ..
		fi
	done
}

ROOT=$PWD
CMT=$ROOT/comments
PUBCMT=pub.cmt

echo Total \*.c files
find $1 -name "*.c" | wc -l

cd $CMT
cmt_files=`ls *.cmt`
echo $cmt_files
cd ..

SRC=$1
DST=$1_subs
echo $DST

if [ "$1" == "" ]
then
	echo Your must enter the dir name 
	echo For example: ./s.sh testdir
	exit
fi

rm -rf $DST
mkdir $DST
cp $SRC/* $DST -r

cd $DST
subs_dir 
cd ..

diff -Nur $SRC $DST > diff.$SRC

exit
