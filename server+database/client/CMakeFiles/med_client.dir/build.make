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
CMAKE_SOURCE_DIR = /mnt/c/MedProject/MedScheduler/server+database/client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/MedProject/MedScheduler/server+database/client

# Include any dependencies generated for this target.
include CMakeFiles/med_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/med_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/med_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/med_client.dir/flags.make

CMakeFiles/med_client.dir/src/client_auth.cpp.o: CMakeFiles/med_client.dir/flags.make
CMakeFiles/med_client.dir/src/client_auth.cpp.o: src/client_auth.cpp
CMakeFiles/med_client.dir/src/client_auth.cpp.o: CMakeFiles/med_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/med_client.dir/src/client_auth.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/med_client.dir/src/client_auth.cpp.o -MF CMakeFiles/med_client.dir/src/client_auth.cpp.o.d -o CMakeFiles/med_client.dir/src/client_auth.cpp.o -c /mnt/c/MedProject/MedScheduler/server+database/client/src/client_auth.cpp

CMakeFiles/med_client.dir/src/client_auth.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/med_client.dir/src/client_auth.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/MedProject/MedScheduler/server+database/client/src/client_auth.cpp > CMakeFiles/med_client.dir/src/client_auth.cpp.i

CMakeFiles/med_client.dir/src/client_auth.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/med_client.dir/src/client_auth.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/MedProject/MedScheduler/server+database/client/src/client_auth.cpp -o CMakeFiles/med_client.dir/src/client_auth.cpp.s

CMakeFiles/med_client.dir/src/client_doctor.cpp.o: CMakeFiles/med_client.dir/flags.make
CMakeFiles/med_client.dir/src/client_doctor.cpp.o: src/client_doctor.cpp
CMakeFiles/med_client.dir/src/client_doctor.cpp.o: CMakeFiles/med_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/med_client.dir/src/client_doctor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/med_client.dir/src/client_doctor.cpp.o -MF CMakeFiles/med_client.dir/src/client_doctor.cpp.o.d -o CMakeFiles/med_client.dir/src/client_doctor.cpp.o -c /mnt/c/MedProject/MedScheduler/server+database/client/src/client_doctor.cpp

CMakeFiles/med_client.dir/src/client_doctor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/med_client.dir/src/client_doctor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/MedProject/MedScheduler/server+database/client/src/client_doctor.cpp > CMakeFiles/med_client.dir/src/client_doctor.cpp.i

CMakeFiles/med_client.dir/src/client_doctor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/med_client.dir/src/client_doctor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/MedProject/MedScheduler/server+database/client/src/client_doctor.cpp -o CMakeFiles/med_client.dir/src/client_doctor.cpp.s

CMakeFiles/med_client.dir/src/client_junior_admin.cpp.o: CMakeFiles/med_client.dir/flags.make
CMakeFiles/med_client.dir/src/client_junior_admin.cpp.o: src/client_junior_admin.cpp
CMakeFiles/med_client.dir/src/client_junior_admin.cpp.o: CMakeFiles/med_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/med_client.dir/src/client_junior_admin.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/med_client.dir/src/client_junior_admin.cpp.o -MF CMakeFiles/med_client.dir/src/client_junior_admin.cpp.o.d -o CMakeFiles/med_client.dir/src/client_junior_admin.cpp.o -c /mnt/c/MedProject/MedScheduler/server+database/client/src/client_junior_admin.cpp

CMakeFiles/med_client.dir/src/client_junior_admin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/med_client.dir/src/client_junior_admin.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/MedProject/MedScheduler/server+database/client/src/client_junior_admin.cpp > CMakeFiles/med_client.dir/src/client_junior_admin.cpp.i

CMakeFiles/med_client.dir/src/client_junior_admin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/med_client.dir/src/client_junior_admin.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/MedProject/MedScheduler/server+database/client/src/client_junior_admin.cpp -o CMakeFiles/med_client.dir/src/client_junior_admin.cpp.s

