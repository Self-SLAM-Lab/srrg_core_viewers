#pragma once
#include <QKeyEvent>
#include <QGLViewer/qglviewer.h>



// Macro helpers for identifying the version number of QGLViewer
// QGLViewer changed some parts of its API in version 2.6.
// The following preprocessor hack accounts for this.
#if (((QGLVIEWER_VERSION & 0xff0000) >> 16) >= 2 && ((QGLVIEWER_VERSION & 0x00ff00) >> 8) >= 6)
#define qglv_real qreal
#else
#define qglv_real float
#endif



namespace srrg_core_viewers {

/** Simple viewer class, derived fro QGLViewer.
    Allows to display a simple OpenGL scene through the
    overriding of the draw() method.
    It offers some basic frae manupulation via the mouse.
    It also captures the keyboard events and allows to inspect the last key pressed.
    <br>
    The typical usage is in an unrolled app loop<br>
    QApplication app;<br>
    SimpleViewer v;<br>
    v.show();<br>
    ..<br>
    ..<br>
    while(1){<br>
    app.processEvents();<br>
      QKeyEvent* e = v.lastKeyEvent();<br>
      if (v.lastKeyEvent() == something){<br>
         do something;<br>
      }<br>
      v.keyEventProcessed();<br>
    }<br>
 */
class SimpleViewer: public QGLViewer {

  //! @class internal camera class
  class StandardCamera: public qglviewer::Camera {
  public:
    StandardCamera(): _standard(true) {}

    qglv_real zNear() const {
      if(_standard) { return qglv_real(_z_near); }
      else { return Camera::zNear(); }
    }

    qglv_real zFar() const {
      if(_standard) { return qglv_real(_z_far); }
      else { return Camera::zFar(); }
    }

    bool standard() const { return _standard; }
    void setStandard(bool s) { _standard = s; }
    void setZNear(const float& z_near_) {_z_near = z_near_;}
    void setZFar(const float& z_far_) {_z_far = z_far_;}

  protected:
    bool _standard;
    float _z_near = 0.001f;
    float _z_far  = 10000.0f;
  };

public:

  //! ctor
  SimpleViewer(QWidget* parent = 0);

  //! init method, opens the gl viewport and sets the key bindings
  void init();

  //! callback invoked by the application on new key event. It saves the last event in
  //! a member variable
  virtual void keyPressEvent(QKeyEvent *e);

  //! returns the last key pressed since invoking keyEventProcessed();
  QKeyEvent* lastKeyEvent();

  //! call this to clear the events, after processing them
  void keyEventProcessed();

  //! @brief modifiable camera
  StandardCamera* standardCamera() {return _standard_camera;}

protected:

  //! @brief customized 'standard' camera instance
  StandardCamera* _standard_camera;

  //! key handling
  QKeyEvent _last_key_event;
  bool _last_key_event_processed;

};
}
