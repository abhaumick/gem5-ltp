#!/bin/bash

cd home
mkdir mntpnt
mount /dev/hdb ./mntpnt -t ext2
ls
cd ./mntpnt/parsec-2.1
ls

m5 dumpresetstats 0

./bin/parsecmgmt -a run -x pre -p blackscholes -i simdev

m5 dumpresetstats 0

./bin/parsecmgmt -a run -x pre -p blackscholes -i simsmall

m5 dumpresetstats 0

m5 exit