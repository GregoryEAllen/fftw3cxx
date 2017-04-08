cmake_minimum_required(VERSION 2.8.11)
project(fftw3cxx)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(FFTW3 COMPONENTS fftw3 fftw3f fftw3l)

include(GNUInstallDirs)
install(FILES include/fftw3cxx.hh
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT devel
)

enable_testing()
add_subdirectory(test)
