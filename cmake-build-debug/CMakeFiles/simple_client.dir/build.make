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
include CMakeFiles/simple_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/simple_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/simple_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simple_client.dir/flags.make

CMakeFiles/simple_client.dir/simplest_client.cpp.o: CMakeFiles/simple_client.dir/flags.make
CMakeFiles/simple_client.dir/simplest_client.cpp.o: /Users/sun/CLionProjects/rdma-project/simplest_client.cpp
CMakeFiles/simple_client.dir/simplest_client.cpp.o: CMakeFiles/simple_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simple_client.dir/simplest_client.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/simple_client.dir/simplest_client.cpp.o -MF CMakeFiles/simple_client.dir/simplest_client.cpp.o.d -o CMakeFiles/simple_client.dir/simplest_client.cpp.o -c /Users/sun/CLionProjects/rdma-project/simplest_client.cpp

CMakeFiles/simple_client.dir/simplest_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple_client.dir/simplest_client.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sun/CLionProjects/rdma-project/simplest_client.cpp > CMakeFiles/simple_client.dir/simplest_client.cpp.i

CMakeFiles/simple_client.dir/simplest_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple_client.dir/simplest_client.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sun/CLionProjects/rdma-project/simplest_client.cpp -o CMakeFiles/simple_client.dir/simplest_client.cpp.s

CMakeFiles/simple_client.dir/DataStruct.cpp.o: CMakeFiles/simple_client.dir/flags.make
CMakeFiles/simple_client.dir/DataStruct.cpp.o: /Users/sun/CLionProjects/rdma-project/DataStruct.cpp
CMakeFiles/simple_client.dir/DataStruct.cpp.o: CMakeFiles/simple_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/simple_client.dir/DataStruct.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/simple_client.dir/DataStruct.cpp.o -MF CMakeFiles/simple_client.dir/DataStruct.cpp.o.d -o CMakeFiles/simple_client.dir/DataStruct.cpp.o -c /Users/sun/CLionProjects/rdma-project/DataStruct.cpp

CMakeFiles/simple_client.dir/DataStruct.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple_client.dir/DataStruct.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sun/CLionProjects/rdma-project/DataStruct.cpp > CMakeFiles/simple_client.dir/DataStruct.cpp.i

CMakeFiles/simple_client.dir/DataStruct.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple_client.dir/DataStruct.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sun/CLionProjects/rdma-project/DataStruct.cpp -o CMakeFiles/simple_client.dir/DataStruct.cpp.s

# Object files for target simple_client
simple_client_OBJECTS = \
"CMakeFiles/simple_client.dir/simplest_client.cpp.o" \
"CMakeFiles/simple_client.dir/DataStruct.cpp.o"

# External object files for target simple_client
simple_client_EXTERNAL_OBJECTS =

simple_client: CMakeFiles/simple_client.dir/simplest_client.cpp.o
simple_client: CMakeFiles/simple_client.dir/DataStruct.cpp.o
simple_client: CMakeFiles/simple_client.dir/build.make
simple_client: librdmacm_tools.a
simple_client: CMakeFiles/simple_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable simple_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simple_client.dir/build: simple_client
.PHONY : CMakeFiles/simple_client.dir/build

CMakeFiles/simple_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simple_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simple_client.dir/clean

CMakeFiles/simple_client.dir/depend:
	cd /Users/sun/CLionProjects/rdma-project/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sun/CLionProjects/rdma-project /Users/sun/CLionProjects/rdma-project /Users/sun/CLionProjects/rdma-project/cmake-build-debug /Users/sun/CLionProjects/rdma-project/cmake-build-debug /Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles/simple_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simple_client.dir/depend
