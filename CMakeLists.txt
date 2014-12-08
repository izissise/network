cmake_minimum_required(VERSION 2.8)

project(NetworkLib)

set(NAME_LIB "network")

set(INCLUDE_DIRECTORY "${PROJECT_SOURCE_DIR}/inc/")
set(SRC_DIRECTORY "${PROJECT_SOURCE_DIR}/src/")

set(DOC_PATH "${PROJECT_SOURCE_DIR}/doc")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

INCLUDE(CMakeDependentOption)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(InstallFilesRecursive)
INCLUDE(EnableCoverageReport)
INCLUDE(EnableSlocCount)
INCLUDE(GenerateDoxygen)
INCLUDE(GenerateCppcheck)
INCLUDE(GenerateCccc)
INCLUDE(GenerateValgrind)
INCLUDE(DefineProjectVersion)
INCLUDE(CurrentDate)
INCLUDE(DemangleCppABI)
INCLUDE(CheckApplyCompilerFlags)

# --- user options ---
OPTION(BUILD_TESTS "Decide if the test suite shall be built or not." OFF)
CMAKE_DEPENDENT_OPTION(BUILD_TESTS_COVERAGE "Decide if a coverage report should be generated." ON "BUILD_TESTS" OFF)
CMAKE_DEPENDENT_OPTION(VALGRIND_CHECK "Decide if valgrind should be executed with the test binary to check for memory leaks." ON "BUILD_TESTS" OFF)
OPTION(CODE_METRICS "Decide if code's metrics should be generated." OFF)
OPTION(CODE_CHECK "Decide if code should be checked for common errors." OFF)
OPTION(GENERATE_DOC "Decide if documentation should be generated." OFF)

# --- global compiler flags ---
if (UNIX)
  COMPILER_FLAGS(FLAGS "-pipe" "-Wall" "-Wextra"
			   "--std=c++11"
			   "-Wchar-subscripts" "-Winit-self" "-Wswitch-enum"
			   "-Wstrict-aliasing=3"
			   "-Wundef" "-Wno-endif-labels"
			   "-Wshadow" "-Wno-builtin-macro-redefined"
			   "-Wmissing-declarations"
			   "-Wdocumentation"
			   "-Wno-switch-enum"
		FLAGS_RELEASE  "-march=native" "-O2"
		FLAGS_DEBUG    "-DNDEBUG" "-g3"
			   )
elseif (MSVC)
  COMPILER_FLAGS(FLAGS "/wd4251" # needs to have dll-interface to be used by clients
		       "/wd4275" # exported class derived from non-exported class
			   )
endif()

# --- global definitions ---
DEFINE_PROJECT_VERSION(PROJECT 0 0 1 "snapshot")
CURRENT_DATE(PROJECT_BUILD_DATE)

# All source files
set(lib_source_files "src/Error.cpp"
					 "src/ANetwork.cpp"
					 "src/NetworkFactory.cpp")

set(BUILDTARGET "Unknown")
if (WIN32)
set(BUILDTARGET "WIN32")
set(lib_source_files ${lib_source_files}
			 "src/Win/WinNetwork.cpp"
		     "src/Win/WinNetworkBasicSocket.cpp"
		     "src/Win/WinNetworkConnectSocket.cpp"
		     "src/Win/WinNetworkListenSocket.cpp"
		     "src/Win/WinSocket.cpp")
elseif(UNIX)
set(BUILDTARGET "UNIX")
set(lib_source_files ${lib_source_files}
		     "src/inet_accept.c"
		     "src/inet_asynchronous_connect.c"
		     "src/inet.c"
		     "src/inet_reuse.c"
		     "src/liste.c"
		     "src/liste_find.c"
		     "src/ring_buffer.c"
		     "src/ring_buffer_fill.c"
		     "src/select.c"
		     "src/Unix/UnixNetwork.cpp"
		     "src/Unix/UnixNetworkBasicSocket.cpp"
		     "src/Unix/UnixNetworkConnectSocket.cpp"
		     "src/Unix/UnixNetworkListenSocket.cpp"
		     "src/Unix/UnixSocket.cpp")
endif()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "inc/Config.h.in"
  "inc/Config.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/inc")

