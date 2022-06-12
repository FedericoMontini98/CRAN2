#!/bin/bash

repeat=25
fileExt=vec
statType=vector
OPTIONS=hc:r:s:
DIR_RESULT="C:/Users/Emanuele/Desktop/Projects/C-RAN2 Main/CRAN2/CRAN2/simulations/results"
#DIR_RESULT="D:/PECSN_simulation_results/verification/continuity"
DIR_OUT="C:/Users/Emanuele/Desktop/Projects/C-RAN2 Main/results"
echo a
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

for j in 0.1 0.5 ; do
	for i in 30 60 ; do
		for k in 1.1 3.7 ; do
			for m in 7 18 ; do
				for n in 0.01 0.02 ; do    
    				echo ./CRAN2/scripts/extract_single_stat_ema.sh -c "${configName}-${j},${i},${k},${m},${n}-" -s ${statistic}
				done
			done
		done
	done	
done

exit 0
