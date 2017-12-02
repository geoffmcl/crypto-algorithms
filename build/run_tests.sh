#!/bin/sh
TMPFAIL=""
TMPOPTS="-T"
TMPTEST="md5"
TMPEXE="md5_test"
if [ ! -x "$TMPEXE" ]; then
    echo "Error: Can NOT locate $TMPEXE! *** FIX ME ***"
    exit 1
fi    
echo "Doing: $TMPEXE $TMPOPTS"
./$TMPEXE $TMPOPTS
if [ ! "$?" = "0" ]; then
    TMPFAIL="$TMPFAIL $TMPTEST"
fi

TMPTEST="sha1"
TMPEXE="sha1_test"
if [ ! -x "$TMPEXE" ]; then
    echo "Error: Can NOT locate $TMPEXE! *** FIX ME ***"
    exit 1
fi    
echo "Doing: $TMPEXE $TMPOPTS"
./$TMPEXE $TMPOPTS
if [ ! "$?" = "0" ]; then
    TMPFAIL="$TMPFAIL $TMPTEST"
fi

TMPTEST="sha256"
TMPEXE="sha256_test"
if [ ! -x "$TMPEXE" ]; then
    echo "Error: Can NOT locate $TMPEXE! *** FIX ME ***"
    exit 1
fi    
echo "Doing: $TMPEXE $TMPOPTS"
./$TMPEXE $TMPOPTS
if [ ! "$?" = "0" ]; then
    TMPFAIL="$TMPFAIL $TMPTEST"
fi

echo ""
if [ -z "$TMPFAIL" ]; then
	echo "Appears ALL tests PASSED"
else
	echo "Tests $TMPFAIL FAILED"
fi
echo ""

# eof

