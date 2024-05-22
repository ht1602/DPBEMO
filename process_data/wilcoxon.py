import scipy.stats as stats
import numpy as np
performance = "ExpectDis"
problem = "RNA_2"
# algorithm1 = "DPBEMO_NSGA2"
algorithm1 = "DPBEMO_MOEAD"
peers = ["MOEADPLVF", "NSGA2_LTR", "IEMOD"]
# peers = ["DPBEMO_DTS", "MOEADPBO"]
peer_id = 2
# epsilon = ['1e-1', '1e-3', '1e-6']
# K = [2, 5, 10]
# winner_id = 0
# loser_id = 2
m = 2
d = 16
algorithm2 = peers[peer_id]
if problem.startswith("ZDT"):
    m = 2
    if problem == "ZDT1" or problem == "ZDT2" or problem == "ZDT3":
        d = 30
    else:
        d = 10
if problem.startswith("WFG"):
    m = 3
    d = 64
if problem.startswith("DTLZ"):
    if problem == "DTLZ1":
        d = m + 4
    else:
        d = m + 9
file_name1 = f"./{performance}/{algorithm1}/{problem}_M{m}_D{d}.txt"
file_name2 = f"./{performance}/{algorithm2}/{problem}_M{m}_D{d}.txt"
x = np.loadtxt(file_name1)
y = np.loadtxt(file_name2)
x = x[:10]
y = y[:10]
print(x)
print(y)

result = stats.wilcoxon(x,y,correction=False, alternative = 'two-sided')
print(result)