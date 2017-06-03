#pragma once

#include <set>

#include <srrg_types/cloud_3d.h>
#include "simple_viewer.h"

namespace srrg_core_viewers {

  class Cloud3DViewer : public srrg_core_viewers::SimpleViewer {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    typedef std::map<const srrg_core::Cloud3D*, Eigen::Isometry3f, std::less<const srrg_core::Cloud3D*>,
		     Eigen::aligned_allocator<std::pair<const srrg_core::Cloud3D*, Eigen::Isometry3f> > > CloudIsometryMap;
    Cloud3DViewer();
    enum Mode {MoveCamera=0x0, MoveObject=0x1};

    virtual void draw();
    virtual void drawWithNames();
    virtual void keyPressEvent(QKeyEvent *e);

    void addCloud(srrg_core::Cloud3D* c, const Eigen::Isometry3f& iso = Eigen::Isometry3f::Identity());
    void eraseCloud(srrg_core::Cloud3D* c);

  protected:
    CloudIsometryMap _clouds;
    Mode _mode;
    std::map<int, const srrg_core::Cloud3D*> _names_map;
    virtual void postSelection(const QPoint& point);
    std::set<const srrg_core::Cloud3D*> _selected_objects;
    bool _is_orthographic;
    
  };

}
