for alg in NSGA2_LTR;
do
    for problem in RNA;
    do
        # beforeTime=`date +%s`
        # ./EMOC -a $alg -p $problem -M 2 -D 12 -N 100 -e 25000 -r 1 -i 15 -W -6,0 -q 50 -R "((((...))))." -I 1 &
        # afterTime=`date +%s`
        # echo "finished in $(($afterTime-$beforeTime)) seconds."
        # ./EMOC -a $alg -p $problem -M 2 -D 16 -N 100 -e 25000 -r 1 -i 15 -W -10,0 -q 50 -R "(((((......)))))" -I 2 &

        # ./EMOC -a $alg -p $problem -M 2 -D 17 -N 100 -e 25000 -r 1 -i 15 -W -4,0 -q 50 -R "((....)).((....))" -I 3 &
        
        # ./EMOC -a $alg -p $problem -M 2 -D 26 -N 100 -e 25000 -r 1 -i 15 -W -25,0 -q 50 -R "..((((((((.....)).)))))).." -I 4 &

        # ./EMOC -a $alg -p $problem -M 2 -D 30 -N 100 -e 25000 -r 1 -i 15 -W -20,0 -q 50 -R "(((((.....))..((.........)))))" -I 5 &

        # ./EMOC -a $alg -p $problem -M 2 -D 31 -N 100 -e 25000 -r 1 -i 15 -W -23,0 -q 50 -R ".(((((((((((...)))))....))))))." -I 6 &

        # ./EMOC -a $alg -p $problem -M 2 -D 34 -N 100 -e 25000 -r 1 -i 15 -W -24,0 -q 50 -R "....((((((((.(....)).).).)))))...." -I 7 &

        # ./EMOC -a $alg -p $problem -M 2 -D 35 -N 100 -e 25000 -r 1 -i 15 -W -18,0 -q 50 -R "((....)).((....)).((....)).((....))" -I 8 &

        # ./EMOC -a $alg -p $problem -M 2 -D 36 -N 100 -e 25000 -r 1 -i 15 -W -32,0 -q 50 -R "((((((.((((....))))))).))).........." -I 9 &

        ./EMOC -a $alg -p $problem -M 2 -D 36 -N 100 -e 25000 -r 1 -i 15 -W -40,0 -q 50 -R "((((((.((((((((....))))).)).).))))))" -I 10 &

    done
done