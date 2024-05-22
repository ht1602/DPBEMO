import pandas as pd
# algorithm = ['DPBEMO_NSGA2']
algorithm = ["DPBEMO_NSGA2", "DPBEMO_MOEAD", "MOEADPLVF", "NSGA2_LTR","IEMOD"]
# algorithm = ["DPBEMO_NSGA2", "DPBEMO_MOEAD", "DPBEMO_R2_IBEA", "DPBEMO_DTS", "MOEADPBO", "MOEADPLVF", "NSGA2_LTR","IEMOD"]
problem = ["RNA"]
obj_num = 2
dec_num = [12, 16, 17, 26, 30, 31, 34, 35, 36, 36]
# dec_num = 30
dir = "ExpectDis"
df = pd.DataFrame()

for alg in algorithm:
    # col_data = pd.DataFrame(columns = [f"{alg}_min",f"{alg}_var"])
    col_data = pd.DataFrame(columns=[f"{alg}_mean_var"])
    for prob in problem:
        for i in range(1, 11):
            txt_file = f"./{dir}/{alg}/{prob}_{i}_M2_D{dec_num[i - 1]}.txt"
            min_list = pd.read_csv(txt_file, names=[f"{alg}_min"])
            print(min_list)
            mean = pd.DataFrame(data = [min_list.mean(0)], index = [f"{prob}{i}"], columns=[f"{alg}_min"])
            var = pd.DataFrame(data = [min_list.var(0)], index=[f"{prob}{i}"], columns=[f"{alg}_min"])
            concat_data = pd.DataFrame(data=[f"{min_list.mean(0)}({min_list.var(0)})"], index=[f"{prob}{i}"], columns=[f"{alg}_min"])
            mean = mean.round(3)
            var = var.round(2)
            print(mean)
            print(var)
        
            # print(concat_data)
            combine = pd.DataFrame()
            combine[f"{alg}_mean_var"] = mean[f"{alg}_min"].astype(str) + '(' + var[f"{alg}_min"].astype(str) + ')'
            # combine.columns = [f"{alg}_mean_var"]
            # var_min = pd.concat([mean, var], axis=1)
            # var_min = pd.concat([var_min, combine], axis = 1)
            # var_min.columns = [f"{alg}_min", f"{alg}_var", f"{alg}_mean_var"]
            col_data = pd.concat([col_data, combine], axis=0)
            
            # col_data[f'{alg}_mean_var'] = col_data[f'{alg}_min'].astype(str) + '(' + col_data[f"{alg}_var"].astype(str) +')'
            # var_min = pd.concat([])
            # col_data[f'{alg}_mean_var'] = mean[f"{alg}_min"].astype(str) + '(' + var[f"{alg}_min"].astype(str) + ')'
            
        
    
    df = pd.concat([df,col_data],axis=1)
    # data.insert(loc=1,column="alg",value=f"{alg}")
    
    
    
    
    
    # df.concat([df,data],axis=0,sort=False)
print(df)
csv_file_path = f"./process_data/{dir}_RNA.csv"

# df = pd.read_csv(txt_file, header = 0 , sep = "\t")

df.to_csv(csv_file_path, index = False)

