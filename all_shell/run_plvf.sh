#! /bin/bash

for alg in MOEADPLVF;
do
	for problem in 5 6 ;
	do
		if [ "$problem" == "1" ];then
			./EMOC -a $alg -p DTLZ$problem -M 3 -D 7 -N 64 -e 35200 -r 20 -W 0.3,0.3,0.2 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 3 -D 7 -N 64 -e 35200 -r 20 -W 0.05,0.05,0.2 -q 25&
			./EMOC -a $alg -p DTLZ$problem -M 5 -D 9 -N 128 -e 83200 -r 20 -W 0.2,0.1,0.1,0.3,0.4 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 5 -D 9 -N 128 -e 83200 -r 20 -W  0.05,0.05,0.1,0.08,0.03 -q 25&
			./EMOC -a $alg -p DTLZ$problem -M 8 -D 12 -N 224 -e 179200 -r 20 -W 0.1,0.2,0.1,0.4,0.4,0.1,0.3,0.1 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 8 -D 12 -N 224 -e 179200 -r 20 -W 0.01,0.02,0.07,0.02,0.06,0.2,0.1,0.01 -q 25&
			./EMOC -a $alg -p DTLZ$problem -M 10 -D 14 -N 288 -e 259200 -r 20 -W  0.05,0.1,0.1,0.05,0.1,0.2,0.08,0.03,0.3,0.1 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 10 -D 14 -N 288 -e 259200 -r 20 -W  0.02,0.01,0.06,0.04,0.01,0.02,0.03,0.05,0.08 -q 25&

		elif [ "$problem" == "2" ];then
			# ./EMOC -a $alg -p DTLZ$problem -M 3 -D 12 -N 64 -e 16000 -r 20 -W 0.7,0.8,0.5 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 3 -D 12 -N 64 -e 16000 -r 20 -W 0.2,0.5,0.6 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 5 -D 14 -N 128 -e 44800 -r 20 -W 0.7,0.6,0.3,0.8,0.5 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 5 -D 14 -N 128 -e 44800 -r 20 -W 0.3,0.1,0.4,0.2,0.3 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 8 -D 17 -N 224 -e 112000 -r 20 -W 0.6,0.5,0.75,0.2,0.3,0.55,0.7,0.6 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 8 -D 17 -N 224 -e 112000 -r 20 -W 0.3,0.1,0.4,0.25,0.1,0.15,0.4,0.25 -q 25&
			./EMOC -a $alg -p DTLZ$problem -M 10 -D 19 -N 288 -e 172800 -r 20 -W 0.3,0.3,0.3,0.1,0.3,0.55,0.35,0.35,0.25,0.45 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 10 -D 19 -N 288 -e 172800 -r 20 -W 0.1,0.1,0.3,0.4,0.2,0.5,0.25,0.15,0.1,0.4 -q 25&
		elif [ "$problem" == "3" ];then
			# ./EMOC -a $alg -p DTLZ$problem -M 3 -D 12 -N 64 -e 67200 -r 20 -W 0.7,0.8,0.5 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 3 -D 12 -N 64 -e 67200 -r 20 -W 0.2,0.5,0.6 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 5 -D 14 -N 128 -e 147200 -r 20 -W 0.7,0.6,0.3,0.8,0.5 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 5 -D 14 -N 128 -e 147200 -r 20 -W 0.3,0.1,0.4,0.2,0.3 -q 25&
			./EMOC -a $alg -p DTLZ$problem -M 8 -D 17 -N 224 -e 291200 -r 20 -W 0.6,0.5,0.75,0.2,0.3,0.55,0.7,0.6 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 8 -D 17 -N 224 -e 291200 -r 20 -W 0.3,0.1,0.4,0.25,0.1,0.15,0.4,0.25 -q 25&
			./EMOC -a $alg -p DTLZ$problem -M 10 -D 19 -N 288 -e 403200 -r 20 -W 0.3,0.3,0.3,0.1,0.3,0.55,0.35,0.35,0.25,0.45 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 10 -D 19 -N 288 -e 403200 -r 20 -W 0.1,0.1,0.3,0.4,0.2,0.5,0.25,0.15,0.1,0.4 -q 25&
		elif [ "$problem" == "4" ];then
			# ./EMOC -a $alg -p DTLZ$problem -M 3 -D 12 -N 64 -e 16000 -r 20 -W 0.7,0.8,0.5 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 3 -D 12 -N 64 -e 16000 -r 20 -W 0.2,0.5,0.6 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 5 -D 14 -N 128 -e 44800 -r 20 -W 0.7,0.6,0.3,0.8,0.5 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 5 -D 14 -N 128 -e 44800 -r 20 -W 0.3,0.1,0.4,0.2,0.3 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 8 -D 17 -N 224 -e 112000 -r 20 -W 0.6,0.5,0.75,0.2,0.3,0.55,0.7,0.6 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 8 -D 17 -N 224 -e 112000 -r 20 -W 0.3,0.1,0.4,0.25,0.1,0.15,0.4,0.25 -q 25&
			./EMOC -a $alg -p DTLZ$problem -M 10 -D 19 -N 288 -e 172800 -r 20 -W 0.3,0.3,0.3,0.1,0.3,0.55,0.35,0.35,0.25,0.45 -q 25&
			# ./EMOC -a $alg -p DTLZ$problem -M 10 -D 19 -N 288 -e 172800 -r 20 -W 0.1,0.1,0.3,0.4,0.2,0.5,0.25,0.15,0.1,0.4 -q 25&
		elif [ "$problem" == "5" ];then
			./EMOC -a $alg -p DTLZ$problem -M 3 -D 12 -N 64 -e 16000 -r 20 -W 0.2,0.3,0.6 -q 25&
			
			./EMOC -a $alg -p DTLZ$problem -M 5 -D 14 -N 128 -e 44800 -r 20 -W 0.12,0.12,0.17,0.24,0.7 -q 25&
			
			./EMOC -a $alg -p DTLZ$problem -M 8 -D 17 -N 224 -e 112000 -r 20 -W 0.04,0.04,0.0566,0.8,0.113,0.16,0.2263,0.68 -q 25&
		
			./EMOC -a $alg -p DTLZ$problem -M 10 -D 19 -N 288 -e 172800 -r 20 -W  0,0,0,0,0.0096,0.027,0.082,0.25,0.75,0.08 -q 25&
		
		elif [ "$problem" == "6" ];then
			./EMOC -a $alg -p DTLZ$problem -M 3 -D 12 -N 64 -e 16000 -r 20 -W 0.2,0.3,0.6 -q 25&
			
			./EMOC -a $alg -p DTLZ$problem -M 5 -D 14 -N 128 -e 44800 -r 20 -W 0.12,0.12,0.17,0.24,0.7 -q 25&
			
			./EMOC -a $alg -p DTLZ$problem -M 8 -D 17 -N 224 -e 112000 -r 20 -W 0.04,0.04,0.0566,0.8,0.113,0.16,0.2263,0.68 -q 25&
		
			./EMOC -a $alg -p DTLZ$problem -M 10 -D 19 -N 288 -e 172800 -r 20 -W  0,0,0,0,0.0096,0.027,0.082,0.25,0.75,0.08 -q 25&
					
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
