# /bin/bash
#
# This script generates .csv files from .sca and .vec files that are in a directory.
# The path of the directory must be passed from command line as first argument.
#
#Insert this script in a new folder called "scripts" in "omnetpp-5.7". Execute it with "./scripts/extract-warm-up-results.sh"


#Results location
DIR_RESULT="../CRAN2/simulations/results"
#DIR_RESULT="D:/PECSN_simulation_results/verification"
cd $DIR_RESULT

N=25	# repeat

for FILE in continuity_A*.vec
#for FILE in continuity_B*.vec
#for FILE in consistency_A*.vec
#for FILE in consistency_B*.vec
#for FILE in monotonicity_A*.vec
#for FILE in monotonicity_B_exp_dr*.vec
#for FILE in monotonicity_B_exp_cells*.vec
do
	name=$(echo $FILE | sed 's/-#.*.vec//')
	
#cell statistics
	scavetool export --type v -o $name-delayE2E.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f "delayStat:vector" $name-*.vec

#bbu statistics
	#scavetool export --type v -o $name-bbuOccupationQueueStat.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f "bbuOccupationQueueStat:vector" $name-*.vec
	#scavetool export --type v -o $name-bbuPacketInQueue.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f "bbuPacketInQueue:vector" $name-*.vec
	#scavetool export --type v -o $name-bbuQueueingTimeStat.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f "bbuQueueingTimeStat:vector" $name-*.vec
	#scavetool export --type v -o $name-bbuResponseTimeStat.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f "bbuResponseTimeStat:vector" $name-*.vec

#rrh statistics
	#scavetool export --type v -o $name-rrhResponseTimeStat.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f "rrhResponseTimeStat:vector" $name-*.vec
	#scavetool export --type v -o $name-rrhQueueingTimeStat.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f "rrhQueueingTimeStat:vector" $name-*.vec	
	#scavetool export --type v -o $name-rrhPacketInQueueStat.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f "rrhPacketInQueueStat:vector" $name-*.vec
done
