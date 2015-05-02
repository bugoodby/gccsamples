#!/bin/sh
GCOV=/usr/bin/gcov
GCOVDIR=./gcov`date '+%Y%m%d_%H%M%S'`

if [ $# -ne 1 ]; then
	echo "Usage: mygcov.sh <target_dir>"
	exit 0
fi

SRCDIR=$1

if [ -d $GCOVDIR ]; then
    rm -rf $GCOVDIR
fi
mkdir $GCOVDIR

echo "<target>"
find $SRCDIR -name "*.gcda"
echo ""

for file in `find $SRCDIR -name "*.gcda"`; do
#    $GCOV $file -b -r -o $SRCDIR
    $GCOV $file -b -r
done

mv ./*.gcov $GCOVDIR
