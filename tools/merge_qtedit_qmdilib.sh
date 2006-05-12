#! /bin/sh

set -e
# set -x

do_clean()
{
	rm -fr qtedit4 devqt co 
	rm -f _tmp_*_dump
}

make_qtedit4()
{
	echo  "create the qtedit4 repository"
	rm -fr qtedit4
	svnadmin create qtedit4 --fs-type=fsfs
	svn mkdir -m "Preparing for initial import of qtedit4" file://`pwd`/qtedit4/trunk 
	cat qtedit4-svn.dump | svnadmin --quiet --parent-dir=/trunk load  qtedit4
}

make_devqt()
{
	echo "create the devqt repository"
	rm -fr devqt
	svnadmin create devqt --fs-type=fsfs
	cat devqtsvn-dump | svnadmin --quiet load  devqt
}

generate_qmdilib()
{
	echo "dump the devqt, and filter the qmdlib path"
	svnadmin --quiet dump devqt > _tmp_devqt_svn_dump
	svnadmin --quiet dump devqt | svndumpfilter --quiet  --renumber-revs --drop-empty-revs  include tools/qmdilib > _tmp_qmdilib_svn_dump
}

#svn mkdir -m "Preparing for initial import of qmdlib" file://`pwd`/qtedit4/qmdilib
#cat _tmp_qmdilib_svn_dump | svnadmin --parent-dir=/qmdilib load  qtedit4

make_qmdi_lib()
{
	echo "create the qmidlib repository"
	rm -fr qmdilib
	svnadmin create qmdilib --fs-type=fsfs
	svn mkdir -m "Preparing for initial import of qmdlib" file://`pwd`/qmdilib/tools/ 
	cat _tmp_qmdilib_svn_dump | svnadmin load qmdilib

# 	svnadmin --quiet --incremental dump devqt | \
# 		svndumpfilter --quiet  --renumber-revs --drop-empty-revs  include tools/qmdilib | \
# 		svnadmin load qmdilib
}

merge_qtedit4_qdmilib()
{
	svn mkdir -m "Preparing for initial import of qmdlib" file://`pwd`/qtedit4/tools/
	cat _tmp_qmdilib_svn_dump | svnadmin load qtedit4
}

do_checkouts()
{
	echo "checking out all projects"
	rm -fr co
	
	echo " -  make a local checkout of qtedit4"
	mkdir -p co/qtedit4
	DIR=`pwd`/qtedit4
	cd co/qtedit4
	svn co file://$DIR .
	cd ../../
	
	echo " -   make a local checkout of devqt"
	mkdir -p co/devqt
	DIR=`pwd`/devqt
	cd co/devqt
	svn co file://$DIR .
	cd ../../
	
	echo " -   make a local checkout of qmdilib"
	mkdir -p co/qmdilib
	DIR=`pwd`/qmdilib
	cd co/qmdilib
	svn co file://$DIR .
	cd ../../
}

do_clean
make_qtedit4
make_devqt
generate_qmdilib
make_qmdi_lib
merge_qtedit4_qdmilib
do_checkouts
