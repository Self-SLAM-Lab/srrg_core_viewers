#include <cstring>

#include <srrg_gl_helpers/opengl_primitives.h>
#include <srrg_gl_helpers/draw_attributes.h>

#include "cloud_viewer.h"

namespace srrg_core_viewers {
  
  using namespace std;
  using namespace Eigen;
  using namespace srrg_gl_helpers;
  using namespace srrg_core;
  
  Cloud3DViewer::Cloud3DViewer() {
    _mode = MoveCamera;
    _is_orthographic = false;
  }

  void Cloud3DViewer::addCloud(Cloud3D* c, const Eigen::Isometry3f& iso) {
    _clouds.insert(make_pair(c,iso));
  }

  void Cloud3DViewer::eraseCloud(Cloud3D* c) {
    _clouds.erase(c);
  }
  
  void Cloud3DViewer::draw(){
    for (CloudIsometryMap::iterator it = _clouds.begin(); it!=_clouds.end(); it++) {
      const Cloud3D* cloud = it->first;
      if (_selected_objects.count(cloud)) {
	glColor3f(0.8, 0.8, 0.2);
      } else {
	glColor3f(0.2, 0.4, 0.4);
      }
      glPushMatrix();
      glMultMatrix(it->second);
      cloud->draw();
      glPopMatrix();
    }
  }

  void Cloud3DViewer::drawWithNames() {
    _names_map.clear();
    int name = 0;
    for (CloudIsometryMap::iterator it = _clouds.begin(); it!=_clouds.end(); it++) {
      const Cloud3D* cloud = it->first;
      glPushMatrix();
      glMultMatrix(it->second);
      cloud->draw(name);
      glPopMatrix();
      _names_map.insert(make_pair(name, cloud));
      name++;
    }
  }

  void Cloud3DViewer::postSelection(const QPoint&){
    int id = selectedName();
    if (id < 0)
      return;
    const Cloud3D* cloud = _names_map[id];
    if (_selected_objects.count(cloud)){
      _selected_objects.erase(cloud);
    } else {
      _selected_objects.insert(cloud);
    }
  }


  void Cloud3DViewer::keyPressEvent(QKeyEvent *e) {
    switch(e->key()){
    case Qt::Key_Space: 
      _mode = MoveCamera;
      cerr << "MoveCamera Mode is OFF" << endl;
      return;
    case Qt::Key_M:
      cerr << "MoveObject Mode is ON" << endl;
      _mode = MoveObject;
      return;
    case Qt::Key_O:
      _is_orthographic =  !_is_orthographic;
      if(_is_orthographic)  
	camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);
      else
	camera()->setType(qglviewer::Camera::PERSPECTIVE);
      updateGL();
      return;
    default:;
    }
    if (_mode != MoveObject){
      SimpleViewer::keyPressEvent(e);
      return;
    }
    
    Eigen::Vector3f v(0,0,0);
    switch(e->key()){
    case Qt::Key_Up:
      v=Eigen::Vector3f(1,0,0);
      break;
    case Qt::Key_Down:
      v=Eigen::Vector3f(-1,0,0);
      break;
    case Qt::Key_Left:
      v=Eigen::Vector3f(0,1,0);
      break;
    case Qt::Key_Right:
      v=Eigen::Vector3f(0,-1,0);
      break;
    case Qt::Key_PageUp:
      v=Eigen::Vector3f(0,0,1);
      break;
    case Qt::Key_PageDown:
      v=Eigen::Vector3f(0,0,-1);
      break;
    default:
      SimpleViewer::keyPressEvent(e);
      return;
    }

    Eigen::Isometry3f transform;
    transform.setIdentity();
    float translational_step=0.1;
    float rotational_step = 0.1;
    if (e->modifiers()==Qt::ControlModifier){
      transform.linear() = Eigen::AngleAxisf(rotational_step, v).toRotationMatrix();
    } else {
      transform.translation() = v*translational_step;
    }

    for (std::set<const Cloud3D*>::iterator it=_selected_objects.begin(); 
	 it!=_selected_objects.end(); it++){
      Isometry3f& iso = _clouds[*it];
      iso = transform*iso;
    }
    updateGL();
  }

}
