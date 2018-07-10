# Study 10 - Use external sixtracklib installation for interfacing with PyHEADTAIL (WIP)
This is one quick & dirty way to use the upstream sixtracklib repository for a simple demo program or for checking/comparison, purposes. To this end, a recent upstream version of sixtracklib is installed into the "external" folder in this study, a C wrapper function providing the interface between PyHEADTAIL and sixtracklib is built into a shared library and an example application program is created to execute the wrapper code.
**Note**: The proper way to do this would be to use git submodules; this is not the proper way to do this, but probably easier to understand

## Prerequisities
* CMake 3.8 or higher (https://cmake.org/download)
* gcc and g++ 5.x or higher (C99/C++11 support)
* Cuda 7.5 or higher
* Optionally: Other dependencies for building the submodules of sixtracklib (googletest, OpenCL, etc.) -> not needed in this minimal environment

## Preparation: Installing sixtracklib into the external subfolder of study10
**Assumptions**:
* this document is located under ${STUDY10_DIR} 
* Upstream sixtracklib will be cloned into ${HOME}/git/sixtracklib

```
cd ${HOME}/git
git clone git@github.com:martinschwinzerl/sixtracklib.git 
cd sixtracklib
echo "to be sure that we are always on the right branch, checkout master!"
git checkout master 
```
Create a copy of `Settings.camke.default` under the name of `Settings.cmake`, edit it and enable the CUDA backend:
```
cp Settings.cmake.default Settings.cmake
vi Settings.cmake
head --lines=20 Settings.cmake
```
The output should be something like this:
```
# sixtracklib/Settings.cmake.default: default settings file
#
# Usage: if you want to deviate from the provided default values, create a copy
#        of this file named Settings.cmake in the same directory and perform the
#        modifications therein.

# ==============================================================================
# ====  Top Level Settings and Options:

# ------------------------------------------------------------------------------
martin@mycroft:~/git/sixtracklib$ head --lines=20 Settings.cmake
# sixtracklib/Settings.cmake.default: default settings file
#
# Usage: if you want to deviate from the provided default values, create a copy
#        of this file named Settings.cmake in the same directory and perform the
#        modifications therein.

# ==============================================================================
# ====  Top Level Settings and Options:

# ------------------------------------------------------------------------------
# ---- Backends and computing architectures:

option( SIXTRACKL_ENABLE_AUTOVECTORIZATION "Enable Autovectorization"       OFF )
option( SIXTRACKL_ENABLE_MANUAL_SIMD       "Enable manual SIMD backend"     OFF )
option( SIXTRACKL_ENABLE_OPENMP            "Enable OpenMP backend"          OFF )
option( SIXTRACKL_ENABLE_OPENCL            "Enable OpenCL backend"          OFF )
option( SIXTRACKL_ENABLE_CUDA              "Enable CUDA backend"            ON  )
option( SIXTRACKL_ENABLE_MPFR4             "Enable GMP/MPFR4 backend"       OFF )
```
Then create a build directory and configure the build process by providing the path to the external subdirectory in study 10:
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=${STUDY10_DIR}/external 
```
The output should contain no errors. Then build and install using the usual combination of 
```
make
make install
cd ${STUDY10_DIR}
```
Verify that the contents of external look something like this:
```
external/
├── include
│   └── sixtracklib
│       ├── common
│       │   ├── alignment.h
│       │   ├── beam_elements.h
│       │   ├── blocks.h
│       │   ├── compute_arch.h
│       │   ├── impl
│       │   │   ├── alignment_impl.h
│       │   │   ├── beam_beam_element_6d.h
│       │   │   ├── beam_elements_api.h
│       │   │   ├── beam_elements_type.h
│       │   │   ├── faddeeva.h
│       │   │   ├── faddeeva_mit.h
│       │   │   ├── particles_api.h
│       │   │   ├── particles_type.h
│       │   │   └── track_api.h
│       │   ├── mem_pool.h
│       │   ├── particles.h
│       │   └── track.h
│       ├── cuda
│       │   ├── cuda_env.h
│       │   └── impl
│       │       └── track_particles_kernel.cuh
│       ├── _impl
│       │   ├── definitions.h
│       │   ├── modules.h
│       │   ├── namespace_begin.h
│       │   ├── namespace_end.h
│       │   └── path.h
│       ├── opencl
│       │   └── ocl_environment.h
│       ├── sixtracklib.h
│       ├── testlib
│       │   ├── gpu_kernel_tools.h
│       │   ├── random.h
│       │   ├── testdata_files.h
│       │   ├── test_particles_tools.h
│       │   └── test_track_tools.h
│       └── testlib.h
└── lib
    ├── cmake
    │   ├── SixTracklibConfig.cmake
    │   ├── SixTracklibTargets.cmake
    │   ├── SixTracklibTargets-debug.cmake
    │   ├── SixTrackTestlibTargets.cmake
    │   └── SixTrackTestlibTargets-debug.cmake
    ├── libsixtrackd.so
    └── libsixtrack_testd.a
```
## Building the Study 10 shared library and example executable
Inside ${STUDY10_DIR}, enter the build directory and run cmake and make:
```
cd ${STUDY10_DIR}/build
cmake ..
make
```

