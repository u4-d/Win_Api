# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\Tools\cMake\bin\cmake.exe

# The command to remove a file.
RM = D:\Tools\cMake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\code\Cpp\Win_Api

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\code\Cpp\Win_Api\build

# Include any dependencies generated for this target.
include Chap03\HelloWin\CMakeFiles\HelloWin.dir\depend.make
# Include any dependencies generated by the compiler for this target.
include Chap03\HelloWin\CMakeFiles\HelloWin.dir\compiler_depend.make

# Include the progress variables for this target.
include Chap03\HelloWin\CMakeFiles\HelloWin.dir\progress.make

# Include the compile flags for this target's objects.
include Chap03\HelloWin\CMakeFiles\HelloWin.dir\flags.make

Chap03\HelloWin\CMakeFiles\HelloWin.dir\codegen:
.PHONY : Chap03\HelloWin\CMakeFiles\HelloWin.dir\codegen

Chap03\HelloWin\CMakeFiles\HelloWin.dir\HelloWin.cpp.obj: Chap03\HelloWin\CMakeFiles\HelloWin.dir\flags.make
Chap03\HelloWin\CMakeFiles\HelloWin.dir\HelloWin.cpp.obj: D:\code\Cpp\Win_Api\Chap03\HelloWin\HelloWin.cpp
Chap03\HelloWin\CMakeFiles\HelloWin.dir\HelloWin.cpp.obj: Chap03\HelloWin\CMakeFiles\HelloWin.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\code\Cpp\Win_Api\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Chap03/HelloWin/CMakeFiles/HelloWin.dir/HelloWin.cpp.obj"
	cd D:\code\Cpp\Win_Api\build\Chap03\HelloWin
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\HelloWin.dir\HelloWin.cpp.obj.d --working-dir=D:\code\Cpp\Win_Api\build\Chap03\HelloWin --filter-prefix="ע��: �����ļ�:  " -- D:\Tools\MSVC\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\HelloWin.dir\HelloWin.cpp.obj /FdCMakeFiles\HelloWin.dir\ /FS -c D:\code\Cpp\Win_Api\Chap03\HelloWin\HelloWin.cpp
<<
	cd D:\code\Cpp\Win_Api\build

Chap03\HelloWin\CMakeFiles\HelloWin.dir\HelloWin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HelloWin.dir/HelloWin.cpp.i"
	cd D:\code\Cpp\Win_Api\build\Chap03\HelloWin
	D:\Tools\MSVC\bin\Hostx64\x64\cl.exe > CMakeFiles\HelloWin.dir\HelloWin.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\code\Cpp\Win_Api\Chap03\HelloWin\HelloWin.cpp
<<
	cd D:\code\Cpp\Win_Api\build

Chap03\HelloWin\CMakeFiles\HelloWin.dir\HelloWin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HelloWin.dir/HelloWin.cpp.s"
	cd D:\code\Cpp\Win_Api\build\Chap03\HelloWin
	D:\Tools\MSVC\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\HelloWin.dir\HelloWin.cpp.s /c D:\code\Cpp\Win_Api\Chap03\HelloWin\HelloWin.cpp
<<
	cd D:\code\Cpp\Win_Api\build

# Object files for target HelloWin
HelloWin_OBJECTS = \
"CMakeFiles\HelloWin.dir\HelloWin.cpp.obj"

# External object files for target HelloWin
HelloWin_EXTERNAL_OBJECTS =

bin\HelloWin.exe: Chap03\HelloWin\CMakeFiles\HelloWin.dir\HelloWin.cpp.obj
bin\HelloWin.exe: Chap03\HelloWin\CMakeFiles\HelloWin.dir\build.make
bin\HelloWin.exe: Chap03\HelloWin\CMakeFiles\HelloWin.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\code\Cpp\Win_Api\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ..\..\bin\HelloWin.exe"
	cd D:\code\Cpp\Win_Api\build\Chap03\HelloWin
	D:\Tools\cMake\bin\cmake.exe -E vs_link_exe --msvc-ver=1942 --intdir=CMakeFiles\HelloWin.dir --rc="D:\Tools\MSVC\bin\Windows Kits\x64\rc.exe" --mt="D:\Tools\MSVC\bin\Windows Kits\x64\mt.exe" --manifests -- D:\Tools\MSVC\bin\Hostx64\x64\link.exe /nologo @CMakeFiles\HelloWin.dir\objects1.rsp @<<
 /out:..\..\bin\HelloWin.exe /implib:HelloWin.lib /pdb:D:\code\Cpp\Win_Api\build\bin\HelloWin.pdb /version:0.0 /machine:x64 /debug /INCREMENTAL /subsystem:windows  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<
	cd D:\code\Cpp\Win_Api\build

# Rule to build all files generated by this target.
Chap03\HelloWin\CMakeFiles\HelloWin.dir\build: bin\HelloWin.exe
.PHONY : Chap03\HelloWin\CMakeFiles\HelloWin.dir\build

Chap03\HelloWin\CMakeFiles\HelloWin.dir\clean:
	cd D:\code\Cpp\Win_Api\build\Chap03\HelloWin
	$(CMAKE_COMMAND) -P CMakeFiles\HelloWin.dir\cmake_clean.cmake
	cd D:\code\Cpp\Win_Api\build
.PHONY : Chap03\HelloWin\CMakeFiles\HelloWin.dir\clean

Chap03\HelloWin\CMakeFiles\HelloWin.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\code\Cpp\Win_Api D:\code\Cpp\Win_Api\Chap03\HelloWin D:\code\Cpp\Win_Api\build D:\code\Cpp\Win_Api\build\Chap03\HelloWin D:\code\Cpp\Win_Api\build\Chap03\HelloWin\CMakeFiles\HelloWin.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : Chap03\HelloWin\CMakeFiles\HelloWin.dir\depend

