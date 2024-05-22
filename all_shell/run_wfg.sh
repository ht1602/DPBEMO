#! /bin/bash

for alg in DPBEMO_NSGA2 DPBEMO_MOEAD DPBEMO_R2_IBEA DPBEMO_DTS MOEADPLVF NSGA2_LTR IEMOD;
do
	for problem in 1 3 5 7;
	do
		if [ "$problem" == "1" ];then
			./EMOC -a $alg -p WFG$problem -M 3 -D 64 -N 64 -e 67200 -r 20 -W 0.2,0.5,0.6 -q 60&
			# ./EMOC -a $alg -p WFG$problem -M 5 -D 28 -N 128 -e 128000 -r 20 -W 0.3,0.1,0.4,0.2,0.3 -q 25&
			# ./EMOC -a $alg -p WFG$problem -M 8 -D 34 -N 224 -e 268800 -r 20 -W 0.01,0.02,0.03,0.01,0.01,0.05,0.01,0.01 -q 25&
			# ./EMOC -a $alg -p WFG$problem -M 10 -D 38 -N 288 -e 432000 -r 20 -W 0.1,0.1,0.3,0.4,0.2,0.5,0.25,0.15,0.1,0.4 -q 25&
		elif [ "$problem" == "3" ];then
			./EMOC -a $alg -p WFG$problem -M 3 -D 64 -N 64 -e 67200 -r 20 -W 0.6,0.8,0.8 -q 60&
			# ./EMOC -a $alg -p WFG$problem -M 5 -D 28 -N 128 -e 128000 -r 20 -W 0.4,0.4,0.5,0.4,0.4 -q 25&
			# ./EMOC -a $alg -p WFG$problem -M 8 -D 34 -N 224 -e 268800 -r 20 -W 0.4,0.4,0.5,0.4,0.4,0.5,0.6,0.5 -q 25&
			# ./EMOC -a $alg -p WFG$problem -M 10 -D 38 -N 288 -e 432000 -r 20 -W 0.5,0.2,0.4,0.3,0.4,0.5,0.6,0.7,0.6,0.7 -q 25&
		elif [ "$problem" == "5" ];then
		./EMOC -a $alg -p WFG$problem -M 3 -D 64 -N 64 -e 67200 -r 20 -W 0.3,0.7,0.3 -q 60&
			# ./EMOC -a $alg -p WFG$problem -M 5 -D 28 -N 128 -e 128000 -r 20 -W 0.6,0.3,0.3,0.3,0.3 -q 25&
			# ./EMOC -a $alg -p WFG$problem -M 8 -D 34 -N 224 -e 268800 -r 20 -W 0.2,0.3,0.3,0.3,0.3,0.2,0.1,0.2 -q 25&
			# ./EMOC -a $alg -p WFG$problem -M 10 -D 38 -N 288 -e 432000 -r 20 -W 0.2,0.15,0.15,0.2,0.1,0.15,0.05,0.1,0.1,0.2 -q 25&
		elif [ "$problem" == "7" ];then
			./EMOC -a $alg -p WFG$problem -M 3 -D 64 -N 64 -e 67200 -r 20 -W 0.7,0.4,0.4 -q 60&
			# ./EMOC -a $alg -p WFG$problem -M 5 -D 28 -N 128 -e 128000 -r 20 -W 0.6,0.3,0.3,0.3,0.2 -q 25&
			# ./EMOC -a $alg -p WFG$problem -M 8 -D 34 -N 224 -e 268800 -r 20 -W 0.08,0.15,0.1,0.1,0.12,0.1,0.1,0.1 -q 25&
			# ./EMOC -a $alg -p WFG$problem -M 10 -D 38 -N 288 -e 432000 -r 20 -W 0.05,0.05,0.1,0.1,0.05,0.08,0.05,0.05,0.03,0.05 -q 25&
		fi
	done
done

# for alg in MOEADPLVF;
# do
# 	for problem in 1 2 3 4 6;
# 	do
# 		if [ "$problem" == "1" ];then
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.3,0.4 -q 25&
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.65,0.3 -q 25&
# 		elif [ "$problem" == "2" ];then
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.2,0.8 -q 25&
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.9,0.4 -q 25&
# 		elif [ "$problem" == "3" ];then
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.15,0.4 -q 25&
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.4,0.0 -q 25&
# 		elif [ "$problem" == "4" ];then
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 10 -N 100 -e 25000 -r 20 -W 0.3,0.4 -q 25&
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 10 -N 100 -e 25000 -r 20 -W 0.65,0.3 -q 25&
# 		elif [ "$problem" == "6" ];then
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 10 -N 100 -e 25000 -r 20 -W 0.9,0.3 -q 25&
# 			./EMOC -a $alg -p ZDT$problem -M 2 -D 10 -N 100 -e 25000 -r 20 -W 0.5,0.7 -q 25&
# 		fi
# 	done
# done
