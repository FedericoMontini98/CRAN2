#!/bin/bash

# shell script to customize, for extracting statistics, given a configuration name and a statistic name 
# and some other optional parameters

# used to extract similar configuration results, modified each time

# edit the folder name where you want to save the extracted files (DIR_OUT) or customize as nedeed

repeat=10
fileExt=vec
statType=vector
OPTIONS=hc:r:s:
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

mkdir -p ${DIR_OUT}/${configName}		# create a folder with the same name of the configuration (but don't save there the results automatically)

# nested for
#for i in 20 22 40 42 44 ; do			# continuity
#for i in 3 7 10 16 20 ; do				# monotonicity cells
#for i in 20 23 30 ; do					# monotonicity dr
#for i in 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1700 1800 ; do		# calibration_size
#for i in 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2 ; do		# calibration_speed
for i in 1200; do #`seq 6.55 0.1 7.2`; do		# datarate Mbps1.2 1.25 1.3 1.35 size 7.235 7.24 7.245 
	# for j in `seq start step end (included)` ; do
	for j in `seq 1.4 0.1 1.9` ; do	#10 12 14 16 18 20 22 24 26 28 30 ; do	# 32 34 36 38 40 42 44 46 48 50 ; do			# %compression
	# for j in 7.09 7.245 7.313 ; do
		#./scripts/extract_single_stat.sh -c "${configName}-${j}" -s ${statistic}		# fede
		./CRAN2/scripts/extract_single_stat.sh -c "${configName}-${i},${j}" -s ${statistic}
	done
done

# single for
#for j in `seq 2 2 9` ; do	#10 12 14 16 18 20 22 24 26 28 30 ; do	# 32 34 36 38 40 42 44 46 48 50 ; do			# %compression
#	./CRAN2/scripts/extract_single_stat.sh -c "${configName}-${j}" -s ${statistic}
#done

exit 0
