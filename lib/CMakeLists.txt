## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

file(GLOB _lib_hdr "${CMAKE_CURRENT_SOURCE_DIR}/pujOgre/*.h*")
file(GLOB _lib_src "${CMAKE_CURRENT_SOURCE_DIR}/pujOgre/*.c*")

include_directories(
  ${OIS_INCLUDE_DIRS}
  ${OGRE_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)
link_libraries(
  ${OIS_LIBS_DIRS}
  ${OGRE_LIBS_DIRS}
)
add_library(pujOgre SHARED ${_lib_src})
target_link_libraries(pujOgre ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

## eof - $RCSfile$
