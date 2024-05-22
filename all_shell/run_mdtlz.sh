#! /bin/bash

for alg in MOEADPLVF NSGA2_LTR MOEAD_LTR R2_IBEA_LTR IEMOD MOEADPPL;
do
	for problem in 1 2 3 4;
	do
		if [ "$problem" == "1" ];then
			./EMOC -a $alg -p MDTLZ$problem -M 3 -D 10 -N 300 -e 90000 -r 20 -W 0.4,0.4,0.4 -q 25&
		else
			./EMOC -a $alg -p MDTLZ$problem -M 3 -D 12 -N 64 -e 16000 -r 20 -W 0.5,0.4,0.4 -q 25&
		fi
	done
    
done

