# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/htian/Documents/EMOC-main/vendor/pinv/build_dir

# Include any dependencies generated for this target.
include test/CMakeFiles/real_qz_3.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/real_qz_3.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/real_qz_3.dir/flags.make

test/CMakeFiles/real_qz_3.dir/real_qz.cpp.o: test/CMakeFiles/real_qz_3.dir/flags.make
test/CMakeFiles/real_qz_3.dir/real_qz.cpp.o: /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test/real_qz.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/htian/Documents/EMOC-main/vendor/pinv/build_dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/real_qz_3.dir/real_qz.cpp.o"
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/real_qz_3.dir/real_qz.cpp.o -c /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test/real_qz.cpp

test/CMakeFiles/real_qz_3.dir/real_qz.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/real_qz_3.dir/real_qz.cpp.i"
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test/real_qz.cpp > CMakeFiles/real_qz_3.dir/real_qz.cpp.i

test/CMakeFiles/real_qz_3.dir/real_qz.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/real_qz_3.dir/real_qz.cpp.s"
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test/real_qz.cpp -o CMakeFiles/real_qz_3.dir/real_qz.cpp.s

# Object files for target real_qz_3
real_qz_3_OBJECTS = \
"CMakeFiles/real_qz_3.dir/real_qz.cpp.o"

# External object files for target real_qz_3
real_qz_3_EXTERNAL_OBJECTS =

test/real_qz_3: test/CMakeFiles/real_qz_3.dir/real_qz.cpp.o
test/real_qz_3: test/CMakeFiles/real_qz_3.dir/build.make
test/real_qz_3: test/CMakeFiles/real_qz_3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/htian/Documents/EMOC-main/vendor/pinv/build_dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable real_qz_3"
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/real_qz_3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/real_qz_3.dir/build: test/real_qz_3

.PHONY : test/CMakeFiles/real_qz_3.dir/build

test/CMakeFiles/real_qz_3.dir/clean:
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && $(CMAKE_COMMAND) -P CMakeFiles/real_qz_3.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/real_qz_3.dir/clean

test/CMakeFiles/real_qz_3.dir/depend:
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8 /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test /home/htian/Documents/EMOC-main/vendor/pinv/build_dir /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test/CMakeFiles/real_qz_3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/real_qz_3.dir/depend

