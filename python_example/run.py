import EMOC
import numpy as np
import os
import json
import argparse
from utils.LogUtils import *
from search.PCM import *
from utils.ProteinUtils import *
import time
from energy.energy_nofile import *


class MyUF1(EMOC.Problem):
    def __init__(self, dec_num, obj_num):
        super(MyUF1, self).__init__(dec_num,obj_num)
        lower_bound = [-1] * dec_num
        lower_bound[0] = 0
        self.lower_bound = lower_bound
        self.upper_bound = [1] * dec_num

    def CalObj(self, ind):
        x = ind.dec
        temp_obj = [0] * self.obj_num
        # print("here\n")
        sum1 = 0
        count1 = 0
        sum2 = 0
        count2 = 0
        for i in range(2,self.dec_num+1):
            yj = x[i-1] - np.sin(6.0 * np.pi * x[0] + i * np.pi / self.dec_num)
            yj = yj * yj
            if i % 2 == 0:
                sum2 += yj
                count2 = count2 + 1
            else:
                sum1 += yj
                count1 += 1
        temp_obj[0] = x[0] + 2.0 * sum1 / count1
        temp_obj[1] = 1.0 - np.sqrt(x[0]) + 2.0 * sum2 / count2
        ind.obj = temp_obj

class PSP(EMOC.Problem):
    def __init__(self, dec_num, obj_num, config, energy_config, protein_config, protein_status, coder):
        super(PSP, self).__init__(dec_num, obj_num)
        # init constraint of each decision num
        lower_bound = [-180] * dec_num
        lower_bound[0] = 0
        self.lower_bound = lower_bound
        self.upper_bound = [180] * dec_num
        self.config = config
        self.energy_config = energy_config
        self.protein_config = protein_config
        self.protein_status = protein_status
        self.coder = coder
        # self.seq_save_path = []
        # self.protein_name = []
        

    def init_dec_protein(self, x):
        # init decision variable to protein structure
        protein = x
        self.protein_name = config['protein_params']['name']
        self.seq_save_path = config['protein_params']['second_struct_file']
        # data_path = os.path.join(protein_save_path, protein_name + '_' + str(x))
        protein = Protein(self.obj_num, protein_status, coder)
        return protein

    
    def CalObj(self, ind):
        x = ind.dec
        protein = []
        new_protein = self.init_dec_protein(x)
        protein.append(coder.decoder_from_dec(self.seq_save_path, x, new_protein))
        # print(protein[0].res.angle)
        print(protein)
        # TODO
        config = json.loads(config_file)
        protein_config = json.loads(protein_config_file)
        energy_config = json.loads(energy_config_file)

        root = config['paths']['root']
        second_struct_file_path = config['protein_params']['second_struct_file']
        max_thread = config['energy_params']['max_thread']
        print(energy_config)
        energy = Energy(energy_config, root, energy_temp_save_path, protein_name, second_struct_file_path, max_thread, protein[0])
        energy.calculate_energy(protein)
        energy.stop()
        ind.obj = protein[0].obj
        
class PSP_1ZDD(EMOC.Problem):
    def __init__(self, dec_num, obj_num, config, energy_config, protein_config, protein_status, coder):
        super(PSP_1ZDD, self).__init__(dec_num, obj_num)
        # init constraint of each decision num
        lower_bound = [-180] * dec_num
        lower_bound[0] = 0
        self.lower_bound = lower_bound
        self.upper_bound = [180] * dec_num
        self.config = config
        self.energy_config = energy_config
        self.protein_config = protein_config
        self.protein_status = protein_status
        self.coder = coder
        # self.seq_save_path = []
        # self.protein_name = []
        

    def init_dec_protein(self, x):
        # init decision variable to protein structure
        protein = x
        self.protein_name = config['protein_params']['name']
        self.seq_save_path = config['protein_params']['second_struct_file']
        # data_path = os.path.join(protein_save_path, protein_name + '_' + str(x))
        protein = Protein(self.obj_num, protein_status, coder)
        return protein

    
    def CalObj(self, ind):
        x = ind.dec
        protein = []
        new_protein = self.init_dec_protein(x)
        protein.append(coder.decoder_from_dec(self.seq_save_path, x, new_protein))
        # print(protein[0].res.angle)
        print(protein)
        # TODO
        config = json.loads(config_file)
        protein_config = json.loads(protein_config_file)
        energy_config = json.loads(energy_config_file)

        root = config['paths']['root']
        second_struct_file_path = config['protein_params']['second_struct_file']
        max_thread = config['energy_params']['max_thread']
        print(energy_config)
        energy = Energy(energy_config, root, energy_temp_save_path, protein_name, second_struct_file_path, max_thread, protein[0])
        energy.calculate_energy(protein)
        energy.stop()
        ind.obj = protein[0].obj
        
