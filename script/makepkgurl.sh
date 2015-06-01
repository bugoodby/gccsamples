#!/bin/sh

TARGETS=`cat << EOS
abc-IFP-NOARCH-1.0-0.rpm
EOS
`

echo $TARGETS
for line in ${TARGETS[@]}; do
	PKGNAME=${line// }
	PREFIX=`echo $PKGNAME | cut -c 1`
	CATEGORY=`echo $PKGNAME | sed -e "s/^\(.\+-\(xxx\|yyy\|release\)\)-.\+/\1/"`
	echo http:/hogehoge/$PREFIX/$CATEGORY/
done

