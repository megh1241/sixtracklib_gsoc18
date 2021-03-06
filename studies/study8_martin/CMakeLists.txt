cmake_minimum_required( VERSION 3.3 )

project( sixtracklib_gsoc2018_study8 )

# We know where to find the SixTracklib installation ->
# set the SixTracklib_DIR variable to help CMake finding it:

set( SixTracklib_DIR "${CMAKE_SOURCE_DIR}/external/lib/cmake" )
find_package( SixTracklib REQUIRED )

if( NOT SixTrackTestlib_FOUND )
    message( FATAL_ERROR
             "SixTracklib::sixtrack_test testlib is required for "
             "test_particles, but was not found!" )
endif()

# -----------------------------------------------------------------------------
# use_blocks:

add_executable( use_blocks use_blocks.cpp )
target_link_libraries( use_blocks SixTracklib::sixtrack )


set_property( TARGET use_blocks PROPERTY CXX_STANDARD 11 )
set_property( TARGET use_blocks PROPERTY CXX_STANDARD_REQUIRED ON )
set_property( TARGET use_blocks PROPERTY COMPILE_OPTIONS -Wall -Werror -pedantic )


# -----------------------------------------------------------------------------
# test_particles:

add_executable( test_particles test_particles.cpp )

target_link_libraries( test_particles
                       SixTracklib::sixtrack_test SixTracklib::sixtrack )

set_property( TARGET test_particles PROPERTY CXX_STANDARD 11 )
set_property( TARGET test_particles PROPERTY CXX_STANDARD_REQUIRED ON )
set_property( TARGET test_particles PROPERTY COMPILE_OPTIONS -Wall -Werror -pedantic )

