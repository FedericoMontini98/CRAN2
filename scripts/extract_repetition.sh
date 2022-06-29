#!/bin/bash

# shell script to extract each repetition individually, given the configuration name and the statistic name to collect
# and other optional argument, like the number of repetitions

# edit the folder name of the simulation results (DIR_RESULT) 
# and the folder where you want to save the extracted files (DIR_OUT)

repeat=25
fileExt=vec
statType=vector
OPTIONS=hc:r:e:s:t:
DIR_RESULT="./CRAN2/CRAN2/simulations/results"
#DIR_RESULT="D:/PECSN_simulation_results/verification/continuity"
DIR_OUT="./CRAN2/csv_results"

usage() {
cat << EOF
Collect statistics from OMNET++

-h          help
-c          name of the configuration
-r          number of repetitions (default 25)
-e          extension of the file (default vec)
-s          statistics to collect
-t          type of the statistics (defaul vector)

EOF
}

# Parse command line
while getopts $OPTIONS opt ; do
	case $opt in
	h ) usage ; exit 0 ;;
	c ) configName=$OPTARG ;;
	r ) repeat=$OPTARG ;;
	e ) fileExt=$OPTARG ;;
	s ) statistic=$OPTARG ;;
    t ) statType=$OPTARG ;;
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


#for i in 40 42 44 ; do
for file_id in `seq $repeat` ; do
	file_id=$(($file_id - 1))
	path=${DIR_RESULT}"/${configName}"-#${file_id}.${fileExt}

	#echo "scavetool export --type v -o ${DIR_OUT}/${configName}_*#${file_id}_${statistic}.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f ${statistic}:$statType $path"
	scavetool export --type v -o ${DIR_OUT}/${configName}_${statistic}_#${file_id}.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f ${statistic}:$statType $path

done
#done

exit 0
