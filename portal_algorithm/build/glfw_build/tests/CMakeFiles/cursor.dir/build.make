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
include glfw_build/tests/CMakeFiles/cursor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include glfw_build/tests/CMakeFiles/cursor.dir/compiler_depend.make

# Include the progress variables for this target.
include glfw_build/tests/CMakeFiles/cursor.dir/progress.make

# Include the compile flags for this target's objects.
include glfw_build/tests/CMakeFiles/cursor.dir/flags.make

glfw_build/tests/CMakeFiles/cursor.dir/cursor.c.o: glfw_build/tests/CMakeFiles/cursor.dir/flags.make
glfw_build/tests/CMakeFiles/cursor.dir/cursor.c.o: /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/tests/cursor.c
glfw_build/tests/CMakeFiles/cursor.dir/cursor.c.o: glfw_build/tests/CMakeFiles/cursor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lpiwowar/school/school-pgr/portal_algorithm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw_build/tests/CMakeFiles/cursor.dir/cursor.c.o"
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT glfw_build/tests/CMakeFiles/cursor.dir/cursor.c.o -MF CMakeFiles/cursor.dir/cursor.c.o.d -o CMakeFiles/cursor.dir/cursor.c.o -c /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/tests/cursor.c

glfw_build/tests/CMakeFiles/cursor.dir/cursor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cursor.dir/cursor.c.i"
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/tests/cursor.c > CMakeFiles/cursor.dir/cursor.c.i

glfw_build/tests/CMakeFiles/cursor.dir/cursor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cursor.dir/cursor.c.s"
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/tests/cursor.c -o CMakeFiles/cursor.dir/cursor.c.s

# Object files for target cursor
cursor_OBJECTS = \
"CMakeFiles/cursor.dir/cursor.c.o"

# External object files for target cursor
cursor_EXTERNAL_OBJECTS =

glfw_build/tests/cursor: glfw_build/tests/CMakeFiles/cursor.dir/cursor.c.o
glfw_build/tests/cursor: glfw_build/tests/CMakeFiles/cursor.dir/build.make
glfw_build/tests/cursor: glfw_build/src/libglfw3.a
glfw_build/tests/cursor: /usr/lib/librt.so
glfw_build/tests/cursor: /usr/lib/libm.so
glfw_build/tests/cursor: /usr/lib/libX11.so
glfw_build/tests/cursor: /usr/lib/libXrandr.so
glfw_build/tests/cursor: /usr/lib/libXinerama.so
glfw_build/tests/cursor: /usr/lib/libXi.so
glfw_build/tests/cursor: /usr/lib/libXxf86vm.so
glfw_build/tests/cursor: /usr/lib/libXcursor.so
glfw_build/tests/cursor: /usr/lib/libGL.so
glfw_build/tests/cursor: glfw_build/tests/CMakeFiles/cursor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lpiwowar/school/school-pgr/portal_algorithm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cursor"
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cursor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw_build/tests/CMakeFiles/cursor.dir/build: glfw_build/tests/cursor
.PHONY : glfw_build/tests/CMakeFiles/cursor.dir/build

glfw_build/tests/CMakeFiles/cursor.dir/clean:
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/tests && $(CMAKE_COMMAND) -P CMakeFiles/cursor.dir/cmake_clean.cmake
.PHONY : glfw_build/tests/CMakeFiles/cursor.dir/clean

glfw_build/tests/CMakeFiles/cursor.dir/depend:
	cd /home/lpiwowar/school/school-pgr/portal_algorithm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lpiwowar/school/school-pgr/portal_algorithm /home/lpiwowar/school/school-pgr/libs/glfw-3.1.2/tests /home/lpiwowar/school/school-pgr/portal_algorithm/build /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/tests /home/lpiwowar/school/school-pgr/portal_algorithm/build/glfw_build/tests/CMakeFiles/cursor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw_build/tests/CMakeFiles/cursor.dir/depend

