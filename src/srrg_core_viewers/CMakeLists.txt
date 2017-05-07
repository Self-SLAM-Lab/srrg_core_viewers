add_library(srrg_core_viewers_library SHARED 
  simple_viewer.cpp simple_viewer.h
  cloud_viewer.cpp cloud_viewer.h
)

target_link_libraries(srrg_core_viewers_library
  ${SRRG_QT_LIBRARIES}
  ${QGLVIEWER_LIBRARY}
  ${catkin_LIBRARIES}
)
