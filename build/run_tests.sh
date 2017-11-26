#!/bin/sh
TMPOPTS="-T"
TMPEXE="md5_test"
if [ ! -x "$TMPEXE" ]; then
    echo "Error: Can NOT locate $TMPEXE! *** FIX ME ***"
    exit 1
fi    
echo "Doing: $TMPEXE $TMPOPTS"
./$TMPEXE $TMPOPTS

TMPEXE="sha1_test"
if [ ! -x "$TMPEXE" ]; then
    echo "Error: Can NOT locate $TMPEXE! *** FIX ME ***"
    exit 1
fi    
echo "Doing: $TMPEXE $TMPOPTS"
./$TMPEXE $TMPOPTS

TMPEXE="sha256_test"
if [ ! -x "$TMPEXE" ]; then
    echo "Error: Can NOT locate $TMPEXE! *** FIX ME ***"
    exit 1
fi    
echo "Doing: $TMPEXE $TMPOPTS"
./$TMPEXE $TMPOPTS

# eof

