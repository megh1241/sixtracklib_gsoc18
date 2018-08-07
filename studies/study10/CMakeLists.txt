cmake_minimum_required( VERSION 3.8 FATAL_ERROR )

project( sixtracklib_gsoc2018_study10 )

# We know where to find the SixTracklib installation ->
# set the SixTracklib_DIR variable to help CMake finding it:

set( SixTracklib_DIR "${CMAKE_SOURCE_DIR}/external/lib/cmake" )
find_package( SixTracklib REQUIRED )
find_package( PythonInterp 2.7 REQUIRED )
find_package( PythonLibs 2.7 REQUIRED )
add_executable(run_sample_fodo run_sample_fodo.c)
include_directories(${PYTHON_INCLUDE_DIRS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb -L/usr/local/cuda -L/home/mmadhyas/anaconda2/envs/my_env/lib/libpython2.7.so -L/home/mmadhyas/anaconda2/envs/my_env/lib/python2.7/config/libpython2.7.a -I/home/mmadhyas/anaconda2/envs/my_env/include/python2.7 -I/home/mmadhyas/anaconda2/envs/my_env/include/python2.7 -fno-strict-aliasing -g -O2 -DNDEBUG -g -lutil -lm -Xlinker -export-dynamic -lpthread -ldl -O3 -Wall -Wstrict-prototypes")
# convert sample_fodo.c into a shared library:

add_library( sample_fodo SHARED sample_fodo.c )

target_link_libraries(sample_fodo SixTracklib::sixtrack)
target_link_libraries(sample_fodo -L/home/mmadhyas/anaconda2/envs/my_env/lib/python2.7/config/libpython2.7.a -L/home/mmadhyas/anaconda2/lib/python2.7/config/libpython2.7.a)
set_target_properties(
        sample_fodo PROPERTIES
                    LINKER_LANGUAGE C
                    POSITION_INDEPENDENT_CODE ON
                    C_STANDARD 99
                    C_STANDARD_REQUIRED ON
                    DEBUG_POSTFIX d )

set_property( TARGET sample_fodo
              APPEND PROPERTY COMPILE_OPTIONS -Wall -pedantic )

set(PYTHON_LIBRARY ${PYTHON_LIBRARY} "/home/mmadhyas/anaconda2/envs/my_env/lib/libpython2.7.so/")
set(PYTHON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/home/mmadhyas/anaconda2/envs/my_env/include/python2.7/")
set(PYTHON_INCLUDE_DIR ${PYTHON_INCLUDE_DIR} CACHE FILEPATH "" FORCE)
find_package(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
# -----------------------------------------------------------------------------
# use sample_fodo in an executable:


#Since sample_fodo explicitly depends on SixTracklib::sixtrack, we use do not
# have to add SixTracklib::sixtrack library explicitly as a dependency of the
# executable
target_link_libraries(run_sample_fodo sample_fodo -L/home/mmadhyas/anaconda2/envs/my_env/lib/python2.7/config/libpython2.7.a -L/home/mmadhyas/anaconda2/lib/python2.7/config/libpython2.7.a)
#target_link_libraries(run_sample_fodo sample_fodo -L/home/mmadhyas/anaconda2/envs/my_env/lib/libpython2.7.so -L/home/mmadhyas/anaconda2/envs/my_env/lib/python2.7/config/libpython2.7.a -L/home/mmadhyas/anaconda2/lib/python2.7/config/libpython2.7.a)
