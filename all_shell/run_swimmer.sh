for alg in MOEADPBO;
do
    for problem in Swimmer;
    do
        beforeTime=`date +%s`
        ./EMOC -a $alg -p $problem -M 2 -D 18 -N 100 -e 30000 -r 1 -W 110,180 -q 50 &&
        afterTime=`date +%s`
        echo "finished in $(($afterTime-$beforeTime)) seconds."
    done
done