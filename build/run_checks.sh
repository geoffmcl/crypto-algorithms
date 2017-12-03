#!/bin/sh

TMPFAIL=""
TMPFIL="bldlog-1.txt"
if [ ! -f "$TMPFIL" ]; then
    echo "Can NOT locate '$TMPFIL'! *** FIX ME ***"
    echo "This can be ANY existing file..."
    exit 1
fi

TMPCHK="temp-$TMPFIL"

TMPEXE="md5_test"
if [ ! -x "$TMPEXE" ]; then
    echo "Error: Can NOT locate $TMPEXE! *** FIX ME ***"
    exit 1
fi
TMPOUT="$TMPCHK.MD5"
if [ -f "$TMPOUT" ]; then
    rm -f $TMPOUT
fi
echo "Doing: '$TMPEXE $TMPFIL -o $TMPOUT'"
./$TMPEXE $TMPFIL -o $TMPOUT
cat $TMPOUT
echo ""
echo "Doing: '$TMPEXE -c $TMPOUT'"
./$TMPEXE -c $TMPOUT
if [ ! "$?" = "0" ]; then
    TMPFAIL="$TMPFAIL MD5"
fi
echo ""


TMPEXE="sha1_test"
if [ ! -x "$TMPEXE" ]; then
    echo "Error: Can NOT locate $TMPEXE! *** FIX ME ***"
    exit 1
fi
TMPOUT="$TMPCHK.SHA1"
if [ -f "$TMPOUT" ]; then
    rm -f $TMPOUT
fi
echo "Doing: '$TMPEXE $TMPFIL -o $TMPOUT'"
./$TMPEXE $TMPFIL -o $TMPOUT
cat $TMPOUT
echo ""
echo "Doing: '$TMPEXE -c $TMPOUT'"
./$TMPEXE -c $TMPOUT
if [ ! "$?" = "0" ]; then
    TMPFAIL="$TMPFAIL SHA1"
fi
echo ""

TMPEXE="sha256_test"
if [ ! -x "$TMPEXE" ]; then
    echo "Error: Can NOT locate $TMPEXE! *** FIX ME ***"
    exit 1
fi
TMPOUT="$TMPCHK.SHA256"
if [ -f "$TMPOUT" ]; then
    rm -f $TMPOUT
fi
echo "Doing: '$TMPEXE $TMPFIL -o $TMPOUT'"
./$TMPEXE $TMPFIL -o $TMPOUT
cat $TMPOUT
echo ""
echo "Doing: '$TMPEXE -c $TMPOUT'"
./$TMPEXE -c $TMPOUT
if [ ! "$?" = "0" ]; then
    TMPFAIL="$TMPFAIL MD5"
fi
echo ""

if [ -z "$TMPFAIL" ]; then
    echo "Appear ALL checks PASSED"
else
    echo "check $TMPFAIL FAILED"
fi
echo ""

# eof

