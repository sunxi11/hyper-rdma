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
include CMakeFiles/rdmacm_tools.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rdmacm_tools.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rdmacm_tools.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rdmacm_tools.dir/flags.make

CMakeFiles/rdmacm_tools.dir/common.c.o: CMakeFiles/rdmacm_tools.dir/flags.make
CMakeFiles/rdmacm_tools.dir/common.c.o: /Users/sun/CLionProjects/rdma-project/common.c
CMakeFiles/rdmacm_tools.dir/common.c.o: CMakeFiles/rdmacm_tools.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/rdmacm_tools.dir/common.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rdmacm_tools.dir/common.c.o -MF CMakeFiles/rdmacm_tools.dir/common.c.o.d -o CMakeFiles/rdmacm_tools.dir/common.c.o -c /Users/sun/CLionProjects/rdma-project/common.c

CMakeFiles/rdmacm_tools.dir/common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rdmacm_tools.dir/common.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sun/CLionProjects/rdma-project/common.c > CMakeFiles/rdmacm_tools.dir/common.c.i

CMakeFiles/rdmacm_tools.dir/common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rdmacm_tools.dir/common.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sun/CLionProjects/rdma-project/common.c -o CMakeFiles/rdmacm_tools.dir/common.c.s

# Object files for target rdmacm_tools
rdmacm_tools_OBJECTS = \
"CMakeFiles/rdmacm_tools.dir/common.c.o"

# External object files for target rdmacm_tools
rdmacm_tools_EXTERNAL_OBJECTS =

librdmacm_tools.a: CMakeFiles/rdmacm_tools.dir/common.c.o
librdmacm_tools.a: CMakeFiles/rdmacm_tools.dir/build.make
librdmacm_tools.a: CMakeFiles/rdmacm_tools.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library librdmacm_tools.a"
	$(CMAKE_COMMAND) -P CMakeFiles/rdmacm_tools.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rdmacm_tools.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rdmacm_tools.dir/build: librdmacm_tools.a
.PHONY : CMakeFiles/rdmacm_tools.dir/build

CMakeFiles/rdmacm_tools.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rdmacm_tools.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rdmacm_tools.dir/clean

CMakeFiles/rdmacm_tools.dir/depend:
	cd /Users/sun/CLionProjects/rdma-project/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sun/CLionProjects/rdma-project /Users/sun/CLionProjects/rdma-project /Users/sun/CLionProjects/rdma-project/cmake-build-debug /Users/sun/CLionProjects/rdma-project/cmake-build-debug /Users/sun/CLionProjects/rdma-project/cmake-build-debug/CMakeFiles/rdmacm_tools.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rdmacm_tools.dir/depend

