# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sun/CLionProjects/rdma-project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sun/CLionProjects/rdma-project/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ibinfo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ibinfo.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ibinfo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ibinfo.dir/flags.make

CMakeFiles/ibinfo.dir/get_ibinfo.cpp.o: CMakeFiles/ibinfo.dir/flags.make
CMakeFiles/ibinfo.dir/get_ibinfo.cpp.o: /Users/sun/CLionProjects/rdma-project/get_ibinfo.cpp
CMakeFiles/ibinfo.dir/get_ibinfo.cpp.o: CMakeFiles/ibinfo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ibinfo.dir/get_ibinfo.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ibinfo.dir/get_ibinfo.cpp.o -MF CMakeFiles/ibinfo.dir/get_ibinfo.cpp.o.d -o CMakeFiles/ibinfo.dir/get_ibinfo.cpp.o -c /Users/sun/CLionProjects/rdma-project/get_ibinfo.cpp

CMakeFiles/ibinfo.dir/get_ibinfo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ibinfo.dir/get_ibinfo.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sun/CLionProjects/rdma-project/get_ibinfo.cpp > CMakeFiles/ibinfo.dir/get_ibinfo.cpp.i

CMakeFiles/ibinfo.dir/get_ibinfo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ibinfo.dir/get_ibinfo.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sun/CLionProjects/rdma-project/get_ibinfo.cpp -o CMakeFiles/ibinfo.dir/get_ibinfo.cpp.s

# Object files for target ibinfo
ibinfo_OBJECTS = \
"CMakeFiles/ibinfo.dir/get_ibinfo.cpp.o"

# External object files for target ibinfo
ibinfo_EXTERNAL_OBJECTS =

ibinfo: CMakeFiles/ibinfo.dir/get_ibinfo.cpp.o
ibinfo: CMakeFiles/ibinfo.dir/build.make
ibinfo: librdmacm_tools.a
ibinfo: CMakeFiles/ibinfo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ibinfo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ibinfo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ibinfo.dir/build: ibinfo
.PHONY : CMakeFiles/ibinfo.dir/build

CMakeFiles/ibinfo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ibinfo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ibinfo.dir/clean

CMakeFiles/ibinfo.dir/depend:
	cd /Users/sun/CLionProjects/rdma-project/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sun/CLionProjects/rdma-project /Users/sun/CLionProjects/rdma-project /Users/sun/CLionProjects/rdma-project/cmake-build-debug /Users/sun/CLionProjects/rdma-project/cmake-build-debug /Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles/ibinfo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ibinfo.dir/depend
