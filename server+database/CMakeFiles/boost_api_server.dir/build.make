# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/victoria/http-server-cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/victoria/http-server-cpp

# Include any dependencies generated for this target.
include CMakeFiles/boost_api_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/boost_api_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/boost_api_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/boost_api_server.dir/flags.make

CMakeFiles/boost_api_server.dir/src/auth.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/auth.cpp.o: src/auth.cpp
CMakeFiles/boost_api_server.dir/src/auth.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/boost_api_server.dir/src/auth.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/auth.cpp.o -MF CMakeFiles/boost_api_server.dir/src/auth.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/auth.cpp.o -c /home/victoria/http-server-cpp/src/auth.cpp

CMakeFiles/boost_api_server.dir/src/auth.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/auth.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/auth.cpp > CMakeFiles/boost_api_server.dir/src/auth.cpp.i

CMakeFiles/boost_api_server.dir/src/auth.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/auth.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/auth.cpp -o CMakeFiles/boost_api_server.dir/src/auth.cpp.s

CMakeFiles/boost_api_server.dir/src/common.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/common.cpp.o: src/common.cpp
CMakeFiles/boost_api_server.dir/src/common.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/boost_api_server.dir/src/common.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/common.cpp.o -MF CMakeFiles/boost_api_server.dir/src/common.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/common.cpp.o -c /home/victoria/http-server-cpp/src/common.cpp

CMakeFiles/boost_api_server.dir/src/common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/common.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/common.cpp > CMakeFiles/boost_api_server.dir/src/common.cpp.i

CMakeFiles/boost_api_server.dir/src/common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/common.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/common.cpp -o CMakeFiles/boost_api_server.dir/src/common.cpp.s

CMakeFiles/boost_api_server.dir/src/handler_request.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/handler_request.cpp.o: src/handler_request.cpp
CMakeFiles/boost_api_server.dir/src/handler_request.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/boost_api_server.dir/src/handler_request.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/handler_request.cpp.o -MF CMakeFiles/boost_api_server.dir/src/handler_request.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/handler_request.cpp.o -c /home/victoria/http-server-cpp/src/handler_request.cpp

CMakeFiles/boost_api_server.dir/src/handler_request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/handler_request.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/handler_request.cpp > CMakeFiles/boost_api_server.dir/src/handler_request.cpp.i

CMakeFiles/boost_api_server.dir/src/handler_request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/handler_request.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/handler_request.cpp -o CMakeFiles/boost_api_server.dir/src/handler_request.cpp.s

CMakeFiles/boost_api_server.dir/src/handlers.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/handlers.cpp.o: src/handlers.cpp
CMakeFiles/boost_api_server.dir/src/handlers.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/boost_api_server.dir/src/handlers.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/handlers.cpp.o -MF CMakeFiles/boost_api_server.dir/src/handlers.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/handlers.cpp.o -c /home/victoria/http-server-cpp/src/handlers.cpp

CMakeFiles/boost_api_server.dir/src/handlers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/handlers.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/handlers.cpp > CMakeFiles/boost_api_server.dir/src/handlers.cpp.i

CMakeFiles/boost_api_server.dir/src/handlers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/handlers.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/handlers.cpp -o CMakeFiles/boost_api_server.dir/src/handlers.cpp.s

CMakeFiles/boost_api_server.dir/src/input_utils.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/input_utils.cpp.o: src/input_utils.cpp
CMakeFiles/boost_api_server.dir/src/input_utils.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/boost_api_server.dir/src/input_utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/input_utils.cpp.o -MF CMakeFiles/boost_api_server.dir/src/input_utils.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/input_utils.cpp.o -c /home/victoria/http-server-cpp/src/input_utils.cpp

CMakeFiles/boost_api_server.dir/src/input_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/input_utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/input_utils.cpp > CMakeFiles/boost_api_server.dir/src/input_utils.cpp.i

CMakeFiles/boost_api_server.dir/src/input_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/input_utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/input_utils.cpp -o CMakeFiles/boost_api_server.dir/src/input_utils.cpp.s

CMakeFiles/boost_api_server.dir/src/main.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/main.cpp.o: src/main.cpp
CMakeFiles/boost_api_server.dir/src/main.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/boost_api_server.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/main.cpp.o -MF CMakeFiles/boost_api_server.dir/src/main.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/main.cpp.o -c /home/victoria/http-server-cpp/src/main.cpp

CMakeFiles/boost_api_server.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/main.cpp > CMakeFiles/boost_api_server.dir/src/main.cpp.i

