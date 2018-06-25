#PBS -l procs=12
#PBS -l walltime=4:00:00
#PBS -N main
#PBS -o main.out
#PBS -j oe
#PBS -m ae


if [ "$PBS_ENVIRONMENT" != "PBS_INTERACTIVE" ] ; then
cd $PBS_O_WORKDIR
fi

##
# Takes timings of Convex Hulls for various configurations 
# Adapted from Andrew Hilton's bash script
##


BIN=./main  #this variable is the name of the executable

wall=" ================================================================= "

# Here are the test parameters
TRIALS=5
SIZES="100 1000 10000 100000 1000000"
threads="1 2 4 8 12"

echo "Convex Hull Timings"
echo 
for n in $SIZES; do
	echo "Num Points: ${n}"
	echo $wall
	for t in $threads; do
	    echo "Thread Num: ${t}"
	    for i in 1 .. $TRIALS ; do
	    echo "Trial ${i}"
		$BIN 1000 1000 $n $t "${n}p_${t}t_${i}.ppm"
		echo
	    done
	    echo $wall
	done
    echo
    echo
    echo
done
