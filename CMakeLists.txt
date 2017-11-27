
# CMakeLists.txt, generated gencmake.pl, on 2017/11/25 12:31:44
cmake_minimum_required( VERSION 2.8.8 )

# CMakeScripts or use the ones that come by default with CMake.
# set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})

project( crypto )

# The version number.
set( crypto_MAJOR 3 )
set( crypto_MINOR 0 )
set( crypto_POINT 0 )


# Allow developer to select is Dynamic or static library built
set( LIB_TYPE STATIC )  # set default static
option( BUILD_SHARED_LIB "Set ON to build shared Library (DLL)" OFF )


if(CMAKE_COMPILER_IS_GNUCXX)
    set( WARNING_FLAGS -Wall )
endif(CMAKE_COMPILER_IS_GNUCXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
   set( WARNING_FLAGS "-Wall -Wno-overloaded-virtual" )
endif() 

if(WIN32 AND MSVC)
    # turn off various warnings - none needed in this compile
    set(WARNING_FLAGS "${WARNING_FLAGS} /wd4996")
    # foreach(warning 4244 4251 4267 4275 4290 4786 4305)
    #     set(WARNING_FLAGS "${WARNING_FLAGS} /wd${warning}")
    # endforeach(warning)
    set( MSVC_FLAGS "-DNOMINMAX -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D__CRT_NONSTDC_NO_WARNINGS" )
    # if (${MSVC_VERSION} EQUAL 1600)
    #    set( MSVC_LD_FLAGS "/FORCE:MULTIPLE" )
    # endif (${MSVC_VERSION} EQUAL 1600)
    #set( NOMINMAX 1 )
    # to distinguish between debug and release libs
    set( CMAKE_DEBUG_POSTFIX "d" )
else()
    # items for unix
    set(WARNING_FLAGS "${WARNING_FLAGS} -Wno-pointer-sign")
endif()

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LD_FLAGS}" )

# configuration file, if needed
# configure_file( ${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h )
# add_definitions( -DHAVE_CONFIG_H )
# include_directories( ${CMAKE_BINARY_DIR} )

if(BUILD_SHARED_LIB)
   set(LIB_TYPE SHARED)
   message(STATUS "*** Building DLL library ${LIB_TYPE}")
else()
   message(STATUS "*** Option BUILD_SHARED_LIB is OFF ${LIB_TYPE}")
endif()
# Total libs 1, several exes
#################################################
##### LIBRARIES 1 #####
#################################################
set(name cry_utils)
set(dir src)
set(${name}_SRCS
    ${dir}/cry_utils.c
    )
set(${name}_HDRS
    ${dir}/cry_utils.h
    )
add_library( ${name} ${LIB_TYPE} ${${name}_SRCS} ${${name}_HDRS} )
list(APPEND add_LIBS ${name})
# deal with install, if any...
# but inteneded to be a local only static library of common functions...
#install( TARGETS ${name}
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib )
#install(FILES ${${name}_HDRS} DESTINATION include)

#################################################
##### EXECUTABLE 3 #####
#################################################
set(name sha256_test)
set(dir src)
set(${name}_SRCS
    ${dir}/sha256.c
    ${dir}/sha256_test.c
    )
set(${name}_HDRS
    ${dir}/sha256.h
    )
add_executable( ${name} ${${name}_SRCS} ${${name}_HDRS} )
if (add_LIBS)
    target_link_libraries( ${name} ${add_LIBS} )
endif ()
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()
# deal with install, if any...
install( TARGETS ${name} DESTINATION bin )

set(name sha1_test)
set(dir src)
set(${name}_SRCS
    ${dir}/sha1.c
    ${dir}/sha1_test.c
    )
set(${name}_HDRS
    ${dir}/sha1.h
    )
add_executable( ${name} ${${name}_SRCS} ${${name}_HDRS} )
if (add_LIBS)
    target_link_libraries( ${name} ${add_LIBS} )
endif ()
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()
# deal with install, if any...
install( TARGETS ${name} DESTINATION bin )

set(name md5_test)
set(dir src)
set(${name}_SRCS
    ${dir}/md5.c
    ${dir}/md5_test.c
    )
set(${name}_HDRS
    ${dir}/md5.h
    )
add_executable( ${name} ${${name}_SRCS} ${${name}_HDRS} )
if (add_LIBS)
    target_link_libraries( ${name} ${add_LIBS} )
endif ()
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()
# deal with install, if any...
install( TARGETS ${name} DESTINATION bin )

if (ADD_OTHER_EXES)
# These have yet to be built implemented... WIP...

set(name crypto-algotithms-fork)
set(dir src)
set(${name}_SRCS
    ${dir}/aes.c
    ${dir}/aes_test.c
    ${dir}/arcfour.c
    ${dir}/arcfour_test.c
    ${dir}/base64.c
    ${dir}/base64_test.c
    ${dir}/blowfish.c
    ${dir}/blowfish_test.c
    ${dir}/des.c
    ${dir}/des_test.c
    ${dir}/md2.c
    ${dir}/md2_test.c
    ${dir}/rot-13.c
    ${dir}/rot-13_test.c
    )
set(${name}_HDRS
    ${dir}/aes.h
    ${dir}/arcfour.h
    ${dir}/base64.h
    ${dir}/blowfish.h
    ${dir}/des.h
    ${dir}/md2.h
    ${dir}/rot-13.h
    )
add_executable( ${name} ${${name}_SRCS} ${${name}_HDRS} )
if (add_LIBS)
    target_link_libraries( ${name} ${add_LIBS} )
endif ()
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()
# deal with install, if any...
#install( TARGETS ${name} DESTINATION bin )
endif ()

# eof
