add_library(srrg_core_viewers_library SHARED
  simple_viewer.cpp simple_viewer.h
)
target_link_libraries(srrg_core_viewers_library
  ${OPENGL_gl_LIBRARY} 
  ${OPENGL_glu_LIBRARY}
  ${QGLVIEWER_LIBRARY} 
  ${QT_QTXML_LIBRARY} 
  ${QT_QTOPENGL_LIBRARY} 
  ${QT_QTGUI_LIBRARY} 
  ${QT_QTCORE_LIBRARY} 
)