CMakeFiles/med_client.dir/src/client_patient.cpp.o: CMakeFiles/med_client.dir/flags.make
CMakeFiles/med_client.dir/src/client_patient.cpp.o: src/client_patient.cpp
CMakeFiles/med_client.dir/src/client_patient.cpp.o: CMakeFiles/med_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/med_client.dir/src/client_patient.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/med_client.dir/src/client_patient.cpp.o -MF CMakeFiles/med_client.dir/src/client_patient.cpp.o.d -o CMakeFiles/med_client.dir/src/client_patient.cpp.o -c /mnt/c/MedProject/MedScheduler/server+database/client/src/client_patient.cpp

CMakeFiles/med_client.dir/src/client_patient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/med_client.dir/src/client_patient.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/MedProject/MedScheduler/server+database/client/src/client_patient.cpp > CMakeFiles/med_client.dir/src/client_patient.cpp.i

CMakeFiles/med_client.dir/src/client_patient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/med_client.dir/src/client_patient.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/MedProject/MedScheduler/server+database/client/src/client_patient.cpp -o CMakeFiles/med_client.dir/src/client_patient.cpp.s

CMakeFiles/med_client.dir/src/client_senior_admin.cpp.o: CMakeFiles/med_client.dir/flags.make
CMakeFiles/med_client.dir/src/client_senior_admin.cpp.o: src/client_senior_admin.cpp
CMakeFiles/med_client.dir/src/client_senior_admin.cpp.o: CMakeFiles/med_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/med_client.dir/src/client_senior_admin.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/med_client.dir/src/client_senior_admin.cpp.o -MF CMakeFiles/med_client.dir/src/client_senior_admin.cpp.o.d -o CMakeFiles/med_client.dir/src/client_senior_admin.cpp.o -c /mnt/c/MedProject/MedScheduler/server+database/client/src/client_senior_admin.cpp

CMakeFiles/med_client.dir/src/client_senior_admin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/med_client.dir/src/client_senior_admin.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/MedProject/MedScheduler/server+database/client/src/client_senior_admin.cpp > CMakeFiles/med_client.dir/src/client_senior_admin.cpp.i

CMakeFiles/med_client.dir/src/client_senior_admin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/med_client.dir/src/client_senior_admin.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/MedProject/MedScheduler/server+database/client/src/client_senior_admin.cpp -o CMakeFiles/med_client.dir/src/client_senior_admin.cpp.s

CMakeFiles/med_client.dir/src/common_for_all.cpp.o: CMakeFiles/med_client.dir/flags.make
CMakeFiles/med_client.dir/src/common_for_all.cpp.o: src/common_for_all.cpp
CMakeFiles/med_client.dir/src/common_for_all.cpp.o: CMakeFiles/med_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/med_client.dir/src/common_for_all.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/med_client.dir/src/common_for_all.cpp.o -MF CMakeFiles/med_client.dir/src/common_for_all.cpp.o.d -o CMakeFiles/med_client.dir/src/common_for_all.cpp.o -c /mnt/c/MedProject/MedScheduler/server+database/client/src/common_for_all.cpp

CMakeFiles/med_client.dir/src/common_for_all.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/med_client.dir/src/common_for_all.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/MedProject/MedScheduler/server+database/client/src/common_for_all.cpp > CMakeFiles/med_client.dir/src/common_for_all.cpp.i

CMakeFiles/med_client.dir/src/common_for_all.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/med_client.dir/src/common_for_all.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/MedProject/MedScheduler/server+database/client/src/common_for_all.cpp -o CMakeFiles/med_client.dir/src/common_for_all.cpp.s

CMakeFiles/med_client.dir/src/get_by_phone.cpp.o: CMakeFiles/med_client.dir/flags.make
CMakeFiles/med_client.dir/src/get_by_phone.cpp.o: src/get_by_phone.cpp
CMakeFiles/med_client.dir/src/get_by_phone.cpp.o: CMakeFiles/med_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/med_client.dir/src/get_by_phone.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/med_client.dir/src/get_by_phone.cpp.o -MF CMakeFiles/med_client.dir/src/get_by_phone.cpp.o.d -o CMakeFiles/med_client.dir/src/get_by_phone.cpp.o -c /mnt/c/MedProject/MedScheduler/server+database/client/src/get_by_phone.cpp

