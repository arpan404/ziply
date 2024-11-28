# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/arpanbhandari/Documents/coding/ziply

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/arpanbhandari/Documents/coding/ziply

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/opt/homebrew/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/opt/homebrew/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/arpanbhandari/Documents/coding/ziply/CMakeFiles /Users/arpanbhandari/Documents/coding/ziply//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/arpanbhandari/Documents/coding/ziply/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Ziply

# Build rule for target.
Ziply: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 Ziply
.PHONY : Ziply

# fast build rule for target.
Ziply/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/build
.PHONY : Ziply/fast

src/Error.o: src/Error.cpp.o
.PHONY : src/Error.o

# target to build an object file
src/Error.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/Error.cpp.o
.PHONY : src/Error.cpp.o

src/Error.i: src/Error.cpp.i
.PHONY : src/Error.i

# target to preprocess a source file
src/Error.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/Error.cpp.i
.PHONY : src/Error.cpp.i

src/Error.s: src/Error.cpp.s
.PHONY : src/Error.s

# target to generate assembly for a file
src/Error.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/Error.cpp.s
.PHONY : src/Error.cpp.s

src/File.o: src/File.cpp.o
.PHONY : src/File.o

# target to build an object file
src/File.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/File.cpp.o
.PHONY : src/File.cpp.o

src/File.i: src/File.cpp.i
.PHONY : src/File.i

# target to preprocess a source file
src/File.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/File.cpp.i
.PHONY : src/File.cpp.i

src/File.s: src/File.cpp.s
.PHONY : src/File.s

# target to generate assembly for a file
src/File.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/File.cpp.s
.PHONY : src/File.cpp.s

src/ThreadPool.o: src/ThreadPool.cpp.o
.PHONY : src/ThreadPool.o

# target to build an object file
src/ThreadPool.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/ThreadPool.cpp.o
.PHONY : src/ThreadPool.cpp.o

src/ThreadPool.i: src/ThreadPool.cpp.i
.PHONY : src/ThreadPool.i

# target to preprocess a source file
src/ThreadPool.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/ThreadPool.cpp.i
.PHONY : src/ThreadPool.cpp.i

src/ThreadPool.s: src/ThreadPool.cpp.s
.PHONY : src/ThreadPool.s

# target to generate assembly for a file
src/ThreadPool.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/ThreadPool.cpp.s
.PHONY : src/ThreadPool.cpp.s

src/Ziply.o: src/Ziply.cpp.o
.PHONY : src/Ziply.o

# target to build an object file
src/Ziply.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/Ziply.cpp.o
.PHONY : src/Ziply.cpp.o

src/Ziply.i: src/Ziply.cpp.i
.PHONY : src/Ziply.i

# target to preprocess a source file
src/Ziply.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/Ziply.cpp.i
.PHONY : src/Ziply.cpp.i

src/Ziply.s: src/Ziply.cpp.s
.PHONY : src/Ziply.s

# target to generate assembly for a file
src/Ziply.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/Ziply.cpp.s
.PHONY : src/Ziply.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Ziply.dir/build.make CMakeFiles/Ziply.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... Ziply"
	@echo "... src/Error.o"
	@echo "... src/Error.i"
	@echo "... src/Error.s"
	@echo "... src/File.o"
	@echo "... src/File.i"
	@echo "... src/File.s"
	@echo "... src/ThreadPool.o"
	@echo "... src/ThreadPool.i"
	@echo "... src/ThreadPool.s"
	@echo "... src/Ziply.o"
	@echo "... src/Ziply.i"
	@echo "... src/Ziply.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

