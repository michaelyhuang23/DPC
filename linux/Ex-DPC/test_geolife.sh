for((s=1; s<=1; s*=2)); do
    echo ${s}

	r=5

	cp ../../../ParCluster/dataset/reallife/3D_GeoLife_1K.pbbs ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/reallife/geolife/dpc_${r}_1000_${s}.txt

	cp ../../../ParCluster/dataset/reallife/3D_GeoLife_10K.pbbs ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/reallife/geolife/dpc_${r}_10000_${s}.txt

	cp ../../../ParCluster/dataset/reallife/3D_GeoLife_100K.pbbs ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/reallife/geolife/dpc_${r}_100000_${s}.txt


	cp ../../../ParCluster/dataset/reallife/3D_GeoLife_1M.pbbs ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/reallife/geolife/dpc_${r}_1000000_${s}.txt

done

for((s=30; s<=60; s*=2)); do
    echo ${s}

	r=5

	cp ../../../ParCluster/dataset/reallife/3D_GeoLife_1K.pbbs ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/reallife/geolife/dpc_${r}_1000_${s}.txt

	cp ../../../ParCluster/dataset/reallife/3D_GeoLife_10K.pbbs ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/reallife/geolife/dpc_${r}_10000_${s}.txt

	cp ../../../ParCluster/dataset/reallife/3D_GeoLife_100K.pbbs ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/reallife/geolife/dpc_${r}_100000_${s}.txt

	
	cp ../../../ParCluster/dataset/reallife/3D_GeoLife_1M.pbbs ../_dataset/synthetic.txt
	echo ${r} > ../_parameter/cutoff.txt
	echo ${s} > ../_parameter/thread_num.txt
	./exdpc.out > results/reallife/geolife/dpc_${r}_1000000_${s}.txt
	

done
