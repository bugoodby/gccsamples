#!/bin/sh
GCOV=/usr/bin/gcov
LCOVDIR=./lcov`date '+%Y%m%d_%H%M%S'`

if [ $# -ne 1 ]; then
	echo "Usage: mygcov.sh <target_dir>"
	exit 0
fi

SRCDIR=$1

if [ -d $LCOVDIR ]; then
	rm -rf $LCOVDIR
fi
mkdir $LCOVDIR

lcov --capture --directory $SRCDIR --output-file $LCOVDIR/lcov.info --gcov-tool $GCOV --rc lcov_branch_coverage=1 --no-external
cd $LCOVDIR
genhtml --branch-coverage --demangle-cpp lcov.info
#genhtml --no-function-coverage --branch-coverage --demangle-cpp lcov.info
