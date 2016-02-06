#!/bin/sh

function usage()
{
	echo "ファイル先頭にLCOV_EXCL_START、末尾にLCOV_EXCL_STOPを追加します"
    echo "usage: `basename $0` <dir>"
	echo ""
}


function add_lcov_excl()
{
	echo "------------------------------"
	echo " add lcov comment"
	echo "------------------------------"
	for file in `find $1 \( -iname "*.[c|h]" -o -iname "*.[c|h]pp" -o -iname "*.[c|h]xx" \)`; do
		echo "  - $file"
		sed -i -e "1i // LCOV_EXCL_START" $file
		echo "// LCOV_EXCL_STOP" >> $file
	done
}

if [ $# -ne 1 ]; then
    usage
    exit 0
fi

add_lcov_excl $1



