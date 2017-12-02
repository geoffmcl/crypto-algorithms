#!/bin/sh
BN=`basename $0`

TMPFAIL=""
TMPLIST="aes_test arcfour_test base64_test blowfish_test des_test md2_test rot-13_test"

for var in $TMPLIST; do
    if [ -x "$var" ]; then
        echo "$BN: Doing './$var'"
        ./$var
        if [ ! "$?" = "0" ]; then
            TMPFAIL="$TMPFAIL $var"
        fi
    else
        echo "$BN: Exe './$var' not found."
    fi
done 
echo ""
if [ -z "$TMPFAIL" ]; then
    echo "$BN: Appears ALL tests PASSED"
else
    echo "$BN: Tests $TMPFAIL FAILED"
fi
echo ""

# eof

