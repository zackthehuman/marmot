#
# The MIT License (MIT)
# 
# Copyright (c) 2013 Zack Mulgrew
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
        # Protip: run cmake like this: cmake -G "<generator>" -H. -Bbuild
        message(FATAL_ERROR "Prevented in-tree build.")
endif()

cmake_minimum_required(VERSION 2.8)

project(marmot)

set(MARMOT_EXE_NAME marmot)
set(MARMOT_LIBRARY_NAME libmarmot)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(MARMOT_X64 TRUE)
        set(MARMOT_TARGET_PLATFORM x64)
else()
        set(MARMOT_X86 TRUE)
        set(MARMOT_TARGET_PLATFORM x86)
endif()

message(STATUS "Target platform: ${MARMOT_TARGET_PLATFORM}")

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -O")

#
# main
#
set(MARMOT_SRC_MAIN
    src/Main.cpp
)

set(MARMOT_HEADER_MAIN
    include/Main.hpp
)

source_group("main" FILES ${MARMOT_SRC_MAIN} ${MARMOT_HEADER_MAIN})

#
# library
#
set(MARMOT_SRC_LIBRARY
    src/marmot/VM.cpp
)

set(MARMOT_SRC_LIBRARY
    include/marmot/VM.hpp
    include/marmot/Error.hpp
    include/marmot/Proxy.hpp
    include/marmot/Reference.hpp
    include/marmot/Stack.hpp
    include/marmot/State.hpp
    include/marmot/Table.hpp
)

source_group("library" FILES ${MARMOT_SRC_LIBRARY} ${MARMOT_HEADER_LIBRARY})

set(SQUIRREL_DIR "${PROJECT_SOURCE_DIR}/extlibs/squirrel")
set(SQUIRREL_FILES
    # Squirrel core
    ${SQUIRREL_DIR}/squirrel/sqapi.cpp
    ${SQUIRREL_DIR}/squirrel/sqbaselib.cpp
    ${SQUIRREL_DIR}/squirrel/sqclass.cpp
    ${SQUIRREL_DIR}/squirrel/sqcompiler.cpp
    ${SQUIRREL_DIR}/squirrel/sqdebug.cpp
    ${SQUIRREL_DIR}/squirrel/sqfuncstate.cpp
    ${SQUIRREL_DIR}/squirrel/sqlexer.cpp
    ${SQUIRREL_DIR}/squirrel/sqmem.cpp
    ${SQUIRREL_DIR}/squirrel/sqobject.cpp
    ${SQUIRREL_DIR}/squirrel/sqstate.cpp
    ${SQUIRREL_DIR}/squirrel/sqtable.cpp
    ${SQUIRREL_DIR}/squirrel/sqvm.cpp

    # Squirrel standard library
    ${SQUIRREL_DIR}/sqstdlib/sqstdaux.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdblob.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdio.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdmath.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdrex.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdstream.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdstring.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdsystem.cpp
)

#
# catch
#
set(CATCH_DIR "${PROJECT_SOURCE_DIR}/extlibs/catch")

#
# test
#
set(MARMOT_SRC_TEST
    src/test/Test.cpp
    src/test/TestFunction.cpp
    src/test/TestReference.cpp
    src/test/TestStack.cpp
    src/test/TestState.cpp
    src/test/TestTable.cpp
)

set(MARMOT_HEADER_TEST
    include/test/Test.hpp
)

include_directories(
    include
    extlibs
    ${SQUIRREL_DIR}/include
    ${SQUIRREL_DIR}/squirrel
    ${SQUIRREL_DIR}/sqstdlib
)

#
# marmot (executable)
#
add_executable(
    ${MARMOT_EXE_NAME}
    ${MARMOT_SRC_MAIN}
    ${MARMOT_SRC_LIBRARY}
    ${SQUIRREL_FILES}
)

#
# test-marmot (executable)
#
add_executable(
    test-${MARMOT_EXE_NAME}
    ${MARMOT_SRC_TEST}
    ${MARMOT_SRC_LIBRARY}
    ${SQUIRREL_FILES}
)

#
# Apple-specific stuff
#
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -stdlib=libc++")
endif(APPLE)