#!/bin/bash
#Arguments -f FILENAME -l RealTimeLoader  -m MESSAGE | -s STATE [-d cgdb|strace]

DEBUG=""
while test $# -gt 0
do
    case "$1" in
        -d|--debug)
        DEBUG="$2"
        ;;
        -f)
        CFG_FILE="$2"
        ;;
        -l)
        LOADER="$2"
        ;;
        -s)
        STATE="$2"
        ;;
        -m)
        MESSAGE="$2"
        ;;
    esac
    shift
done

if [ -z ${MARTe2_DIR} ]; then 
    echo "environmental variable MARTe2_DIR unset"
    exit 1    
fi
if [ -z ${MARTe2_Components_DIR} ]; then 
    echo "environmental variable MARTe2_Components_DIR unset"
    exit 1
fi

if [ -z ${LOADER} ]; then 
    echo "environmental variable LOADER unset. Using default RealTimeLoader"
    LOADER=RealTimeLoader
fi
INPUT_ARGS="-l $LOADER"
if [ ! -z ${STATE} ]; then 
    INPUT_ARGS="$INPUT_ARGS -s $STATE "
elif [ ! -z ${MESSAGE} ]; then
    INPUT_ARGS="$INPUT_ARGS -m $MESSAGE" 
else
    echo " or -s or -m shall be specified"
    exit 1
fi

if [ -z $CFG_FILE ]; then
    echo "input configuration file with -f option shall be specified"
    exit 1
else
    INPUT_ARGS="$INPUT_ARGS -f $CFG_FILE" 
fi

if [ -z ${TARGET} ]; then 
    echo "environmental variable TARGET unset"
    exit
fi

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/${TARGET}/DataSources/MDSPlusEventDS/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/${TARGET}/Core/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/ALSA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/EPICSCA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/EPICSPVA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/LinuxTimer/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/LoggerDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/DAN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/FileDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/LinkDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/NI6259/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/NI6368/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/NI9157/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/OPCUADataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/SDN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/UDP/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/MDSReader/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/MDSWriter/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/RealTimeThreadSynchronisation/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/DataSources/RealTimeThreadAsyncBridge/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/GAMs/ConstantGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/GAMs/ConversionGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/GAMs/IOGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/GAMs/Interleaved2FlatGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/GAMs/FilterGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/GAMs/HistogramGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/GAMs/SSMGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/GAMs/TriggerOnChangeGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/GAMs/WaveformGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/Interfaces/BaseLib2Wrapper/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/Interfaces/EPICS/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/Interfaces/EPICSPVA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/Interfaces/MemoryGate/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/Interfaces/NI9157Device/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/Interfaces/OPCUA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/${TARGET}/Components/Interfaces/SysLogger/

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/MathExpressionGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SDN_CORE_LIBRARY_DIR
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EPICS_BASE/lib/$EPICS_HOST_ARCH
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SDN_CORE_LIBRARY_DIR
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CODAC_ROOT/lib/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(MDSPLUS_DIR)/lib/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH

echo "INPUT_ARGS = $INPUT_ARGS"
if [ "$DEBUG" = "cgdb" ]
then
    cgdb --args $MARTe2_DIR/Build/${TARGET}/App/MARTeApp.ex $INPUT_ARGS
elif [ "$DEBUG" = "valgrind" ]; then
    valgrind $MARTe2_DIR/Build/${TARGET}/App/MARTeApp.ex $INPUT_ARGS
else
    $MARTe2_DIR/Build/${TARGET}/App/MARTeApp.ex $INPUT_ARGS
fi

