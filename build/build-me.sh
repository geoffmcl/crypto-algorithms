#!/bin/sh
#< build-me.sh - for crypto-fork - 20171126 - was for tcp-tests - 20151231 - was fgio 20141118
BN=`basename $0`

# relative SOURCE - adjust as needed
TMPSRC=".."
TMPCM="$TMPSRC/CMakeLists.txt"
TMPLOG="bldlog-1.txt"

wait_for_input()
{
    if [ "$#" -gt "0" ] ; then
        echo "$1"
    fi
    echo -n "$BN: Enter y to continue : "
    read char
    if [ "$char" = "y" -o "$char" = "Y" ]
    then
        echo "$BN: Got $char ... continuing ..."
    else
        if [ "$char" = "" ] ; then
            echo "$BN: Aborting ... no input!"
        else
            echo "$BN: Aborting ... got $char!"
        fi
        exit 1
    fi
}

ask()
{
    wait_for_input "$BN: *** CONTINUE? ***"
}


if [ ! -f "$TMPCM" ]; then
    echo "$BN: ERROR: Can NOT locate [$TMPCM] file! Check name, location, and FIX ME $0"
    exit 1
fi

CMOPTS=""
VERSBOSE=0
NOSG=1
DO_CMAKE=1

if [ -f "$TMPLOG" ]; then
    rm -f $TMPLOG
fi

give_help()
{
    echo "$BN [OPTIONS]"
    echo "OPTIONS"
    echo " VERBOSE = Use verbose build"
    echo " CLEAN   = Clean and exit."
    echo " NOCMAKE = Do NOT do cmake configuration if a Makefile exists."
    echo ""
    exit 1
}

do_clean()
{
    echo "$BN: Doing 'cmake-clean'"
    cmake-clean
    echo "$BN: Done 'cmake-clean'"
    exit 0
}


for arg in $@; do
      case $arg in
         VERBOSE) VERBOSE=1 ;;
         CLEAN) do_clean ;;
         NOCMAKE) DO_CMAKE=0 ;;
         --help) give_help ;;
         -h) give_help ;;
         -\?) give_help ;;
         *)
            echo "$BN: ERROR: Invalid argument [$arg]"
            exit 1
            ;;
      esac
done

echo "$BN: Commence build of fgio..." > $TMPLOG

if [ "$VERBOSE" = "1" ]; then
    CMOPTS="$CMOPTS -DCMAKE_VERBOSE_MAKEFILE=TRUE"
    echo "$BN: Enabling VERBOSE make"
    echo "$BN: Enabling VERBOSE make" >> $TMPLOG
fi

if [ ! -f "Makefile" ] || [ "$DO_CMAKE" = "1" ]; then
    echo "$BN: Doing: 'cmake $CMOPTS $TMPSRC', output to $TMPLOG"
    echo "$BN: Doing: 'cmake $CMOPTS $TMPSRC'" >> $TMPLOG
    cmake $CMOPTS $TMPSRC >> $TMPLOG 2>&1
    if [ ! "$?" = "0" ]; then
        echo "$BN: cmake configuration, generation error, see $TMPLOG"
        exit 1
    fi
else
    echo "$BN: Found Makefile - run CLEAN or CLEANALL to run CMake again..."
    echo "$BN: Found Makefile - run CLEAN or CLEANALL to run CMake again..." >> $TMPLOG
fi

if [ ! -f "Makefile" ]; then
    echo "$BN: ERROR: 'cmake $TMPOPTS ..' FAILED to create 'Makefile', see $TMPLOG"
    echo "$BN: ERROR: 'cmake $TMPOPTS ..' FAILED to create 'Makefile'" >> $TMPLOG
    exit 1
fi

echo "$BN: Doing: 'make', output to $TMPLOG"
echo "$BN: Doing: 'make'" >> $TMPLOG
make >> $TMPLOG 2>&1
if [ ! "$?" = "0" ]; then
    echo "$BN: Appears to be a 'make' error... see $TMPLOG"
    exit 1
else
    echo "$BN: Appears to be successful make ;=))"
    echo "$BN: Appears to be successful make ;=))" >> $TMPLOG
fi

# eof

