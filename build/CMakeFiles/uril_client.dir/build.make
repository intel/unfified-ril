# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /repo/emarmou/work/uril2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /repo/emarmou/work/uril2/build

# Include any dependencies generated for this target.
include CMakeFiles/uril_client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/uril_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/uril_client.dir/flags.make

CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o: CMakeFiles/uril_client.dir/flags.make
CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o: ../client/src/service_connector/unix_socket_service_connector.c
	$(CMAKE_COMMAND) -E cmake_progress_report /repo/emarmou/work/uril2/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o   -c /repo/emarmou/work/uril2/client/src/service_connector/unix_socket_service_connector.c

CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /repo/emarmou/work/uril2/client/src/service_connector/unix_socket_service_connector.c > CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.i

CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /repo/emarmou/work/uril2/client/src/service_connector/unix_socket_service_connector.c -o CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.s

CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o.requires:
.PHONY : CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o.requires

CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o.provides: CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o.requires
	$(MAKE) -f CMakeFiles/uril_client.dir/build.make CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o.provides.build
.PHONY : CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o.provides

CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o.provides.build: CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o

CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o: CMakeFiles/uril_client.dir/flags.make
CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o: ../client/src/osal/osal_mutex_unix.c
	$(CMAKE_COMMAND) -E cmake_progress_report /repo/emarmou/work/uril2/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o   -c /repo/emarmou/work/uril2/client/src/osal/osal_mutex_unix.c

CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /repo/emarmou/work/uril2/client/src/osal/osal_mutex_unix.c > CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.i

CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /repo/emarmou/work/uril2/client/src/osal/osal_mutex_unix.c -o CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.s

CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o.requires:
.PHONY : CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o.requires

CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o.provides: CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o.requires
	$(MAKE) -f CMakeFiles/uril_client.dir/build.make CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o.provides.build
.PHONY : CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o.provides

CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o.provides.build: CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o

CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o: CMakeFiles/uril_client.dir/flags.make
CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o: ../client/src/osal/osal_condition_unix.c
	$(CMAKE_COMMAND) -E cmake_progress_report /repo/emarmou/work/uril2/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o   -c /repo/emarmou/work/uril2/client/src/osal/osal_condition_unix.c

CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /repo/emarmou/work/uril2/client/src/osal/osal_condition_unix.c > CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.i

CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /repo/emarmou/work/uril2/client/src/osal/osal_condition_unix.c -o CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.s

CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o.requires:
.PHONY : CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o.requires

CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o.provides: CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o.requires
	$(MAKE) -f CMakeFiles/uril_client.dir/build.make CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o.provides.build
.PHONY : CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o.provides

CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o.provides.build: CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o

CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o: CMakeFiles/uril_client.dir/flags.make
CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o: ../client/src/uril_protocol.c
	$(CMAKE_COMMAND) -E cmake_progress_report /repo/emarmou/work/uril2/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o   -c /repo/emarmou/work/uril2/client/src/uril_protocol.c

CMakeFiles/uril_client.dir/client/src/uril_protocol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/uril_client.dir/client/src/uril_protocol.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /repo/emarmou/work/uril2/client/src/uril_protocol.c > CMakeFiles/uril_client.dir/client/src/uril_protocol.c.i

CMakeFiles/uril_client.dir/client/src/uril_protocol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/uril_client.dir/client/src/uril_protocol.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /repo/emarmou/work/uril2/client/src/uril_protocol.c -o CMakeFiles/uril_client.dir/client/src/uril_protocol.c.s

CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o.requires:
.PHONY : CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o.requires

CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o.provides: CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o.requires
	$(MAKE) -f CMakeFiles/uril_client.dir/build.make CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o.provides.build
.PHONY : CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o.provides

CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o.provides.build: CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o

