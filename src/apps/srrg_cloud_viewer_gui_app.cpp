#include <iostream>
#include <qapplication.h>

#include "srrg_system_utils/system_utils.h"
#include "srrg_core_viewers/cloud_viewer.h"



const char* banner[] = {
  "srrg_cloud_viewer_gui_app",
  "shows a set of clouds, shift + left click to select clouds in the viewer",
  "usage:",
  " srrg_cloud_viewer_gui_app <cloud1.dat> <cloud2.dat> ...",
  0
};

int main(int argc, char** argv) {
  std::list<srrg_core::Cloud3D*> clouds;
  if(argc < 2 || !strcmp(argv[1], "-h")) { 
    srrg_core::printBanner(banner);
    return 0;
  }

  int c = 1;
  while(c < argc) {
    srrg_core::Cloud3D* cloud = new srrg_core::Cloud3D;
    std::ifstream is(argv[c]);
    cloud->read(is);
    clouds.push_back(cloud);
    std::cerr << "loaded cloud [" << argv[c] << "] with " << cloud->size() << "] points" << std::endl;
    c++;
  }
  
  QApplication app(argc, argv);
  srrg_core_viewers::Cloud3DViewer viewer;
  for(std::list<srrg_core::Cloud3D*>::iterator it = clouds.begin(); it != clouds.end(); ++it) {
    viewer.addCloud(*it);
  }  
  viewer.show();
  app.exec();

  for(std::list<srrg_core::Cloud3D*>::iterator it = clouds.begin(); it != clouds.end(); ++it) {
    delete *it;
  }  
  
  return 0;
}
