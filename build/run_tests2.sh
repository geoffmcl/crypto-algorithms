#!/bin/sh
BN=`basename $0`

TMPLIST="aes_test arcfour_test base64_test blowfish_test des_test md2_test rot-13_test"

for var in $TMPLIST; do
    if [ -x "$var" ]; then
        echo "$BN: Doing './$var'"
        ./$var
    else
        echo "$BN: Exe './$var' not found."
    fi
done 

# eof

