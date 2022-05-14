#!/bin/bash

repeat=25
fileExt=vec
statType=vector
OPTIONS=hc:r:s:
DIR_RESULT="./CRAN2/CRAN2/simulations/results"
#DIR_RESULT="D:/PECSN_simulation_results/verification/continuity"
DIR_OUT="./CRAN2/csv_results"

usage() {
cat << EOF
Collect statistics from OMNET++

-h          help
-c          name of the configuration
-r          number of repetitions (default 25)
-s          statistics to collect

EOF
}


# Parse command line
while getopts $OPTIONS opt ; do
	case $opt in
	h ) usage ; exit 0 ;;
	c ) configName=$OPTARG ;;
	r ) repeat=$OPTARG ;;
	s ) statistic=$OPTARG ;;
	esac
done
shift $(($OPTIND-1))

if [ -z $configName ] ; then
	echo "ERROR: you must provide a configuration name"
	usage
	exit 1
fi

if [ -z $statistic ] ; then
	echo "ERROR: you must provide a statistic to collect"
	usage
	exit 1
fi


#for i in 20 22 40 42 44 ; do	# continuity
for i in 3 7 10 16 20 ; do	# monotonicity cells
#for i in 20 23 30 ; do			# monotonicity dr
    
    ./CRAN2/scripts/extract_single_stat.sh -c "${configName}-${i}" -s ${statistic}

done

exit 0
