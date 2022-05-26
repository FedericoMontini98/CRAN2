#!/bin/bash

repeat=10
fileExt=vec
statType=vector
OPTIONS=hc:r:s:
DIR_RESULT="./CRAN2/CRAN2/simulations/results"
#DIR_RESULT="D:/PECSN_simulation_results/verification/continuity"
DIR_OUT="./../CRAN2/csv_results"

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
#for i in 20 22 40 42 44 ; do	# monotonicity_b_cells
#for i in 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2 ; do	
	for j in 7 7.09 7.170 7.245 7.313 ; do
		./extract_single_stat.sh -c "${configName}-1.9,${j}-" -s delayStat
#    done
done

exit 0
