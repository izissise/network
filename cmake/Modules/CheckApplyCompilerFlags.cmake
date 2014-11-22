#
# This only works if included after the line containing the PROJECT call in
# CMake.
#
# COMPILER_CXX_FLAGS(FLAGS;FLAGS_RELEASE;FLAGS_DEBUG)
#
# Copyright (C) 2011 by Hugues
#
# This file may be licensed under the terms of the
# GNU Lesser General Public License Version 3 (the ``LGPL''),
# or (at your option) any later version.
#
# Software distributed under the License is distributed
# on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
# express or implied. See the LGPL for the specific language
# governing rights and limitations.
#
# You should have received a copy of the LGPL along with this
# program. If not, go to http://www.gnu.org/licenses/lgpl.html
# or write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

# 

INCLUDE(CheckCXXCompilerFlag)
INCLUDE(CheckCCompilerFlag)
INCLUDE(ParseArguments)

MACRO(COMPILER_FLAGS)
	PARSE_ARGUMENTS(ARG "FLAGS;FLAGS_RELEASE;FLAGS_DEBUG" "" ${ARGN})
	
	set(CMAKE_CXX_FLAGS "")
	set(CMAKE_C_FLAGS "")
	#General flags
	FOREACH(F ${ARG_FLAGS})
	  #we make a good unique variable name for the check
	  STRING(REGEX REPLACE "[-+=]" "_" F_CHECK_NAME ${F})
	  SET(F_CHECK_CXX_NAME CHECK_CXX_FLAG${F_CHECK_NAME})
	  SET(F_CHECK_C_NAME CHECK_C_FLAG${F_CHECK_NAME})
	  #now we do the check
	  CHECK_CXX_COMPILER_FLAG(${F} ${F_CHECK_CXX_NAME})
	  CHECK_C_COMPILER_FLAG(${F} ${F_CHECK_C_NAME})
	  #if the check pass, lets add the definition
	  IF(${F_CHECK_CXX_NAME})
		set(COMPILER_FLAGS_CXX_FLAGS "${COMPILER_FLAGS_CXX_FLAGS} ${F}")
	  ENDIF(${F_CHECK_CXX_NAME})
	  IF(${F_CHECK_C_NAME})
		set(COMPILER_FLAGS_C_FLAGS "${COMPILER_FLAGS_C_FLAGS} ${F}")
	  ENDIF(${F_CHECK_C_NAME})
	ENDFOREACH(F ${ARG_FLAGS})
 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS_CXX_FLAGS}")
 	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILER_FLAGS_C_FLAGS}")
 	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMPILER_FLAGS_CXX_FLAGS}")
 	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${COMPILER_FLAGS_C_FLAGS}")
 	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMPILER_FLAGS_CXX_FLAGS}")
 	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${COMPILER_FLAGS_C_FLAGS}")
 	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${COMPILER_FLAGS_CXX_FLAGS}")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${COMPILER_FLAGS_C_FLAGS}")
	
	#Release flags
	FOREACH(F ${ARG_FLAGS_RELEASE})
	  #we make a good unique variable name for the check
	  STRING(REGEX REPLACE "[-+=]" "_" F_CHECK_NAME ${F})
	  SET(F_CHECK_CXX_NAME CHECK_CXX_FLAG${F_CHECK_NAME})
	  SET(F_CHECK_C_NAME CHECK_C_FLAG${F_CHECK_NAME})
	  #now we do the check
	  CHECK_CXX_COMPILER_FLAG(${F} ${F_CHECK_CXX_NAME})
	  CHECK_C_COMPILER_FLAG(${F} ${F_CHECK_C_NAME})
	  #if the check pass, lets add the definition
	  IF(${F_CHECK_CXX_NAME})
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${F}")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${F}")
	  ENDIF(${F_CHECK_CXX_NAME})
	  IF(${F_CHECK_C_NAME})
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${F}")
		set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${F}")
	  ENDIF(${F_CHECK_C_NAME})
	ENDFOREACH(F ${ARG_FLAGS})
	
	#Debug flags
	FOREACH(F ${ARG_FLAGS_DEBUG})
	  #we make a good unique variable name for the check
	  STRING(REGEX REPLACE "[-+=]" "_" F_CHECK_NAME ${F})
	  SET(F_CHECK_CXX_NAME CHECK_CXX_FLAG${F_CHECK_NAME})
	  SET(F_CHECK_C_NAME CHECK_C_FLAG${F_CHECK_NAME})
	  #now we do the check
	  CHECK_CXX_COMPILER_FLAG(${F} ${F_CHECK_CXX_NAME})
	  CHECK_C_COMPILER_FLAG(${F} ${F_CHECK_C_NAME})
	  #if the check pass, lets add the definition
	  IF(${F_CHECK_CXX_NAME})
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${F}")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${F}")
	  ENDIF(${F_CHECK_CXX_NAME})
	  IF(${F_CHECK_C_NAME})
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${F}")
		set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${F}")
	  ENDIF(${F_CHECK_C_NAME})
	ENDFOREACH(F ${ARG_FLAGS})

ENDMACRO()
