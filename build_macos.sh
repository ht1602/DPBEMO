# download dependencies
brew install gnuplot


mkdir build
cd build
cmake -DBUILD_PYTHON_DLL=OFF ..
#cmake -DBUILD_PYTHON_DLL=ON ..
make -j4