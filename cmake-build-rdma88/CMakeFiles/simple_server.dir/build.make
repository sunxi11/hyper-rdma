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
CMAKE_SOURCE_DIR = /root/rdma-project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/rdma-project/cmake-build-rdma88

# Include any dependencies generated for this target.
include CMakeFiles/simple_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/simple_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simple_server.dir/flags.make

CMakeFiles/simple_server.dir/simplest_server.cpp.o: CMakeFiles/simple_server.dir/flags.make
CMakeFiles/simple_server.dir/simplest_server.cpp.o: ../simplest_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/rdma-project/cmake-build-rdma88/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simple_server.dir/simplest_server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simple_server.dir/simplest_server.cpp.o -c /root/rdma-project/simplest_server.cpp

CMakeFiles/simple_server.dir/simplest_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple_server.dir/simplest_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/rdma-project/simplest_server.cpp > CMakeFiles/simple_server.dir/simplest_server.cpp.i

CMakeFiles/simple_server.dir/simplest_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple_server.dir/simplest_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/rdma-project/simplest_server.cpp -o CMakeFiles/simple_server.dir/simplest_server.cpp.s

CMakeFiles/simple_server.dir/DataStruct.cpp.o: CMakeFiles/simple_server.dir/flags.make
CMakeFiles/simple_server.dir/DataStruct.cpp.o: ../DataStruct.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/rdma-project/cmake-build-rdma88/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/simple_server.dir/DataStruct.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simple_server.dir/DataStruct.cpp.o -c /root/rdma-project/DataStruct.cpp

CMakeFiles/simple_server.dir/DataStruct.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple_server.dir/DataStruct.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/rdma-project/DataStruct.cpp > CMakeFiles/simple_server.dir/DataStruct.cpp.i

CMakeFiles/simple_server.dir/DataStruct.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple_server.dir/DataStruct.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/rdma-project/DataStruct.cpp -o CMakeFiles/simple_server.dir/DataStruct.cpp.s

# Object files for target simple_server
simple_server_OBJECTS = \
"CMakeFiles/simple_server.dir/simplest_server.cpp.o" \
"CMakeFiles/simple_server.dir/DataStruct.cpp.o"

# External object files for target simple_server
simple_server_EXTERNAL_OBJECTS =

simple_server: CMakeFiles/simple_server.dir/simplest_server.cpp.o
simple_server: CMakeFiles/simple_server.dir/DataStruct.cpp.o
simple_server: CMakeFiles/simple_server.dir/build.make
simple_server: librdmacm_tools.a
simple_server: CMakeFiles/simple_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/rdma-project/cmake-build-rdma88/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable simple_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simple_server.dir/build: simple_server

.PHONY : CMakeFiles/simple_server.dir/build

CMakeFiles/simple_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simple_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simple_server.dir/clean

CMakeFiles/simple_server.dir/depend:
	cd /root/rdma-project/cmake-build-rdma88 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/rdma-project /root/rdma-project /root/rdma-project/cmake-build-rdma88 /root/rdma-project/cmake-build-rdma88 /root/rdma-project/cmake-build-rdma88/CMakeFiles/simple_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simple_server.dir/depend
