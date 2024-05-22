#include "problem/antigen/antigen.h"

#include <cmath>
#include <iostream>


#include "core/macro.h"
#include "core/global.h"
#include "../vendor/Config/config.h"
#include "cxxopts/cxxopts.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>




namespace emoc {
    Antigen::Antigen(int dec_num, int obj_num):
    Problem(dec_num, obj_num)
    {
        for (int i = 0; i < dec_num; ++i)
		{
			lower_bound_[i] = 0.0;
			upper_bound_[i] = 20.0;
		}
        encoding_ = PERMUTATION;
        init();
    }

    Antigen::~Antigen()
    {

    }
    
    void Antigen::CalObj(Individual* ind)
    {
        // project to AAs
        projection(ind);
        
        chdir(tool.c_str());
        // printf("current working directory: %s\n", getcwd(NULL, NULL));

        // Absolut! energy and affinity
        // energy: binding energy
        // affinity: stastical energy
        char command_energy[1024];
        sprintf(command_energy, "./src/bin/Absolut repertoire %s TempCDR3_%s.txt", 
        protein_name.c_str(), 
        protein_name.c_str()
        );
        system(command_energy);
        std::cout << "finish Absolut!" << "\n";

        
        std::ifstream resultf(result_file);
        std::cout << "resultf load succ\n";
        if(!resultf)
		{
			std::cout <<"Can not open "<< result_file << " !" << std::endl;
			std::cout << "Press enter to exit" << std::endl;
			std::cout << strerror(errno) << "\n";
			std::cin.get();
			exit(-1);
		}
        else
            std::cout << "result file exists\n";

        std::string lines;
        std::string pattern = {'\t'};
        // std::vector<std::string> result;
        while(getline(resultf, lines))
        {
            // skip first line
            if(lines[0] == '#' || lines[0] == 'I')
                continue;
            std::vector<std::string> result;
            printf("%s\n", lines.c_str());
            result = split(lines, pattern);
            std::cout << "split succ\n";
            ind->obj_[0] = std::stod(result[4]);
            ind->obj_[1] = std::stod(result[6]);
            // auto index = find(result.begin(), result.end(), "true");
            // if(find(result.begin(), result.end(), "true") != result.end())
            // {
            //     ind->obj_[0] = std::stod(result[4]);
            //     ind->obj_[1] = std::stod(result[6]);
            // }
        }
        
        std::cout << "first objective " << ind->obj_[0] << "\n";
        std::cout << "second objective " << ind->obj_[1] << "\n";

        // Absolut! affinity
        // ind->obj_[1] = 0;
        ind->obj_[0] += 110;
        ind->obj_[1] += 110;

        resultf.close();
        chdir(curr_dir);
        // printf("current working directory: %s\n", getcwd(NULL, NULL));
        
    }

    void Antigen::init()
    {
        // get protein name
        std::string strConfigFileName("config.ini");
		Config config(strConfigFileName);
        protein_name = config.Read("Protein_name", protein_name);
        tool = config.Read("tool", tool);
        std::cout << protein_name << std::endl;

        sprintf(temp_file, "%s/TempCDR3_%s.txt", tool.c_str(), protein_name.c_str());
        sprintf(result_file, "%sFinalBindings_Process_1_Of_1.txt", protein_name.c_str());

        // record current dir
        getcwd(curr_dir, 256);
        // std::cout << curr_dir << "\n";
        
    }

    void Antigen::projection(Individual* ind)
    {
        // open the file to record CDR
        
        FILE *fpt = fopen(temp_file, "w");
		
		if(fpt == nullptr)
		{
			std::cout <<"Can not open "<< temp_file << " !" << std::endl;
			std::cout << "Press enter to exit" << std::endl;
			std::cout << strerror(errno) << "\n";
			std::cin.get();
			exit(-1);
		}

        // project to AAs and record in Absolut path
        char AA[dec_num_ + 1];
        AA[dec_num_] = '\0';
        for(int i = 0; i < dec_num_; i++)
        {
            // int X = floor(ind->dec_[i]);
            int X = ind->dec_[i];
            std::cout << ind->dec_[i] << " ";
            // std::cout << "AA" << i << ":" << X << std::endl;
            switch(X)
            {
                case 0: 
                    AA[i] = 'A'; 
                    break;
                case 1: 
                    AA[i] = 'R'; 
                    break;
                case 2:
                    AA[i] = 'N';
                    break;
                case 3:
                    AA[i] = 'D';
                    break;
                case 4:
                    AA[i] = 'C';
                    break;
                case 5:
                    AA[i] = 'Q';
                    break;
                case 6:
                    AA[i] = 'E';
                    break;
                case 7:
                    AA[i] = 'G';
                    break;
                case 8:
                    AA[i] = 'H';
                    break;
                case 9: 
                    AA[i] = 'I';
                    break;
                case 10:
                    AA[i] = 'L';
                    break;
                case 11:
                    AA[i] = 'K';
                    break;
                case 12:
                    AA[i] = 'M';
                    break;
                case 13:
                    AA[i] = 'F';
                    break;
                case 14:
                    AA[i] = 'P';
                    break;
                case 15:
                    AA[i] = 'S';
                    break;
                case 16:
                    AA[i] = 'T';
                    break;
                case 17:
                    AA[i] = 'W';
                    break;
                case 18:
                    AA[i] = 'Y';
                    break;
                case 19:
                    AA[i] = 'V';
                    break;
                default:
                    break;
            }
            
        }
        std::cout << '\n';
        std::cout << AA << "\n";      

        fprintf(fpt, "1\t%s\n", AA);
		fflush(fpt);
		fclose(fpt);
    }

    //split string
    std::vector<std::string> Antigen::split(std::string& str, std::string pattern)
    {
        std::string::size_type pos;
        std::vector<std::string> output;
        std::string cur_str = str + pattern;//扩展字符串以方便操作
        // str += pattern;//扩展字符串以方便操作
        int size = cur_str.size();
        for (int i = 0; i < size; i++)
        {
            pos = cur_str.find(pattern, i);
            if (pos < size)
            {
                std::string s = cur_str.substr(i, pos - i);
                output.push_back(s);
                i = pos + pattern.size() - 1;
            }
        }
        return output;
    }



}