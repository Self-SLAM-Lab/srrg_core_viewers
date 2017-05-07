#pragma once

#include <set>

#include <srrg_types/cloud.h>
#include "simple_viewer.h"

namespace srrg_core_viewers {

  class CloudViewer : public srrg_core_viewers::SimpleViewer {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    typedef std::map<const srrg_core::Cloud*, Eigen::Isometry3f, std::less<const srrg_core::Cloud*>,
		     Eigen::aligned_allocator<std::pair<const srrg_core::Cloud*, Eigen::Isometry3f> > > CloudIsometryMap;
    CloudViewer();
    enum Mode {MoveCamera=0x0, MoveObject=0x1};

    virtual void draw();
    virtual void drawWithNames();
    virtual void keyPressEvent(QKeyEvent *e);

    void addCloud(srrg_core::Cloud* c, const Eigen::Isometry3f& iso = Eigen::Isometry3f::Identity());
    void eraseCloud(srrg_core::Cloud* c);

  protected:
    CloudIsometryMap _clouds;
    Mode _mode;
    std::map<int, const srrg_core::Cloud*> _names_map;
    virtual void postSelection(const QPoint& point);
    std::set<const srrg_core::Cloud*> _selected_objects;
    bool _is_orthographic;
    
  };

}
