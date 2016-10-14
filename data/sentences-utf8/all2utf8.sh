# /bin/bash

for file in `ls`
do 
nkf -w $file > abcxyz
mv abcxyz $file
done
