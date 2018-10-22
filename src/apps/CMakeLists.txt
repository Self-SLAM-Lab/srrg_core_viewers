add_executable(srrg_cloud_viewer_gui_app srrg_cloud_viewer_gui_app.cpp)
target_link_libraries(srrg_cloud_viewer_gui_app
  srrg_core_viewers_library
  ${OpenCV_LIBS}
)

add_executable(trajectory_viewer trajectory_viewer.cpp)
target_link_libraries(trajectory_viewer
  srrg_core_viewers_library
  ${OpenCV_LIBS}
  -lglut -lGL
)
