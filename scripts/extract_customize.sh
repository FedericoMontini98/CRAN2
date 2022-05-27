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

#for i in 20 22 40 42 44 ; do			# continuity
#for i in 3 7 10 16 20 ; do				# monotonicity cells
#for i in 20 23 30 ; do					# monotonicity dr
#for i in 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1700 1800 ; do		# calibration_size
for i in 0.1 0.5; do		# calibration_speed
#for i in 25 30 35 40 45 ; do			# %compression
	for j in 30 60 ; do
		for k in 1.1 3.7 ; do
			for n in 8 18 ; do
				for a in 0.010 0.015 ; do
				# for j in 7.09 7.245 7.313 ; do
					#./scripts/extract_single_stat.sh -c "${configName}-${j}" -s ${statistic}		# fede
					./extract_single_stat.sh -c "${configName}-${i},${j},${k},${n},${a}" -s ${statistic}
				done
			done
		done
	done
done

exit 0
