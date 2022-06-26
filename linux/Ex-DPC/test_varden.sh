R=10000000

for((s=30; s<=60; s*=2)); do
    echo ${s}
    for((i=1000; i<=$R; i*=10)); do
	r=10
	echo $i
	cp ../../../ParCluster/dataset/varden/${i}.txt ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/varden/dpc_${r}_${i}_${s}.txt
    done

    for((i=1000; i<=$R; i*=10)); do
	r=40
	echo $i
	cp ../../../ParCluster/dataset/varden/${i}.txt ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/varden/dpc_${r}_${i}_${s}.txt
    done

    for((i=1000; i<=$R; i*=10)); do
	r=160
	echo $i
	cp ../../../ParCluster/dataset/varden/${i}.txt ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/varden/dpc_${r}_${i}_${s}.txt
    done
done
