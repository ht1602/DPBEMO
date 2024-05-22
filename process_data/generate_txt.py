import pandas as pd

algs = ['DPBEMO_NSGA2', 'DPBEMO_MOEAD', 'DPBEMO_R2_IBEA','DPBEMO_DTS', 'MOEADPLVF', 'NSGA2_LTR', 'IEMOD']
# problems = ['ZDT1', 'ZDT2', 'ZDT3', 'ZDT4', 'ZDT6']
# problems = ['DTLZ1', 'DTLZ2', 'DTLZ3', 'DTLZ4', 'DTLZ5', 'DTLZ6']
problems = ['WFG1', 'WFG3', 'WFG5', 'WFG7']
dir_path = './MinDis/'
M = 3
for prob in problems:
    txt = pd.DataFrame()
    for alg in algs:
        file_path = dir_path + f'{alg}/'
        if prob == "ZDT1" or prob == "ZDT2" or prob == "ZDT3" :
            M = 2
            file_path = file_path + f'{prob}_M2_D30.txt'
            data = pd.read_csv(file_path, header=None, nrows=20).transpose()
            data.index = [f'{alg}']
            txt = pd.concat([txt, data])
            print(txt)
        if prob == "ZDT4" or prob == "ZDT6":
            M = 2
            file_path = file_path + f'{prob}_M2_D10.txt'
            data = pd.read_csv(file_path, header=None, nrows=20).transpose()
            data.index = [f'{alg}']
            txt = pd.concat([txt, data])
            print(txt)
        if prob.startswith('DTLZ'):
                if prob == 'DTLZ1':
                    file_path = dir_path + f'{alg}/' + f'{prob}_M{M}_D{M+4}.txt'
                else:
                    file_path = dir_path + f'{alg}/' + f'{prob}_M{M}_D{M+9}.txt'
                data = pd.read_csv(file_path, header=None, nrows=20).transpose()
                data.index = [f'{alg}']
                txt = pd.concat([txt, data])
                print(txt)
        if prob.startswith('WFG'):
            file_path = file_path + f'{prob}_M{M}_D64.txt'
            data = pd.read_csv(file_path, header=None, nrows=20).transpose()
            data.index = [f'{alg}']
            txt = pd.concat([txt, data])
            print(txt)
    txt.to_csv(f"./process_data/MinDis/{prob}_M{M}.txt", sep='\t', header=None)