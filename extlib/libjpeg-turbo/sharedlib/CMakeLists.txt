# Anything that must be linked against the shared C library on Windows must
# be built in this subdirectory, because CMake doesn't allow us to override
# the compiler flags for each build type except at directory scope.  Note
# to CMake developers:  Add a COMPILE_FLAGS_<CONFIG> target property, or
# better yet, provide a friendly way of configuring a Windows target to use the
# static C library.

# rom-properties: Replace ${CMAKE_SOURCE_DIR} with ${TOPSRCDIR},
# since libjpeg-turbo isn't the base project.
SET(TOPSRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

if(MSVC)
  # Build all configurations against shared C library
  foreach(var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(${var} MATCHES "/MT")
      string(REGEX REPLACE "/MT" "/MD" ${var} "${${var}}")
    endif()
  endforeach()
endif()

foreach(src ${JPEG_SOURCES})
  set(JPEG_SRCS ${JPEG_SRCS} ${TOPSRCDIR}/${src})
endforeach()

if(WITH_SIMD)
  # This tells CMake that the "source" files haven't been generated yet
  set_source_files_properties(${SIMD_OBJS} PROPERTIES GENERATED 1)
endif()

if(WITH_MEM_SRCDST AND NOT WITH_JPEG8)
  add_library(jpeg SHARED ${JPEG_SRCS} ${SIMD_OBJS}
    ${TOPSRCDIR}/win/jpeg${DLL_VERSION}-memsrcdst.def)
else()
  add_library(jpeg SHARED ${JPEG_SRCS} ${SIMD_OBJS}
    ${TOPSRCDIR}/win/jpeg${DLL_VERSION}.def)
endif()
set_target_properties(jpeg PROPERTIES SOVERSION ${DLL_VERSION}
  VERSION ${FULLVERSION})
# rom-properties: Set POSTFIX instead of using SUFFIX.
if(MSVC)
  #set_target_properties(jpeg PROPERTIES SUFFIX ${DLL_VERSION}.dll)
  SET_TARGET_PROPERTIES(jpeg PROPERTIES DEBUG_POSTFIX "${DLL_VERSION}${CMAKE_DEBUG_POSTFIX}")
  SET_TARGET_PROPERTIES(jpeg PROPERTIES RELEASE_POSTFIX "${DLL_VERSION}${CMAKE_RELEASE_POSTFIX}")
elseif(MINGW OR CYGWIN)
  #set_target_properties(jpeg PROPERTIES SUFFIX -${DLL_VERSION}.dll)
  SET_TARGET_PROPERTIES(jpeg PROPERTIES DEBUG_POSTFIX "-${DLL_VERSION}${CMAKE_DEBUG_POSTFIX}")
  SET_TARGET_PROPERTIES(jpeg PROPERTIES RELEASE_POSTFIX "-${DLL_VERSION}${CMAKE_RELEASE_POSTFIX}")
endif(MSVC)
if(WITH_SIMD)
  add_dependencies(jpeg simd)
endif()
# rom-properties: DO_SPLIT_DEBUG(), SET_WINDOWS_SUBSYSTEM()
DO_SPLIT_DEBUG(jpeg)
SET_WINDOWS_SUBSYSTEM(jpeg WIN32)

if(WITH_12BIT)
  set(COMPILE_FLAGS "-DGIF_SUPPORTED -DPPM_SUPPORTED -DUSE_SETMODE")
else()
  set(COMPILE_FLAGS "-DBMP_SUPPORTED -DGIF_SUPPORTED -DPPM_SUPPORTED -DTARGA_SUPPORTED -DUSE_SETMODE")
	set(CJPEG_BMP_SOURCES ../rdbmp.c ../rdtarga.c)
	set(DJPEG_BMP_SOURCES ../wrbmp.c ../wrtarga.c)
endif()

# rom-properties: Disable examples and tests.
IF(0)
add_executable(cjpeg ../cjpeg.c ../cdjpeg.c ../rdgif.c ../rdppm.c
  ../rdswitch.c ${CJPEG_BMP_SOURCES})
set_property(TARGET cjpeg PROPERTY COMPILE_FLAGS ${COMPILE_FLAGS})
target_link_libraries(cjpeg jpeg)

add_executable(djpeg ../djpeg.c ../cdjpeg.c ../rdcolmap.c ../rdswitch.c
  ../wrgif.c ../wrppm.c ${DJPEG_BMP_SOURCES})
set_property(TARGET djpeg PROPERTY COMPILE_FLAGS ${COMPILE_FLAGS})
target_link_libraries(djpeg jpeg)

add_executable(jpegtran ../jpegtran.c ../cdjpeg.c ../rdswitch.c ../transupp.c)
target_link_libraries(jpegtran jpeg)
set_property(TARGET jpegtran PROPERTY COMPILE_FLAGS "-DUSE_SETMODE")

add_executable(jcstest ../jcstest.c)
target_link_libraries(jcstest jpeg)
ENDIF(0) # rom-properties

install(TARGETS jpeg #cjpeg djpeg jpegtran
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)