class PSP_1K36(EMOC.Problem):
    def __init__(self, dec_num, obj_num, config, energy_config, protein_config, protein_status, coder):
        super(PSP_1K36, self).__init__(dec_num, obj_num)
        # init constraint of each decision num
        lower_bound = [-180] * dec_num
        lower_bound[0] = 0
        self.lower_bound = lower_bound
        self.upper_bound = [180] * dec_num
        self.config = config
        self.energy_config = energy_config
        self.protein_config = protein_config
        self.protein_status = protein_status
        self.coder = coder
        # self.seq_save_path = []
        # self.protein_name = []
        

    def init_dec_protein(self, x):
        # init decision variable to protein structure
        protein = x
        self.protein_name = config['protein_params']['name']
        self.seq_save_path = config['protein_params']['second_struct_file']
        # data_path = os.path.join(protein_save_path, protein_name + '_' + str(x))
        protein = Protein(self.obj_num, protein_status, coder)
        return protein

    
    def CalObj(self, ind):
        x = ind.dec
        protein = []
        new_protein = self.init_dec_protein(x)
        protein.append(coder.decoder_from_dec(self.seq_save_path, x, new_protein))
        # print(protein[0].res.angle)
        print(protein)
        # TODO
        config = json.loads(config_file)
        protein_config = json.loads(protein_config_file)
        energy_config = json.loads(energy_config_file)

        root = config['paths']['root']
        second_struct_file_path = config['protein_params']['second_struct_file']
        max_thread = config['energy_params']['max_thread']
        print(energy_config)
        energy = Energy(energy_config, root, energy_temp_save_path, protein_name, second_struct_file_path, max_thread, protein[0])
        energy.calculate_energy(protein)
        energy.stop()
        ind.obj = protein[0].obj
    
class PSP_2M7T(EMOC.Problem):
    def __init__(self, dec_num, obj_num, config, energy_config, protein_config, protein_status, coder):
        super(PSP_2M7T, self).__init__(dec_num, obj_num)
        # init constraint of each decision num
        lower_bound = [-180] * dec_num
        lower_bound[0] = 0
        self.lower_bound = lower_bound
        self.upper_bound = [180] * dec_num
        self.config = config
        self.energy_config = energy_config
        self.protein_config = protein_config
        self.protein_status = protein_status
        self.coder = coder
        # self.seq_save_path = []
        # self.protein_name = []
        

    def init_dec_protein(self, x):
        # init decision variable to protein structure
        protein = x
        self.protein_name = config['protein_params']['name']
        self.seq_save_path = config['protein_params']['second_struct_file']
        # data_path = os.path.join(protein_save_path, protein_name + '_' + str(x))
        protein = Protein(self.obj_num, protein_status, coder)
        return protein

    
    def CalObj(self, ind):
        x = ind.dec
        protein = []
        new_protein = self.init_dec_protein(x)
        protein.append(coder.decoder_from_dec(self.seq_save_path, x, new_protein))
        # print(protein[0].res.angle)
        print(protein)
        # TODO
        config = json.loads(config_file)
        protein_config = json.loads(protein_config_file)
        energy_config = json.loads(energy_config_file)

        root = config['paths']['root']
        second_struct_file_path = config['protein_params']['second_struct_file']
        max_thread = config['energy_params']['max_thread']
        print(energy_config)
        energy = Energy(energy_config, root, energy_temp_save_path, protein_name, second_struct_file_path, max_thread, protein[0])
        energy.calculate_energy(protein)
        energy.stop()
        ind.obj = protein[0].obj
        
