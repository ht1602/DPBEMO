for alg in NSGA2_LTR;
do
    for problem in Swimmer;
    do
        beforeTime=`date +%s`
        ./EMOC -a $alg -p $problem -M 3 -D 18 -N 100 -e 100000 -r 2 -W 90,330,200 -q 25&&
        afterTime=`date +%s`
        echo "finished in $(($afterTime-$beforeTime)) seconds."
    done
done