CMakeFiles/boost_api_server.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/main.cpp -o CMakeFiles/boost_api_server.dir/src/main.cpp.s

CMakeFiles/boost_api_server.dir/src/password_utils.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/password_utils.cpp.o: src/password_utils.cpp
CMakeFiles/boost_api_server.dir/src/password_utils.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/boost_api_server.dir/src/password_utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/password_utils.cpp.o -MF CMakeFiles/boost_api_server.dir/src/password_utils.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/password_utils.cpp.o -c /home/victoria/http-server-cpp/src/password_utils.cpp

CMakeFiles/boost_api_server.dir/src/password_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/password_utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/password_utils.cpp > CMakeFiles/boost_api_server.dir/src/password_utils.cpp.i

CMakeFiles/boost_api_server.dir/src/password_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/password_utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/password_utils.cpp -o CMakeFiles/boost_api_server.dir/src/password_utils.cpp.s

CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.o: src/patient_menu.cpp
CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.o -MF CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.o -c /home/victoria/http-server-cpp/src/patient_menu.cpp

CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/patient_menu.cpp > CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.i

CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/patient_menu.cpp -o CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.s

CMakeFiles/boost_api_server.dir/src/server.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/server.cpp.o: src/server.cpp
CMakeFiles/boost_api_server.dir/src/server.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/boost_api_server.dir/src/server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/server.cpp.o -MF CMakeFiles/boost_api_server.dir/src/server.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/server.cpp.o -c /home/victoria/http-server-cpp/src/server.cpp

CMakeFiles/boost_api_server.dir/src/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/server.cpp > CMakeFiles/boost_api_server.dir/src/server.cpp.i

CMakeFiles/boost_api_server.dir/src/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/server.cpp -o CMakeFiles/boost_api_server.dir/src/server.cpp.s

CMakeFiles/boost_api_server.dir/src/session.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/session.cpp.o: src/session.cpp
CMakeFiles/boost_api_server.dir/src/session.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/boost_api_server.dir/src/session.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/session.cpp.o -MF CMakeFiles/boost_api_server.dir/src/session.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/session.cpp.o -c /home/victoria/http-server-cpp/src/session.cpp

CMakeFiles/boost_api_server.dir/src/session.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/session.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/session.cpp > CMakeFiles/boost_api_server.dir/src/session.cpp.i

CMakeFiles/boost_api_server.dir/src/session.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/session.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/session.cpp -o CMakeFiles/boost_api_server.dir/src/session.cpp.s

CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.o: CMakeFiles/boost_api_server.dir/flags.make
CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.o: src/super_admin_menu.cpp
CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.o: CMakeFiles/boost_api_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.o -MF CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.o.d -o CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.o -c /home/victoria/http-server-cpp/src/super_admin_menu.cpp

CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victoria/http-server-cpp/src/super_admin_menu.cpp > CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.i

CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victoria/http-server-cpp/src/super_admin_menu.cpp -o CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.s

# Object files for target boost_api_server
boost_api_server_OBJECTS = \
"CMakeFiles/boost_api_server.dir/src/auth.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/common.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/handler_request.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/handlers.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/input_utils.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/main.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/password_utils.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/server.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/session.cpp.o" \
"CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.o"

# External object files for target boost_api_server
boost_api_server_EXTERNAL_OBJECTS =

boost_api_server: CMakeFiles/boost_api_server.dir/src/auth.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/common.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/handler_request.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/handlers.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/input_utils.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/main.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/password_utils.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/patient_menu.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/server.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/session.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/src/super_admin_menu.cpp.o
boost_api_server: CMakeFiles/boost_api_server.dir/build.make
boost_api_server: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.74.0
boost_api_server: /usr/lib/x86_64-linux-gnu/libpq.so
boost_api_server: /usr/lib/x86_64-linux-gnu/libcrypto.so
boost_api_server: CMakeFiles/boost_api_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/victoria/http-server-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable boost_api_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/boost_api_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/boost_api_server.dir/build: boost_api_server
.PHONY : CMakeFiles/boost_api_server.dir/build

CMakeFiles/boost_api_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/boost_api_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/boost_api_server.dir/clean

CMakeFiles/boost_api_server.dir/depend:
	cd /home/victoria/http-server-cpp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victoria/http-server-cpp /home/victoria/http-server-cpp /home/victoria/http-server-cpp /home/victoria/http-server-cpp /home/victoria/http-server-cpp/CMakeFiles/boost_api_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/boost_api_server.dir/depend

