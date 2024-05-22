for alg in DPBEMO_MOEAD;
do
    for problem in RNA;
    do
        # beforeTime=`date +%s`
        # ./EMOC -a $alg -p $problem -M 2 -D 12 -N 100 -e 25000 -r 10 -i 15 -W -7.1,0.2 -q 50 -R "((((...))))." -I 1 &
        # afterTime=`date +%s`
        # echo "finished in $(($afterTime-$beforeTime)) seconds."
        # ./EMOC -a $alg -p $problem -M 2 -D 16 -N 100 -e 25000 -r 10 -i 15 -W -13.8,0.125 -q 50 -R "(((((......)))))" -I 2 &

        # ./EMOC -a $alg -p $problem -M 2 -D 17 -N 100 -e 25000 -r 10 -i 15 -W -10,0.6 -q 50 -R "((....)).((....))" -I 3 &
        
        # ./EMOC -a $alg -p $problem -M 2 -D 26 -N 100 -e 25000 -r 10 -i 15 -W -29,0.23 -q 50 -R "..((((((((.....)).)))))).." -I 4 &

        # ./EMOC -a $alg -p $problem -M 2 -D 30 -N 100 -e 25000 -r 10 -i 15 -W -22,0.33 -q 50 -R "(((((.....))..((.........)))))" -I 5 &

        # ./EMOC -a $alg -p $problem -M 2 -D 31 -N 100 -e 25000 -r 10 -i 15 -W -35.5,0.26 -q 50 -R ".(((((((((((...)))))....))))))." -I 6 &

        # ./EMOC -a $alg -p $problem -M 2 -D 34 -N 100 -e 25000 -r 10 -i 15 -W -37,0.24 -q 50 -R "....((((((((.(....)).).).)))))...." -I 7 &

        # ./EMOC -a $alg -p $problem -M 2 -D 35 -N 100 -e 25000 -r 10 -i 15 -W -28,0.4 -q 50 -R "((....)).((....)).((....)).((....))" -I 8 &

        # ./EMOC -a $alg -p $problem -M 2 -D 36 -N 100 -e 25000 -r 10 -i 15 -W -40,0.5 -q 50 -R "((((((.((((....))))))).))).........." -I 9 &

        ./EMOC -a $alg -p $problem -M 2 -D 36 -N 100 -e 25000 -r 10 -i 15 -W -45,0.17 -q 50 -R "((((((.((((((((....))))).)).).))))))" -I 10 &

    done
done