CMakeFiles/uril_client.dir/client/src/uril_client.c.o: CMakeFiles/uril_client.dir/flags.make
CMakeFiles/uril_client.dir/client/src/uril_client.c.o: ../client/src/uril_client.c
	$(CMAKE_COMMAND) -E cmake_progress_report /repo/emarmou/work/uril2/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/uril_client.dir/client/src/uril_client.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/uril_client.dir/client/src/uril_client.c.o   -c /repo/emarmou/work/uril2/client/src/uril_client.c

CMakeFiles/uril_client.dir/client/src/uril_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/uril_client.dir/client/src/uril_client.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /repo/emarmou/work/uril2/client/src/uril_client.c > CMakeFiles/uril_client.dir/client/src/uril_client.c.i

CMakeFiles/uril_client.dir/client/src/uril_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/uril_client.dir/client/src/uril_client.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /repo/emarmou/work/uril2/client/src/uril_client.c -o CMakeFiles/uril_client.dir/client/src/uril_client.c.s

CMakeFiles/uril_client.dir/client/src/uril_client.c.o.requires:
.PHONY : CMakeFiles/uril_client.dir/client/src/uril_client.c.o.requires

CMakeFiles/uril_client.dir/client/src/uril_client.c.o.provides: CMakeFiles/uril_client.dir/client/src/uril_client.c.o.requires
	$(MAKE) -f CMakeFiles/uril_client.dir/build.make CMakeFiles/uril_client.dir/client/src/uril_client.c.o.provides.build
.PHONY : CMakeFiles/uril_client.dir/client/src/uril_client.c.o.provides

CMakeFiles/uril_client.dir/client/src/uril_client.c.o.provides.build: CMakeFiles/uril_client.dir/client/src/uril_client.c.o

# Object files for target uril_client
uril_client_OBJECTS = \
"CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o" \
"CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o" \
"CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o" \
"CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o" \
"CMakeFiles/uril_client.dir/client/src/uril_client.c.o"

# External object files for target uril_client
uril_client_EXTERNAL_OBJECTS =

liburil_client.so.0.1.0: CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o
liburil_client.so.0.1.0: CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o
liburil_client.so.0.1.0: CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o
liburil_client.so.0.1.0: CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o
liburil_client.so.0.1.0: CMakeFiles/uril_client.dir/client/src/uril_client.c.o
liburil_client.so.0.1.0: CMakeFiles/uril_client.dir/build.make
liburil_client.so.0.1.0: CMakeFiles/uril_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library liburil_client.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/uril_client.dir/link.txt --verbose=$(VERBOSE)
	$(CMAKE_COMMAND) -E cmake_symlink_library liburil_client.so.0.1.0 liburil_client.so.0.1.0 liburil_client.so

liburil_client.so: liburil_client.so.0.1.0
	@$(CMAKE_COMMAND) -E touch_nocreate liburil_client.so

# Rule to build all files generated by this target.
CMakeFiles/uril_client.dir/build: liburil_client.so
.PHONY : CMakeFiles/uril_client.dir/build

CMakeFiles/uril_client.dir/requires: CMakeFiles/uril_client.dir/client/src/service_connector/unix_socket_service_connector.c.o.requires
CMakeFiles/uril_client.dir/requires: CMakeFiles/uril_client.dir/client/src/osal/osal_mutex_unix.c.o.requires
CMakeFiles/uril_client.dir/requires: CMakeFiles/uril_client.dir/client/src/osal/osal_condition_unix.c.o.requires
CMakeFiles/uril_client.dir/requires: CMakeFiles/uril_client.dir/client/src/uril_protocol.c.o.requires
CMakeFiles/uril_client.dir/requires: CMakeFiles/uril_client.dir/client/src/uril_client.c.o.requires
.PHONY : CMakeFiles/uril_client.dir/requires

CMakeFiles/uril_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/uril_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/uril_client.dir/clean

CMakeFiles/uril_client.dir/depend:
	cd /repo/emarmou/work/uril2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /repo/emarmou/work/uril2 /repo/emarmou/work/uril2 /repo/emarmou/work/uril2/build /repo/emarmou/work/uril2/build /repo/emarmou/work/uril2/build/CMakeFiles/uril_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/uril_client.dir/depend
