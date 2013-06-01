#! /bin/sh

ROOT=$PWD
CMT=$ROOT/comments
PUBCMT=pub.cmt

subs_files()
{
	count=`expr ${count} + 1`
	echo "    " $count: subs file .${PWD##$ROOT}/$1

	FILE=$1
	cat $FILE > $FILE.tmp

	cmt_files=`grep "^// .*.cmt" $FILE | awk '{ print $2 ;}'`
	echo "\t\t" find [$cmt_files] in $1
	touch $FILE.tmp

	for cmt_file in $cmt_files
	do
		echo -n "\t\t" -\> substitute $cmt_file ...
		cat $CMT/$PUBCMT $CMT/$cmt_file > cmt.tmp
		sed '/'$cmt_file'/r cmt.tmp' $FILE > $FILE.tmp
		sed '/'$cmt_file'/d' $FILE.tmp > $FILE
		rm cmt.tmp
		echo " ok!" 
	done

	rm $FILE.tmp
}

subs_dir()
{
	all=`ls .`

	for i in $all
	do
		if [ -f "$i" ]
		then 
			# echo "   " \* checking file $i
			cfile=`echo $i | awk -F. '{print $NF}'`
			# echo $cfile
			if [ "$cfile" = "c" ]
			then 
				subs_files $i
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


echo Total \*.c files
find $1 -name "*.c" | wc -l

cd $CMT
cmt_files=`ls *.cmt`
echo $cmt_files
cd ..

SRC=$1
DST=$1-subs
echo $DST

if [ "$1" = "" ]
then
	echo Your must enter the dir name 
	echo For example: ./s.sh testdir
	exit
fi

rm -rf $DST
mkdir $DST
cp $SRC/* $DST -r

cd $DST
DIR=$DIR/$1
subs_dir 
cd ..

diff -Nur $SRC $DST > diff.$SRC

exit