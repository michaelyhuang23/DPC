R=10000000
r=30

for((t=1; t<=3; t+=1)); do
    echo $t
    for((s=1; s<=16; s*=2)); do
	echo ${s}
	for((i=1000; i<=$R; i*=10)); do
	    
	    echo $i
	    cp ../../../ParCluster/dataset/simden/${i}.txt ../_dataset/synthetic.txt
	    echo ${r} > ../_parameter/cutoff.txt
	    echo ${s} > ../_parameter/thread_num.txt
	    ./sapproxdpc.out > results/simden${t}/orig_${r}_${i}_${s}.txt
	done
    done
done
