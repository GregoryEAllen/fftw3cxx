# Find FFTW3 includes and library, with components
#
# The following variables will be set upon return
#  FFTW3_FOUND       - True if all specified components of FFTW3 were found
#  FFTW3_INCLUDE_DIR - Path to fftw3.h
#  FFTW3_LIBRARIES   - List of libraries for specified components of FFTW3
#
# Components of FFTW3 can be specified, e.g.
#  find_package(FFTW3 COMPONENTS fftw3f fftw3f_threads)
#
# More detail can be gleaned about the specified components of FFTW3,
# which may include:
#  fftw3,  fftw3_threads  - double precision library (without and with threads)
#  fftw3f, fftw3f_threads - single precision library (without and with threads)
#  fftw3l, fftw3l_threads - long double precision library (without and with threads)
# Upon return, two variables for each specified component are set (all uppercase):
#  FFTW3_${COMPONENT}_FOUND    - True if the named component was found
#  FFTW3_${COMPONENT}_LIBRARY  - The library that supports the named component

set( _DEFAULT_COMPONENTS fftw3 fftw3_threads fftw3f fftw3f_threads fftw3l fftw3l_threads )

if( FFTW3_FIND_COMPONENTS )
	set( _COMPONENTS ${FFTW3_FIND_COMPONENTS} )
else()
	set( _COMPONENTS ${_DEFAULT_COMPONENTS} )
endif()

include( FindPackageHandleStandardArgs )

find_path( FFTW3_INCLUDE_DIR fftw3.h )

foreach( _lib ${_COMPONENTS} )
	string( TOUPPER FFTW3_${_lib} _COMP )
	find_library( ${_COMP}_LIBRARY ${_lib} )
	set(FFTW3_LIBRARIES ${FFTW3_LIBRARIES} ${${_COMP}_LIBRARY})
	if( ${_COMP}_LIBRARY )
		set( ${_COMP}_FOUND TRUE )
	else()
		set( ${_COMP}_FOUND FALSE )
		message( "-- Could NOT find component of FFTW3: ${_lib}" )
	endif()
	mark_as_advanced( ${_COMP}_LIBRARY ${_COMP}_FOUND )
	
	if ( NOT TARGET ${_lib} )
		add_library( ${_lib} IMPORTED STATIC GLOBAL )
		set_property( TARGET ${_lib}
			PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${FFTW3_INCLUDE_DIR}
		)
		set_property( TARGET ${_lib}
			PROPERTY IMPORTED_LOCATION ${${_COMP}_LIBRARY}
		)
	endif ()
endforeach()

find_package_handle_standard_args( FFTW3 DEFAULT_MSG FFTW3_LIBRARIES FFTW3_INCLUDE_DIR )
mark_as_advanced( FFTW3_INCLUDE_DIR FFTW3_LIBRARIES )