file(
  GLOB_RECURSE
  lib_include_files
  inc/*
)

include_directories("${INCLUDE_DIRECTORY}")

add_library(
# executable name
  ${NAME_LIB} STATIC
# source files
  ${lib_source_files}
  ${lib_include_files}
)

set_target_properties(${NAME_LIB} PROPERTIES
   PUBLIC_HEADER "${lib_include_files}"
)
   
install(TARGETS ${NAME_LIB}
   LIBRARY DESTINATION lib COMPONENT shlibs
   RUNTIME DESTINATION bin COMPONENT shlibs
   ARCHIVE DESTINATION lib COMPONENT dev
   PUBLIC_HEADER DESTINATION include COMPONENT dev
)

file(
  GLOB_RECURSE
  test_files
  test/*
)

# --- enable unit tests if desired ---
if(BUILD_TESTS)
  if(NOT CPPUNIT_FOUND)
    MESSAGE(WARNING "CppUnit library can't be found, Unit test won't be compiled")
  else()
    add_executable(
    # executable name
      ${TEST_EXEC}
    # source files
     ${test_files}
    )

    if(BUILD_TESTS_COVERAGE)
      # --- coverage report ---
      ENABLE_COVERAGE_REPORT(TARGETS ${TEST_EXEC} TESTS ${test_files})
    endif()

    if(VALGRIND_CHECK)
      # --- Valgrind --- #
      GENERATE_VALGRIND(BIN "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC}")
    endif()

    set(TEST_REPORT_FILE "cpptestresults.xml")
    # Execute ${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC} binary to execute tests
    ADD_CUSTOM_COMMAND(OUTPUT ${TEST_REPORT_FILE}
                           DEPENDS ${TEST_EXEC}
                           COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC}" ARGS "--xml-file=${TEST_REPORT_FILE}"
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    ADD_CUSTOM_TARGET(launchtest DEPENDS ${TEST_REPORT_FILE})

    include_directories(${CPPUNIT_INCLUDE_DIRS})
    target_link_libraries (${TEST_EXEC} ${CPPUNIT_LIBRARY} ${NAME_LIB})

  endif()
endif()

#Extra targets:
if(CODE_CHECK)
  # --- cppcheck ---
  GENERATE_CPPCHECK(SOURCES ${SRC_DIRECTORY}
                            "${PROJECT_SOURCE_DIR}/test"
                            ENABLE_IDS style
                            INLINE_SUPPRESSION)
endif()

if(CODE_METRICS)
  # --- cccc ---
  GENERATE_CCCC(SOURCES ${SRC_DIRECTORY} ${INCLUDE_DIRECTORY}
                        "${PROJECT_SOURCE_DIR}/test")
  # --- SlocCount --- #
  ENABLE_SLOCCOUNT(FOLDERS ${SRC_DIRECTORY} ${INCLUDE_DIRECTORY}
                        "${PROJECT_SOURCE_DIR}/test")
endif()

if(GENERATE_DOC)
  # --- Doxygen --- #
  GENERATE_DOXYGEN(SOURCES ${SRC_DIRECTORY} ${INCLUDE_DIRECTORY}
                        "${PROJECT_SOURCE_DIR}/test" ${DOC_PATH}
                        VERSION "${PROJECTVERSION}")
endif()

# Export library dependencies
#############################
export(TARGETS ${NAME_LIB}
   NAMESPACE NETWORKLIB_
   FILE ${CMAKE_BINARY_DIR}/NetworkLibLibraryDepends.cmake
)

# create NetworkLibConfig.cmake for the build tree
#########################################
# SET TO THE PREPROCESSOR FLAGS REQUIRED TO USE PROJECT NetworkLib
set(CONFIG_DEFINITIONS)
# SET TO WHATEVER INCLUDE PATH IS REQUIRED TO USE NetworkLib'S BINARY TREE
set(CONFIG_HEADER_PATH "${CMAKE_SOURCE_DIR}/inc" "${CMAKE_CURRENT_BINARY_DIR}/inc")
# SET TO WHATEVER LIBRARY PATH IS REQUIRED TO USE NetworkLib'S BINARY TREE
set(CONFIG_LIBRARY_PATH "${CMAKE_CURRENT_BINARY_DIR}")

set(CONFIG_USE_FILE_PATH ${CMAKE_SOURCE_DIR})
configure_file(
   ${CMAKE_SOURCE_DIR}/cmake/NetworkLibConfig.cmake.in
   ${CMAKE_BINARY_DIR}/NetworkLibConfig.cmake
   @ONLY
)

# create NetworkLibConfig.cmake for the install tree
###########################################
set(CONFIG_HEADER_PATH "${CMAKE_INSTALL_PREFIX}/include")
set(CONFIG_LIBRARY_PATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CONFIG_USE_FILE_PATH "${CMAKE_INSTALL_PREFIX}/share/cmake/NetworkLib" )
configure_file(
   ${CMAKE_SOURCE_DIR}/cmake/NetworkLibConfig.cmake.in
   ${CMAKE_BINARY_DIR}/InstallFiles/NetworkLibConfig.cmake
   @ONLY
)
   
# install the CMake config files
################################
install(FILES
   "${CMAKE_BINARY_DIR}/InstallFiles/NetworkLibConfig.cmake"
   "${CMAKE_BINARY_DIR}/NetworkLibBuildSettings.cmake"
   DESTINATION share/cmake/NetworkLib
   COMPONENT dev
)