CMakeFiles/med_client.dir/src/get_by_phone.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/med_client.dir/src/get_by_phone.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/MedProject/MedScheduler/server+database/client/src/get_by_phone.cpp > CMakeFiles/med_client.dir/src/get_by_phone.cpp.i

CMakeFiles/med_client.dir/src/get_by_phone.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/med_client.dir/src/get_by_phone.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/MedProject/MedScheduler/server+database/client/src/get_by_phone.cpp -o CMakeFiles/med_client.dir/src/get_by_phone.cpp.s

CMakeFiles/med_client.dir/src/main.cpp.o: CMakeFiles/med_client.dir/flags.make
CMakeFiles/med_client.dir/src/main.cpp.o: src/main.cpp
CMakeFiles/med_client.dir/src/main.cpp.o: CMakeFiles/med_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/med_client.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/med_client.dir/src/main.cpp.o -MF CMakeFiles/med_client.dir/src/main.cpp.o.d -o CMakeFiles/med_client.dir/src/main.cpp.o -c /mnt/c/MedProject/MedScheduler/server+database/client/src/main.cpp

CMakeFiles/med_client.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/med_client.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/MedProject/MedScheduler/server+database/client/src/main.cpp > CMakeFiles/med_client.dir/src/main.cpp.i

CMakeFiles/med_client.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/med_client.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/MedProject/MedScheduler/server+database/client/src/main.cpp -o CMakeFiles/med_client.dir/src/main.cpp.s

# Object files for target med_client
med_client_OBJECTS = \
"CMakeFiles/med_client.dir/src/client_auth.cpp.o" \
"CMakeFiles/med_client.dir/src/client_doctor.cpp.o" \
"CMakeFiles/med_client.dir/src/client_junior_admin.cpp.o" \
"CMakeFiles/med_client.dir/src/client_patient.cpp.o" \
"CMakeFiles/med_client.dir/src/client_senior_admin.cpp.o" \
"CMakeFiles/med_client.dir/src/common_for_all.cpp.o" \
"CMakeFiles/med_client.dir/src/get_by_phone.cpp.o" \
"CMakeFiles/med_client.dir/src/main.cpp.o"

# External object files for target med_client
med_client_EXTERNAL_OBJECTS =

med_client: CMakeFiles/med_client.dir/src/client_auth.cpp.o
med_client: CMakeFiles/med_client.dir/src/client_doctor.cpp.o
med_client: CMakeFiles/med_client.dir/src/client_junior_admin.cpp.o
med_client: CMakeFiles/med_client.dir/src/client_patient.cpp.o
med_client: CMakeFiles/med_client.dir/src/client_senior_admin.cpp.o
med_client: CMakeFiles/med_client.dir/src/common_for_all.cpp.o
med_client: CMakeFiles/med_client.dir/src/get_by_phone.cpp.o
med_client: CMakeFiles/med_client.dir/src/main.cpp.o
med_client: CMakeFiles/med_client.dir/build.make
med_client: /usr/lib/x86_64-linux-gnu/libcurl.so
med_client: CMakeFiles/med_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable med_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/med_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/med_client.dir/build: med_client
.PHONY : CMakeFiles/med_client.dir/build

CMakeFiles/med_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/med_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/med_client.dir/clean

CMakeFiles/med_client.dir/depend:
	cd /mnt/c/MedProject/MedScheduler/server+database/client && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/MedProject/MedScheduler/server+database/client /mnt/c/MedProject/MedScheduler/server+database/client /mnt/c/MedProject/MedScheduler/server+database/client /mnt/c/MedProject/MedScheduler/server+database/client /mnt/c/MedProject/MedScheduler/server+database/client/CMakeFiles/med_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/med_client.dir/depend

