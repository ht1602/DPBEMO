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
include test/CMakeFiles/qr_fullpivoting_5.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/qr_fullpivoting_5.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/qr_fullpivoting_5.dir/flags.make

test/CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.o: test/CMakeFiles/qr_fullpivoting_5.dir/flags.make
test/CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.o: /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test/qr_fullpivoting.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/htian/Documents/EMOC-main/vendor/pinv/build_dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.o"
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.o -c /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test/qr_fullpivoting.cpp

test/CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.i"
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test/qr_fullpivoting.cpp > CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.i

test/CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.s"
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test/qr_fullpivoting.cpp -o CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.s

# Object files for target qr_fullpivoting_5
qr_fullpivoting_5_OBJECTS = \
"CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.o"

# External object files for target qr_fullpivoting_5
qr_fullpivoting_5_EXTERNAL_OBJECTS =

test/qr_fullpivoting_5: test/CMakeFiles/qr_fullpivoting_5.dir/qr_fullpivoting.cpp.o
test/qr_fullpivoting_5: test/CMakeFiles/qr_fullpivoting_5.dir/build.make
test/qr_fullpivoting_5: test/CMakeFiles/qr_fullpivoting_5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/htian/Documents/EMOC-main/vendor/pinv/build_dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable qr_fullpivoting_5"
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/qr_fullpivoting_5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/qr_fullpivoting_5.dir/build: test/qr_fullpivoting_5

.PHONY : test/CMakeFiles/qr_fullpivoting_5.dir/build

test/CMakeFiles/qr_fullpivoting_5.dir/clean:
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test && $(CMAKE_COMMAND) -P CMakeFiles/qr_fullpivoting_5.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/qr_fullpivoting_5.dir/clean

test/CMakeFiles/qr_fullpivoting_5.dir/depend:
	cd /home/htian/Documents/EMOC-main/vendor/pinv/build_dir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8 /home/htian/Documents/EMOC-main/vendor/pinv/eigen-3.3.8/test /home/htian/Documents/EMOC-main/vendor/pinv/build_dir /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test /home/htian/Documents/EMOC-main/vendor/pinv/build_dir/test/CMakeFiles/qr_fullpivoting_5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/qr_fullpivoting_5.dir/depend

