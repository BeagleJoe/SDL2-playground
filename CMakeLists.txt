#
#==============================================================================
#
#    file                 : CMakeLists.txt
#    created              : May 23 2015
#    copyright            : (C) 2015 Joe Thompson
#    email                : beaglejoe@users.sourceforge.net
#    version              : $Id:$
#
#==============================================================================
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#==============================================================================
#
cmake_minimum_required(VERSION 2.8)

set(OPTION_SDL2 true CACHE BOOL "Build with SDL2 instead lib SDL 1.2")
set(PROGAM_NAME "Utility" CACHE STRING "Program Name")
#set(CMAKE_INSTALL_PREFIX "./installed" CACHE PATH "Install location")


project(${PROGAM_NAME})

set(_SOURCES main.cpp)
set(_HEADERS main.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Setup the install prefix.
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  IF(WIN32)
    SET(CMAKE_INSTALL_PREFIX "./installed" CACHE PATH "Prefix prepended to install directories" FORCE)
  ELSE(WIN32)
    SET(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Prefix prepended to install directories" FORCE)
  ENDIF(WIN32)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

if(MSVC)
   set(_OPTION_CUSTOM_3RDPARTY true) # Always needed for MSVC compilers.
   set(OPTION_CUSTOM_3RDPARTY ${_OPTION_CUSTOM_3RDPARTY} CACHE BOOL "Set to ON to use 3rdParty prebuilt API located in <PROJECT_SOURCE_DIR>/../3rdparty")
endif(MSVC)
#mark_as_advanced(OPTION_CUSTOM_3RDPARTY)


if(OPTION_CUSTOM_3RDPARTY)
   set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${PROJECT_SOURCE_DIR}/../3rdParty/")
   set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${PROJECT_SOURCE_DIR}/../3rdParty/include/SDL2/")
endif(OPTION_CUSTOM_3RDPARTY)

if(OPTION_SDL2)
   find_package(SDL2 REQUIRED)
endif(OPTION_SDL2)

add_executable(${PROJECT_NAME} ${_SOURCES} ${_HEADERS})

if(OPTION_SDL2)
   if(SDL2_FOUND)
      include_directories(${SDL2_INCLUDE_DIR})
      target_link_libraries(${PROJECT_NAME} ${SDL2MAIN_LIBRARY})
      target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
   else(SDL2_FOUND)
      message(FATAL_ERROR "Cannot find SDL2 files")
   endif(SDL2_FOUND)
endif(OPTION_SDL2)

#install(FILES icon.bmp DESTINATION bin) 
if(OPTION_SDL2)
   if(WIN32)
      install(PROGRAMS ${SDL2_INCLUDE_DIR}/../../bin/sdl2.dll DESTINATION bin)
   endif(WIN32)
endif(OPTION_SDL2)
install(TARGETS ${PROJECT_NAME} DESTINATION bin) 