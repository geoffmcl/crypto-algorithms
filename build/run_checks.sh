#!/bin/sh
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
echo "Doing: '$TMPEXE $TMPFIL' to $TMPOUT"
./$TMPEXE $TMPFIL > $TMPOUT
cat $TMPOUT
echo "Doing: '$TMPEXE -c $TMPOUT'"
./$TMPEXE -c $TMPOUT
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
echo "Doing: '$TMPEXE $TMPFIL' to $TMPOUT"
./$TMPEXE $TMPFIL > $TMPOUT
cat $TMPOUT
echo "Doing: '$TMPEXE -c $TMPOUT'"
./$TMPEXE -c $TMPOUT
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
echo "Doing: '$TMPEXE $TMPFIL' to $TMPOUT"
./$TMPEXE $TMPFIL > $TMPOUT
cat $TMPOUT
echo "Doing: '$TMPEXE -c $TMPOUT'"
./$TMPEXE -c $TMPOUT
echo ""

# eof

