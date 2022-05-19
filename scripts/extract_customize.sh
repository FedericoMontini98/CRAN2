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

mkdir -p ${DIR_OUT}/${configName}


#for i in 20 22 40 42 44 ; do	# continuity
#for i in 3 7 10 16 20 ; do	# monotonicity cells
#for i in 20 23 30 ; do			# monotonicity dr
#for i in 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1700 1800 ; do	# calibration_size
for i in 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2 ; do	# calibration_speed
	for j in 1000 1100 1200 1300 1400 1500 ; do
    
    	./CRAN2/scripts/extract_single_stat.sh -c "${configName}-${j},${i}-" -s ${statistic}
	done

done

exit 0
