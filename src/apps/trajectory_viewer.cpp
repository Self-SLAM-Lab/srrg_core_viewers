#include <iostream>
#include <chrono>
#include <thread>
#include <qapplication.h>

#include "srrg_system_utils/system_utils.h"
#include "srrg_core_viewers/cloud_viewer.h"
#include "srrg_gl_helpers/opengl_primitives.h"



//ds satisfy Eigen since our viewers are aligned
typedef std::vector<Eigen::Isometry3d, Eigen::aligned_allocator<Eigen::Isometry3d> > IsometryVector;

//ds our own little viewer class
//ds defined here for simplicity, in practice put it in a separate .h/.cpp file!
class TrajectoryViewer: public srrg_core_viewers::SimpleViewer {

//ds object handling
public:

  //ds construct viewer
  TrajectoryViewer() {
    setWindowTitle("Trajectory Viewer");
    _poses.clear();
  }
  ~TrajectoryViewer() {
    //ds no dynamically allocated memory to clean
  }

//ds access
public:

  //ds update displayed data - in the worst case, like here, copying it if ownership is not defined
  void updatePoses(const IsometryVector new_poses_) {
    _poses = new_poses_;
    std::cerr << "updatePoses|updated poses: " << _poses.size() << std::endl;
  }

//ds overwrites
protected:

  //ds Qt standard draw function
  virtual void draw() override {
    glPointSize(1);
    glLineWidth(1);
    drawAxis(10);

    //ds draw camera box in red
    glColor3f(1, 0, 0);
    Eigen::Isometry3d previous_pose(Eigen::Isometry3d::Identity());
    for (const Eigen::Isometry3d& pose: _poses) {
      glPushMatrix();
      glMultMatrixf(pose.cast<float>().data());
      srrg_gl_helpers::drawPyramidWireframe(0.5, 0.5);
      glPopMatrix();
      previous_pose = pose;
    }

    //ds connect poses (highlight trajectory) in blue
    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    previous_pose.setIdentity();
    for (const Eigen::Isometry3d& pose: _poses) {
      glVertex3f(previous_pose.translation().x(), previous_pose.translation().y(), previous_pose.translation().z());
      glVertex3f(pose.translation().x(), pose.translation().y(), pose.translation().z());
      previous_pose = pose;
    }
    glEnd();
  }

  //ds Qt help string - appears when you press [H] on the viewer
  virtual QString helpString() const override {
    return "A little trajectory example viewer, please describe me better.";
  }

//ds displayed data
protected:

  //ds a vector of poses
  IsometryVector _poses;
};



int32_t main(int32_t argc_, char** argv_) {

  //ds check for invalid call and display help
  if (argc_ > 2) {
    std::cerr << "trajectory_viewer\n"
              "shows a sequence of randomly generated 3D (camera) poses \n"
              "usage: \n"
              " ./trajectory_viewer <number_of_generated_poses>" << std::endl;
    return 0;
  }

  //ds configuration
  uint32_t number_of_poses = 10;
  if (argc_ == 2) {
    number_of_poses = std::stoi(argv_[1]);
  }

  //ds launch Qt
  QApplication app(argc_, argv_);

  //ds create our viewer
  std::cerr << "Qt initialized, launching viewer" << std::endl;
  TrajectoryViewer viewer;
  viewer.show();

  //ds enable non-blocking rendering while viewer is active
  uint32_t counter = 0;
  while (viewer.isVisible()) {

    //ds process new input - here we generate our input to display
    //ds in this example we generate random input for every iteration every once in a while
    if (counter > 100) {
      IsometryVector generated_poses(number_of_poses);
      generated_poses[0].setIdentity();
      for (uint32_t u = 1; u < number_of_poses; ++u) {
        generated_poses[u].setIdentity();
        generated_poses[u].translation() = generated_poses[u-1].translation()+Eigen::Vector3d(rand()%10, rand()%10, rand()%10);
        Eigen::Vector3d rotation_axis(1+rand()%10, 1+rand()%10, 1+rand()%10);
        rotation_axis.normalize();
        generated_poses[u].rotate(Eigen::AngleAxis<double>(rand(), rotation_axis));
      }
      counter = 0;

      //ds update viewer data
      viewer.updatePoses(generated_poses);
    }

    //ds update visualization
    viewer.updateGL();
    app.processEvents();

    //ds breathe
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    ++counter;
  }
  std::cerr << "terminated viewer" << std::endl;
  return 0;
}
