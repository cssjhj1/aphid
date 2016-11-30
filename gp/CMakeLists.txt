cmake_minimum_required (VERSION 2.8)
project (gpstudy)
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)
set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_BUILD_TYPE RELEASE)

include_directories ( ../shared ../kdtree ../lfr)

set (TKERN_HEADER_FILES ../shared/BaseImageWidget.h
../lfr/clapackTempl.h
../lfr/linearMath.h
Covariance.h
RbfKernel.h
kernelWidget.h
sampleKernelWidget.h
sampleKernelDialog.h
testKernelWindow.h)

add_executable (testkernel ${TKERN_HEADER_FILES}
../shared/BaseImageWidget.cpp
../shared/Plot1DWidget.cpp
../shared/Plot2DWidget.cpp
kernelWidget.cpp
sampleKernelWidget.cpp
sampleKernelDialog.cpp
testKernelWindow.cpp
testKernel.cpp)

if(WIN32)
    include_directories (D:/usr/CLAPACK/include)
    set(BLAS_LIBRARY D:/usr/CLAPACK/lib/blas.lib)
	set(LAPACK_LIBRARY D:/usr/CLAPACK/lib/lapack.lib)
    set(F2C_LIBRARY D:/usr/CLAPACK/lib/libf2c.lib)

elseif(APPLE)
	set(F2C_LIBRARY libf2c.a)
	set(BLAS_LIBRARY libblas.a)
	set(LAPACK_LIBRARY liblapack.a)
else ()
include_directories (/usr/local/include)
	set(F2C_LIBRARY /usr/local/lib/libf2c.so)
    set(BLAS_LIBRARY /usr/local/lib/libblas.so)
	set(LAPACK_LIBRARY /usr/local/lib/liblapack.so)
	
endif()

message ("blas " ${BLAS_LIBRARY})
message ("lapack " ${LAPACK_LIBRARY})
message ("f2c " ${F2C_LIBRARY})

target_link_libraries (testkernel ${BLAS_LIBRARY})
target_link_libraries (testkernel ${LAPACK_LIBRARY})
target_link_libraries (testkernel ${F2C_LIBRARY})

if (WIN32)
add_definitions (-DOPENEXR_DLL)
set (OpenEXR_INCLUDE_DIR D:/usr/openexr21/include)
set (OpenEXR_Library "D:/usr/openexr21/lib/Half.lib"
"D:/usr/openexr21/lib/Iex-2_1.lib"
"D:/usr/openexr21/lib/IlmImf-2_1.lib"
"D:/usr/openexr21/lib/IlmThread-2_1.lib")
elseif (APPLE)
set (OpenEXR_INCLUDE_DIR /Users/jianzhang/Library/openexr21/include)
set (OpenEXR_Library /Users/jianzhang/Library/openexr21/lib/libHalf.dylib
 /Users/jianzhang/Library/openexr21/lib/libIex-2_1.dylib
 /Users/jianzhang/Library/openexr21/lib/libIlmImf-2_1.dylib
 /Users/jianzhang/Library/openexr21/lib/libIlmThread-2_1.dylib)
else ()
SET (ILMBASE_PACKAGE_PREFIX /usr/local)

set (OpenEXR_INCLUDE_DIR ${ILMBASE_PACKAGE_PREFIX}/include)
set (OpenEXR_Library ${ILMBASE_PACKAGE_PREFIX}/lib/libHalf.so
    ${ILMBASE_PACKAGE_PREFIX}/lib/libIex-2_2.so
    ${ILMBASE_PACKAGE_PREFIX}/lib/libImath-2_2.so
    ${ILMBASE_PACKAGE_PREFIX}/lib/libIlmImf-2_2.so
    ${ILMBASE_PACKAGE_PREFIX}/lib/libIlmThread-2_2.so)
endif ()

message (" openexr inc is " ${OpenEXR_INCLUDE_DIR})
message (" openexr lib is " ${OpenEXR_Library})

include_directories (${OpenEXR_INCLUDE_DIR})
target_link_libraries (testkernel ${OpenEXR_Library})

if (WIN32)
    set (ZLIB_INCLUDE_DIR d:/usr/zlib-1.2.5)
    set (ZLIB_LIBRARY d:/usr/zlib-1.2.5/zdll.lib)
else ()
    set (ZLIB_INCLUDE_DIR /usr/include)
    set (ZLIB_LIBRARY /usr/lib64/libz.so)
endif ()

##find_package (zlib REQUIRED)

message (" zlib is " ${ZLIB_LIBRARY})
target_link_libraries (testkernel ${ZLIB_LIBRARIES})

if (WIN32)
set (HDF5_INCLUDE_DIR "D:/usr/hdf5/include")
set (HDF5_LIBRARIES "D:/usr/hdf5/lib/hdf5.lib")
elseif (APPLE)
set (HDF5_INCLUDE_DIR "/users/jianzhang/Library/hdf5/include")
set (HDF5_LIBRARIES "/users/jianzhang/Library/hdf5/lib/libhdf5.dylib")
else ()
    SET (HDF5_VERSION "1.8.17")
	SET (HDF5_INCLUDE_DIRS "~/Public/hdf5-1.8.17/include")
        SET (HDF5_LIBRARIES "~/Public/hdf5-1.8.17/lib/libhdf5.so")
##    FIND_PACKAGE (HDF5 REQUIRED)
endif ()

if (WIN32)
set (SZIP_INCLUDE_DIR "D:/usr/szip-2.1/src")
set (SZIP_LIBRARIES "D:/usr/szip-2.1/szip.lib")
include_directories (${SZIP_INCLUDE_DIR})
target_link_libraries (testkernel ${SZIP_LIBRARIES})
endif ()

