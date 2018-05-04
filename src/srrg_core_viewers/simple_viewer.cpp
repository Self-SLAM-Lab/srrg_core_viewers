#include "simple_viewer.h"
#include <cstring>



namespace srrg_core_viewers {

SimpleViewer::SimpleViewer(QWidget* parent): QGLViewer(parent),
                                             _standard_camera(0),
                                             _last_key_event(QEvent::None, 0, Qt::NoModifier),
                                             _last_key_event_processed(true) {}

void SimpleViewer::init() {
  // Init QGLViewer.
  QGLViewer::init();
  // Set background color light yellow.
  // setBackgroundColor(QColor::fromRgb(255, 255, 194));


  // Set background color white.
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  //setBackgroundColor(QColor::fromRgb(255, 255, 255));

  // Set some default settings.
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glShadeModel(GL_FLAT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Don't save state.
  setStateFileName(QString::null);

  // Mouse bindings.
  setMouseBinding(Qt::NoModifier, Qt::RightButton, CAMERA, ZOOM);
  setMouseBinding(Qt::NoModifier, Qt::MidButton, CAMERA, TRANSLATE);
  setMouseBinding(Qt::ControlModifier, Qt::LeftButton, RAP_FROM_PIXEL);

  // Replace camera.
  qglviewer::Camera *oldcam = camera();
  _standard_camera = new StandardCamera();
  setCamera(_standard_camera);
  _standard_camera->setPosition(qglviewer::Vec(0.0f, 0.0f, 10.0f));
  _standard_camera->setUpVector(qglviewer::Vec(0.0f, 0.0f, 0.0f));
  _standard_camera->lookAt(qglviewer::Vec(0.0f, 0.0f, 0.0f));
  delete oldcam;

}

void SimpleViewer::keyPressEvent(QKeyEvent *e) {
  QGLViewer::keyPressEvent(e);
    _last_key_event = *e;
    _last_key_event_processed=false;
}

QKeyEvent* SimpleViewer::lastKeyEvent() {
  if (_last_key_event_processed)
    return 0;
  return &_last_key_event;
}

void SimpleViewer::keyEventProcessed() {
  _last_key_event_processed = true;
}
}
