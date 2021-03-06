cmake_minimum_required (VERSION 3.9)

project (dggrid
  VERSION 7.05
  LANGUAGES C CXX
)

find_package(Doxygen)

#Adds aggressive warnings to all compilation
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -pedantic)
endif()



add_subdirectory(src/lib/dglib)
add_subdirectory(src/lib/proj4lib)
add_subdirectory(src/lib/shapelib)

add_subdirectory(src/apps/dggrid)
add_subdirectory(src/apps/appex)



if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.in @ONLY)
  add_custom_target(docs
    ${DOXYGEN_EXECUTABLE}
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.in
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
endif(DOXYGEN_FOUND)
