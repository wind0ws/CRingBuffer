# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${CMAKE_SOURCE_DIR}/src/main/cpp/mlog.c
        ${CMAKE_SOURCE_DIR}/src/main/cpp/ringbuffer.c
        ${CMAKE_SOURCE_DIR}/src/main/cpp/ring_msg_queue.c
        ${CMAKE_SOURCE_DIR}/src/main/cpp/msg_queue_handler.c
        ${CMAKE_SOURCE_DIR}/src/main/cpp/native-lib.cpp)


add_executable(
        ring

        ${CMAKE_SOURCE_DIR}/src/main/cpp/mlog.c
        ${CMAKE_SOURCE_DIR}/src/main/cpp/ringbuffer.c
        ${CMAKE_SOURCE_DIR}/src/main/cpp/ring_msg_queue.c
        ${CMAKE_SOURCE_DIR}/src/main/cpp/msg_queue_handler.c
        ${CMAKE_SOURCE_DIR}/src/main/cpp/main.c
)

#add_library( # Sets the name of the library.
#        native-resample
#
#        # Sets the library as a shared library.
#        SHARED
#
#        # Provides a relative path to your source file(s).
#        src/main/cpp/native-resample.cpp)
#
#add_library( # Sets the name of the library.
#        native-hisi
#
#        # Sets the library as a shared library.
#        SHARED
#
#        # Provides a relative path to your source file(s).
#        src/main/cpp/native-hisi.cpp)

# Specifies a path to native header files.
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/)

#示例，include多个.h所在文件夹位置
#include_directories( src/main/cpp/include/jsoncpp
#        src/main/cpp/include/curl
#        src/main/cpp/include/openssl
#        )


#示例，导入.a静态库
#add_library(jsoncpp STATIC IMPORTED)
#set_target_properties(jsoncpp
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/src/main/cpp/libs/${ANDROID_ABI}/libjsoncpp.a)

#示例，导入.so动态库
#add_library(hi-common SHARED IMPORTED)
#set_target_properties(hi-common
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/src/main/cpp/libs/${ANDROID_ABI}/libhi_common.so)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})

target_link_libraries( # Specifies the target library.
        ring

        #        recorder
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
