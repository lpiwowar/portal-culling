# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lpiwowar/school/school-pgr/portal_algorithm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lpiwowar/school/school-pgr/portal_algorithm/build

# Include any dependencies generated for this target.
include glfw_build/examples/CMakeFiles/gears.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include glfw_build/examples/CMakeFiles/gears.dir/compiler_depend.make

# Include the progress variables for this target.
include glfw_build/examples/CMakeFiles/gears.dir/progress.make

# Include the compile flags for this target's objects.
include glfw_build/examples/CMakeFiles/gears.dir/flags.make

glfw_build/examples/CMakeFiles/gears.dir/gears.c.o: glfw_build/examples/CMakeFiles/gears.dir/flags.make
glfw_build/examples/CMakeFiles/gears.dir/gears.c.o: /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/examples/gears.c
glfw_build/examples/CMakeFiles/gears.dir/gears.c.o: glfw_build/examples/CMakeFiles/gears.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lpiwowar/school/school-pgr/portal_algorithm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw_build/examples/CMakeFiles/gears.dir/gears.c.o"
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT glfw_build/examples/CMakeFiles/gears.dir/gears.c.o -MF CMakeFiles/gears.dir/gears.c.o.d -o CMakeFiles/gears.dir/gears.c.o -c /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/examples/gears.c

glfw_build/examples/CMakeFiles/gears.dir/gears.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gears.dir/gears.c.i"
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/examples/gears.c > CMakeFiles/gears.dir/gears.c.i

glfw_build/examples/CMakeFiles/gears.dir/gears.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gears.dir/gears.c.s"
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/examples/gears.c -o CMakeFiles/gears.dir/gears.c.s

# Object files for target gears
gears_OBJECTS = \
"CMakeFiles/gears.dir/gears.c.o"

# External object files for target gears
gears_EXTERNAL_OBJECTS =

glfw_build/examples/gears: glfw_build/examples/CMakeFiles/gears.dir/gears.c.o
glfw_build/examples/gears: glfw_build/examples/CMakeFiles/gears.dir/build.make
glfw_build/examples/gears: glfw_build/src/libglfw3.a
glfw_build/examples/gears: /usr/lib/librt.so
glfw_build/examples/gears: /usr/lib/libm.so
glfw_build/examples/gears: /usr/lib/libX11.so
glfw_build/examples/gears: /usr/lib/libXrandr.so
glfw_build/examples/gears: /usr/lib/libXinerama.so
glfw_build/examples/gears: /usr/lib/libXi.so
glfw_build/examples/gears: /usr/lib/libXxf86vm.so
glfw_build/examples/gears: /usr/lib/libXcursor.so
glfw_build/examples/gears: /usr/lib/libGL.so
glfw_build/examples/gears: glfw_build/examples/CMakeFiles/gears.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lpiwowar/school/school-pgr/portal_algorithm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable gears"
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gears.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw_build/examples/CMakeFiles/gears.dir/build: glfw_build/examples/gears
.PHONY : glfw_build/examples/CMakeFiles/gears.dir/build

glfw_build/examples/CMakeFiles/gears.dir/clean:
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/examples && $(CMAKE_COMMAND) -P CMakeFiles/gears.dir/cmake_clean.cmake
.PHONY : glfw_build/examples/CMakeFiles/gears.dir/clean

glfw_build/examples/CMakeFiles/gears.dir/depend:
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lpiwowar/school/school-pgr/portal_algorithm /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/examples /home/lpiwowar/school/school-pgr/portal_algorithm/build /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/examples /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/examples/CMakeFiles/gears.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw_build/examples/CMakeFiles/gears.dir/depend

