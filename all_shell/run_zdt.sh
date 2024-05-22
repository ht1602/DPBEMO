#! /bin/bash

for alg in DPBEMO_MOEAD;
do
	for problem in 1 2 3 4 6;
	do
		if [ "$problem" == "1" ];then
			./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.3,0.4 -q 25&
			# ./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.65,0.3 -q 25&
		elif [ "$problem" == "2" ];then
			./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.2,0.8 -q 25&
			# ./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.9,0.4 -q 25&
		elif [ "$problem" == "3" ];then
			./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.15,0.4 -q 25&
			# ./EMOC -a $alg -p ZDT$problem -M 2 -D 30 -N 100 -e 25000 -r 20 -W 0.4,0.0 -q 25&
		elif [ "$problem" == "4" ];then
			./EMOC -a $alg -p ZDT$problem -M 2 -D 10 -N 100 -e 25000 -r 20 -W 0.3,0.4 -q 25&
			# ./EMOC -a $alg -p ZDT$problem -M 2 -D 10 -N 100 -e 25000 -r 20 -W 0.65,0.3 -q 25&
		elif [ "$problem" == "6" ];then
			./EMOC -a $alg -p ZDT$problem -M 2 -D 10 -N 100 -e 25000 -r 20 -W 0.9,0.3 -q 25&
			# ./EMOC -a $alg -p ZDT$problem -M 2 -D 10 -N 100 -e 25000 -r 20 -W 0.5,0.7 -q 25&
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
