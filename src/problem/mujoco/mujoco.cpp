#include "problem/mujoco/mujoco.h"

#include <cmath>

#include "core/macro.h"
#include "core/global.h"

namespace emoc{
    Swimmer::Swimmer(int dec_num, int obj_num) :Problem(dec_num, obj_num)
    {
        // std::cout << "<info> Swimmer::Swimmer" << std::endl;
        for (int i = 0; i < dec_num; ++i)
		{
			lower_bound_[i] = 0.0;
			upper_bound_[i] = 1.0;
		}
    }

    Swimmer::~Swimmer()
    {

    }

    void Swimmer::CalObj(Individual *ind)
    {
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
        PyObject *pModule = PyImport_ImportModule("MuJoCo");
        if (!pModule)
        {
            std::cerr << "[ERROR] Can not open python file MuJoCo.py!"<<std::endl;
            return;
        }

        PyObject *pSwimmer = NULL;
        if (obj_num_ == 2) {
            pSwimmer = PyObject_GetAttrString(pModule, "Swimmer");
        } else if (obj_num_ == 3) {
            pSwimmer = PyObject_GetAttrString(pModule, "SwimmerV3");
        } else {
            exit(-1);
            std::cerr<<"<error> [SWIMMER::CalObj] wrong obj number!"<<std::endl;
        }
        PyObject *x = PyList_New(dec_num_);
        for (int i = 0; i < dec_num_; ++i)
        {
            PyList_SetItem(x, i, PyFloat_FromDouble(ind->dec_[i]));
        }

        PyObject *pRet = PyObject_CallFunctionObjArgs(pSwimmer, x, NULL);
        if (pRet != NULL)  // 验证是否调用成功
        {
            int listSize = PyList_Size(pRet);
            PyObject *float_py = NULL;
            for (int i = 0; i < listSize; ++i)
            {
                float_py = PyList_GetItem(pRet, i);
                ind->obj_[i] = PyFloat_AsDouble(float_py);
            }
        }
        else
        {
            std::cerr<<"[ERROR] Function excuted unsuccessfully."<<std::endl;
            return;
        }
        Py_DECREF(x);
        Py_DECREF(pSwimmer);
        Py_DECREF(pRet);
        Py_DECREF(pModule);

        return;
    }
}