include_directories (${HDF5_INCLUDE_DIR})
target_link_libraries (testkernel ${HDF5_LIBRARIES})

if(WIN32)
set (QT_QMAKE_EXECUTABLE "D:/usr/qt4.8.6/bin/qmake.exe")
endif()

find_package (Qt4 REQUIRED)
target_link_libraries (testkernel Qt4::QtGui)

if(WIN32)
	set (Boost_INCLUDE_DIR "D:/usr/boost_1_51_0")
	set (Boost_LIBRARY_DIR "D:/usr/boost_1_51_0/stage/lib")
	set (Boost_USE_STATIC_LIBS ON)
	set (Boost_USE_MULTITHREADED ON)
	include_directories ("D:/usr/boost_1_51_0")
	find_package(Boost 1.51 COMPONENTS system filesystem date_time regex thread chrono)
	
elseif (APPLE)
	set (Boost_INCLUDE_DIR "/users/jianzhang/Library/boost_1_55_0")
	set (Boost_LIBRARY_DIR "/users/jianzhang/Library/boost_1_55_0/stage/lib")
	include_directories ("/users/jianzhang/Library/boost_1_55_0")
	find_package(Boost 1.55 COMPONENTS system filesystem date_time regex thread)
	include_directories ("/usr/local/include")

else()
	SET (BOOST_ROOT "~/Public/boost_1_51_0")
	include_directories (${BOOST_ROOT})
	find_package(Boost 1.51 COMPONENTS system filesystem date_time regex thread)
	
endif()

message ("boost system " ${Boost_SYSTEM_LIBRARY})
message ("boost date_time " ${Boost_DATE_TIME_LIBRARY})
message ("boost regex " ${Boost_REGEX_LIBRARY})
message ("boost filesystem " ${Boost_FILESYSTEM_LIBRARY})
message ("boost thread " ${Boost_THREAD_LIBRARY})

set (AttributeNoninline "__attribute__ ((noinline))")
add_definitions (-DBOOST_NOINLINE=${AttributeNoninline})

target_link_libraries (testkernel ${Boost_SYSTEM_LIBRARY})
target_link_libraries (testkernel ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries (testkernel ${Boost_DATE_TIME_LIBRARY})
target_link_libraries (testkernel ${Boost_REGEX_LIBRARY})
target_link_libraries (testkernel ${Boost_THREAD_LIBRARY})
target_link_libraries (testkernel ${Boost_CHRONO_LIBRARY})

if (WIN32)
set (Aphid_Math_Library "D:/aphid/lib/aphidMath.lib")
set (Aphid_Img_Library "D:/aphid/lib/aphidImg.lib")
set (Aphid_Geom_Library "D:/aphid/lib/aphidGeom.lib")
set (Aphid_UI_Library "D:/aphid/lib/aphidUI.lib")
elseif (APPLE)
set (Aphid_Math_Library /users/jianzhang/aphid/lib/libaphidMath.a)
set (Aphid_Img_Library /users/jianzhang/aphid/lib/libaphidImg.a)
set (Aphid_Geom_Library /users/jianzhang/aphid/lib/libaphidGeom.a)
set (Aphid_UI_Library /users/jianzhang/aphid/lib/libaphidUI.a)
else()
    set (Aphid_Math_Library ~/aphid/lib/libaphidMath.so)
set (Aphid_Img_Library ~/aphid/lib/libaphidImg.so)
set (Aphid_Geom_Library ~/aphid/lib/libaphidGeom.so)
set (Aphid_UI_Library ~/aphid/lib/libaphidUI.so)
endif ()

target_link_libraries (testkernel ${Aphid_Math_Library})
target_link_libraries (testkernel ${Aphid_Geom_Library})

set (TSHAPE_HEADER_FILES ../shared/BaseImageWidget.h
../shared/Base3DView.h
../shared/GlslBase.h
../shared/GlslInstancer.h
../lfr/clapackTempl.h
../lfr/linearMath.h
Covariance.h
RbfKernel.h
kernelWidget.h
shapewidget.h
shapewindow.h)

set (TSHAPE_SOURCE_FILES ../shared/BaseImageWidget.cpp
../shared/Base3DView.cpp
../shared/GlslBase.cpp
../shared/GlslInstancer.cpp
../shared/Plot1DWidget.cpp
../shared/Plot2DWidget.cpp
kernelWidget.cpp
shapewidget.cpp
shapewindow.cpp
shapemain.cpp)

add_executable (testshape ${TSHAPE_HEADER_FILES} ${TSHAPE_SOURCE_FILES})

target_link_libraries (testshape ${BLAS_LIBRARY})
target_link_libraries (testshape ${LAPACK_LIBRARY})
target_link_libraries (testshape ${F2C_LIBRARY})

find_package (OpenGL REQUIRED)
message ("opengl is " ${OPENGL_LIBRARIES})
target_link_libraries (testshape ${OPENGL_LIBRARIES})

message ("qt opengl " ${QT_QTOPENGL_LIBRARY})
set (QT_USE_QTOPENGL ON)
target_link_libraries (testshape Qt4::QtGui Qt4::QtOpenGL)

target_link_libraries (testshape ${Boost_SYSTEM_LIBRARY})
target_link_libraries (testshape ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries (testshape ${Boost_DATE_TIME_LIBRARY})
target_link_libraries (testshape ${Boost_REGEX_LIBRARY})
target_link_libraries (testshape ${Boost_THREAD_LIBRARY})
target_link_libraries (testshape ${Boost_CHRONO_LIBRARY})

target_link_libraries (testshape ${Aphid_Math_Library})
target_link_libraries (testshape ${Aphid_Geom_Library})
target_link_libraries (testshape ${Aphid_UI_Library})

SET (EXECUTABLE_OUTPUT_PATH ../bin)