class PSP_3P7K(EMOC.Problem):
    def __init__(self, dec_num, obj_num, config, energy_config, protein_config, protein_status, coder):
        super(PSP_3P7K, self).__init__(dec_num, obj_num)
        # init constraint of each decision num
        lower_bound = [-180] * dec_num
        lower_bound[0] = 0
        self.lower_bound = lower_bound
        self.upper_bound = [180] * dec_num
        self.config = config
        self.energy_config = energy_config
        self.protein_config = protein_config
        self.protein_status = protein_status
        self.coder = coder
        # self.seq_save_path = []
        # self.protein_name = []
        

    def init_dec_protein(self, x):
        # init decision variable to protein structure
        protein = x
        self.protein_name = config['protein_params']['name']
        self.seq_save_path = config['protein_params']['second_struct_file']
        # data_path = os.path.join(protein_save_path, protein_name + '_' + str(x))
        protein = Protein(self.obj_num, protein_status, coder)
        return protein

    
    def CalObj(self, ind):
        x = ind.dec
        protein = []
        new_protein = self.init_dec_protein(x)
        protein.append(coder.decoder_from_dec(self.seq_save_path, x, new_protein))
        # print(protein[0].res.angle)
        print(protein)
        # TODO
        config = json.loads(config_file)
        protein_config = json.loads(protein_config_file)
        energy_config = json.loads(energy_config_file)

        root = config['paths']['root']
        second_struct_file_path = config['protein_params']['second_struct_file']
        max_thread = config['energy_params']['max_thread']
        print(energy_config)
        energy = Energy(energy_config, root, energy_temp_save_path, protein_name, second_struct_file_path, max_thread, protein[0])
        energy.calculate_energy(protein)
        energy.stop()
        ind.obj = protein[0].obj

class PSP_3V1A(EMOC.Problem):
    def __init__(self, dec_num, obj_num, config, energy_config, protein_config, protein_status, coder):
        super(PSP_3V1A, self).__init__(dec_num, obj_num)
        # init constraint of each decision num
        lower_bound = [-180] * dec_num
        lower_bound[0] = 0
        self.lower_bound = lower_bound
        self.upper_bound = [180] * dec_num
        self.config = config
        self.energy_config = energy_config
        self.protein_config = protein_config
        self.protein_status = protein_status
        self.coder = coder
        # self.seq_save_path = []
        # self.protein_name = []
        

    def init_dec_protein(self, x):
        # init decision variable to protein structure
        protein = x
        self.protein_name = config['protein_params']['name']
        self.seq_save_path = config['protein_params']['second_struct_file']
        # data_path = os.path.join(protein_save_path, protein_name + '_' + str(x))
        protein = Protein(self.obj_num, protein_status, coder)
        return protein

    
    def CalObj(self, ind):
        x = ind.dec
        protein = []
        new_protein = self.init_dec_protein(x)
        protein.append(coder.decoder_from_dec(self.seq_save_path, x, new_protein))
        # print(protein[0].res.angle)
        print(protein)
        # TODO
        config = json.loads(config_file)
        protein_config = json.loads(protein_config_file)
        energy_config = json.loads(energy_config_file)

        root = config['paths']['root']
        second_struct_file_path = config['protein_params']['second_struct_file']
        max_thread = config['energy_params']['max_thread']
        print(energy_config)
        energy = Energy(energy_config, root, energy_temp_save_path, protein_name, second_struct_file_path, max_thread, protein[0])
        energy.calculate_energy(protein)
        energy.stop()
        ind.obj = protein[0].obj

# set PSP parameters
parser = argparse.ArgumentParser()


parser.add_argument('--config', type=str, default='config/config_knea.json',
                    help='the path of global config file.')
parser.add_argument('--protein_config', type=str, default='protein_config.json',
                    help='the path of protein config file.')
parser.add_argument('--energy_config', type=str, default='config/energy_config.json',
                    help='the path of energy function config file.')
parser.add_argument('--checkpoint', type=str, default=None,
                    help='the path of checkpoint and program will run checkpoint data.')
parser.add_argument('--mode', type=str, default=None)


args = parser.parse_args()
checkpoint = args.checkpoint
# set Logger and init logging save path
logging = LoggerWriter(args)
logs_root, energy_temp_save_path, protein_save_path, config_save = logging.init_path()

# read configuration file
config_file = open(os.path.join(config_save, 'config.json'), 'r').read()
config = json.loads(config_file)

protein_config_file = open(os.path.join(config_save, 'protein_config.json'), 'r').read()
protein_config = json.loads(protein_config_file)

energy_config_file = open(os.path.join(config_save, 'energy_config.json'), 'r').read()
energy_config = json.loads(energy_config_file)

# init parameters
second_struct_file_path = config['protein_params']['second_struct_file']
protein_status = config['protein_params']['status']
protein_name = config['protein_params']['name']
root = config['paths']['root']

pop_size = config['algo_params']['pop_size']
max_gen = config['algo_params']['max_gen']

num_obj = config['energy_params']['number_objective']
max_thread = config['energy_params']['max_thread']

# set coder and energy
coder = Coding(protein_config, protein_status)

proteins = []

# init protein
if checkpoint == None:
    # generate proteins
    for x in range(pop_size):
        new_protein = Protein(num_obj, protein_status, coder)
        proteins.append(coder.decoder_from_seq(second_struct_file_path, new_protein))

    # calculate energy of init proteins
    start_time = time.time()
    #energy.calculate_energy(proteins)
    #logging.write(proteins, coder, config['algo_params']['save_all'], 0)
