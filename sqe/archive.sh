#!/bin/sh
CURDIR=`pwd`
TMPDIR=`mktemp -d ./sqe.XXXXX`
echo ${TMPDIR}
cp -R bin/ ${TMPDIR}/
cp -R sel/ ${TMPDIR}/
cp -R sqe/ ${TMPDIR}/
cp -R test/ ${TMPDIR}/
cp -R tmp/ ${TMPDIR}/
cp -R LICENSE ${TMPDIR}/
cp -R Makefile ${TMPDIR}/
cp -R README ${TMPDIR}/
tar cvzf ${CURDIR}/sqe.tar.gz ${TMPDIR}
rm -rf ${TMPDIR}/
