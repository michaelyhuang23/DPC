r=1

for((t=1; t<=3; t+=1)); do
    echo $t
    for((s=1; s<=16; s*=2)); do
        echo ${s}
        cp ../../../ParCluster/dataset/reallife/3D_GeoLife_24M.pbbs ../_dataset/synthetic.txt
        echo ${r} > ../_parameter/cutoff.txt
        echo ${s} > ../_parameter/thread_num.txt
        ./sapproxdpc.out > results/GeoLife${t}/orig_${r}_${s}.txt
    done
    for((s=30; s<=60; s*=2)); do
        echo ${s}
        cp ../../../ParCluster/dataset/reallife/3D_GeoLife_24M.pbbs ../_dataset/synthetic.txt
        echo ${r} > ../_parameter/cutoff.txt
        echo ${s} > ../_parameter/thread_num.txt
        ./sapproxdpc.out > results/GeoLife${t}/orig_${r}_${s}.txt
    done
done