else:
    for x in range(pop_size):
        data_path = os.path.join(protein_save_path, protein_name + '_' + str(x))
        new_protein = Protein(num_obj, protein_status, coder)
        proteins.append(coder.decoder_from_logger(data_path, new_protein))
energy = Energy(energy_config, root, energy_temp_save_path, protein_name, second_struct_file_path, max_thread, proteins[0])
energy.calculate_energy(proteins)
logging.write(proteins, coder, config['algo_params']['save_all'], 0)

end_time = time.time()
print('total time', end_time - start_time)

init_pop = []
for pro in proteins:
    for residue in pro.res:
        for ang in residue.angle:
            init_pop.append(ang)

init_pop = np.array(init_pop).reshape(len(proteins), -1)
print(init_pop)
print(init_pop.shape)
print(len(init_pop[0]))

algorithm = ['DPBEMO_NSGA2']
# algorithm = ['DPBEMO_NSGA2','MOEADPLVF', 'IEMO/D', 'MOEADPBO', 'DPBEMO_DTS', 'DPBEMO_MOEAD']
# create and set EMOC parameters
for alg in algorithm:
    para = EMOC.EMOCParameters()
    para.algorithm_name = alg
    para.population_num = pop_size
    para.objective_num = num_obj
    para.decision_num = len(init_pop[0])
    para.max_evaluation =  max_gen * pop_size
    para.output_interval = 10
    para.runs_num = 3
    if protein_name == '1ZDD':
        para.problem_name = "PSP_" + protein_name
        para.weight_DM = '297.18,-74.02,-27.73,-4604.18'
        myProblem = PSP(para.decision_num, para.objective_num, config, energy_config, protein_config, protein_status, coder)
    elif protein_name == '1K36':
        para.problem_name = "PSP_" + protein_name
        para.weight_DM = '431.51,-52.84,293.70,-5059.39'
        myProblem = PSP(para.decision_num, para.objective_num, config, energy_config, protein_config, protein_status, coder)
    elif protein_name == '2M7T':
        para.problem_name = "PSP_" + protein_name
        para.weight_DM = '269.76,-39.51,-10.82,-3313.84'
        myProblem = PSP(para.decision_num, para.objective_num, config, energy_config, protein_config, protein_status, coder)
    elif protein_name == '3P7K':
        para.problem_name = "PSP_" + protein_name
        para.weight_DM = '379.04,-104.15,-11.29,-6140.81'
        myProblem = PSP(para.decision_num, para.objective_num, config, energy_config, protein_config, protein_status, coder)
    elif protein_name == '3V1A':
        para.problem_name = "PSP_" + protein_name
        para.weight_DM = '392.22,-113.26,-69.84,-7128.48'
        myProblem = PSP(para.decision_num, para.objective_num, config, energy_config, protein_config, protein_status, coder)
    print('algorithm name', para.algorithm_name)
    print('problem_name', para.problem_name)
    print('population_num', para.population_num)
    print('max_evaluation', para.max_evaluation)
    print('objective num', para.objective_num)
    print('decision_num', para.decision_num)
    print('output_interval', para.output_interval)
    print('weight_DM', para.weight_DM)
    print('runs_num', para.runs_num)
    # para.algorithm_name = "MOEADDE"
    # para.problem_name = "UF1"
    # para.population_num = 100
    # para.decision_num = 7
    # para.objective_num = 2
    # para.max_evaluation = 25000
    # para.output_interval = 10000

    # set customized problem (optional)
    # myProblem = MyUF1(para.decision_num,para.objective_num)
    # para.SetProblem(myProblem)
    with open('./record.txt', 'a', encoding='utf-8') as w:
        w.write(time.asctime())
        w.write(para.algorithm_name + "\n")
        w.close() 
    # myProblem = PSP(para.decision_num, para.objective_num, config, energy_config, protein_config, protein_status, coder)
    para.SetProblem(myProblem)

    # set initial population
    para.SetInitialPop(init_pop)
    print("initialize successfully")

    # create EMOCManager instance and run it
    EMOCManager = EMOC.EMOCManager()
    EMOCManager.SetTaskParameters(para)
    EMOCManager.Run()
    energy.stop()
    # get the optimization results
    result = EMOCManager.GetResult()
    print("Population Number: ",result.pop_num)
    print("Population Decisions: ",result.pop_decs)
    print("Population Objectives: ",result.pop_objs)