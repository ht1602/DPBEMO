#include "problem/rna/rna.h"

#include <cmath>

#include "core/macro.h"
#include "core/global.h"
#include "../vendor/Config/config.h"
#include <filesystem>
#include <fstream>

namespace emoc{
    RNA::RNA(int dec_num, int obj_num) :Problem(dec_num, obj_num)
    {
        // std::cout << "<info> Swimmer::Swimmer" << std::endl;
        for (int i = 0; i < dec_num; ++i)
		{
			lower_bound_[i] = 0.0;
			upper_bound_[i] = 3.0;
		}
        encoding_ = PERMUTATION;
        // init();
    }

    RNA::~RNA()
    {
        S = nullptr;
        target_structure = nullptr;
        delete[] S;
        delete[] target_structure;

    }

    void RNA::CalObj(Individual *ind)
    {
        target_structure = RNA_structure;
        std::cout << "target structure is " << target_structure << std::endl;
        projection(ind);

        Py_Initialize();
        if(!Py_IsInitialized())
        {
            printf("Python init failed!\n");
        }

        // std::cout << "<info> Swimmer::CalObj" << std::endl;
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('./')");

        wchar_t * s2[] = { L" " }; // 宽字符，长度为2字节
        PySys_SetArgv(1, s2);   // TODO: 加入argv参数 否则出错.PyAPI_FUNC(void) PySys_SetArgv(int, wchar_t **);
        //导入python文件
        PyObject *pModule = PyImport_ImportModule("RNA_fold");
        if (!pModule)
        {
            std::cerr << "[ERROR] Can not open python file MuJoCo.py!"<<std::endl;
            return;
        }
        PyObject *RNA_inverse = NULL;
        if(obj_num_ ==2)
            RNA_inverse = PyObject_GetAttrString(pModule, "RNA_inverse");
        else
        {
            std::cerr<<"<error> [SWIMMER::CalObj] wrong obj number!"<<std::endl;
            exit(-1);
        }
        PyObject *pArg = PyTuple_New(2);
        // PyTuple_SetItem(pArg, 0, Py_BuildValue("s", "UCGUCGUCGUUAU"));
        // PyTuple_SetItem(pArg, 1, Py_BuildValue("s", "((...))(....)"));
        PyTuple_SetItem(pArg, 0, Py_BuildValue("s", S));
        PyTuple_SetItem(pArg, 1, Py_BuildValue("s", target_structure));
        
        
        PyObject *pRet = PyObject_CallObject(RNA_inverse, pArg);
        if(pRet != NULL)
        {
            int listSize = PyList_Size(pRet);
            PyObject *float_py = NULL;
            for(int i = 0; i < listSize; ++i)
            {
                float_py = PyList_GetItem(pRet, i);
                ind->obj_[i] = PyFloat_AsDouble(float_py);
            }
            Py_DECREF(float_py);
        }
        else
        {
            std::cerr<<"[ERROR] Function excuted unsuccessfully."<<std::endl;
            return;
        }
        
        Py_DECREF(pArg);
        Py_DECREF(pRet);
        Py_DECREF(RNA_inverse);
        Py_DECREF(pModule);

        return;
    }

    void RNA::projection(Individual* ind)
    {
        
        // project to RNA sequences
        S = new char[dec_num_ + 1];
        S[dec_num_] = '\0';
        for(int i = 0; i < dec_num_; i++)
        {
            // int X = floor(ind->dec_[i]);
            int X = round(ind->dec_[i]);
            std::cout << ind->dec_[i] << " ";
            // std::cout << "AA" << i << ":" << X << std::endl;
            switch(X)
            {
                case 0: 
                    S[i] = 'A'; 
                    break;
                case 1: 
                    S[i] = 'G'; 
                    break;
                case 2:
                    S[i] = 'C';
                    break;
                case 3:
                    S[i] = 'U';
                    break;
                default:
                    break;
            }
            
        }
        std::cout << '\n';
        std::cout << S << "\n";   
        // std::cout << sequence << "\n";   
    }


    void RNA::init()
    {
        // get RNA name
        std::string strConfigFileName("config.ini");
		Config config(strConfigFileName);
        std::string structure_input;
        structure_input = config.Read("RNA_structure", structure_input);
        // target_structure = config.Read("RNA_structure", target_structure);
        target_structure = new char[dec_num_ + 1];
        strcpy(target_structure, structure_input.c_str());
        std::cout << target_structure << std::endl;
        
    }
}