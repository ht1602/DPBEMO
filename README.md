# Direct Preference-Based Evolutionary Multi-Objective Optimization with Dueling Bandits

This is the code for the paper "Direct Preference-Based Evolutionary Multi-Objective Optimization with Dueling Bandits". 

## How to use the our algorithms

For installation and usage, please refer to the [document site](https://cola-laboratory.github.io/EMOCDoc/).

In the package, our $\texttt{D-PBEMO}$ algorithms are named DPBEMO_NSGA2 and DPBEMO_MOEAD respectively.

To use our algorithms, you can run in the command line:

`./EMOC -a DPBEMO_NSGA2 -p DTLZ1 -M 3 -D 12 -N 64 -e 16000 -r 20 -W 0.7,0.8,0.5 -q 25`

`./EMOC -a DPBEMO_NSGA2 -p DTLZ1 -M 3 -D 12 -N 64 -e 16000 -r 20 -W 0.7,0.8,0.5 -q 25`

Or you can also set experiment settings in the config.ini file.



## 3rd Party Libraries

- [GLFW](https://www.glfw.org/)
- [ALGLIB](https://www.alglib.net/)
- [Dear Imgui](https://github.com/ocornut/imgui)
- [pybind11](https://github.com/pybind/pybind11)
- [cxxopts](https://github.com/jarro2783/cxxopts)
- [stb_image](https://github.com/nothings/stb)
- [Gnuplot](http://www.gnuplot.info/)
- [CMake](https://cmake.org/)
- [Font-Awesome](https://github.com/FortAwesome/Font-Awesome)

