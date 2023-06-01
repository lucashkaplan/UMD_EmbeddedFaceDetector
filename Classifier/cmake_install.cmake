# Install script for directory: /homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/src/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test01/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test02/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test03/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test04/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test05/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test06/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test07/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test08/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test09/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test10/cmake_install.cmake")
  include("/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/test/test11/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/homes/l/h/lhkaplan/home/enee408m/project/team3/Classifier/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
