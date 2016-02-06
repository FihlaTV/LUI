// Filename: luiBaseLayout.h
// Created by:  tobspr (06Feb16)
//

#ifndef LUI_BASE_LAYOUT_H
#define LUI_BASE_LAYOUT_H

#include "config_lui.h"
#include "luiBaseElement.h"
#include "luiObject.h"
#include "pandabase.h"

NotifyCategoryDecl(luiBaseLayout, EXPCL_LUI, EXPTP_LUI);

class EXPCL_LUI LUIBaseLayout : public LUIObject {
public:

  enum CellMode {
    CM_fit,             // Key: '?'
    CM_percentage,      // Key: "xxx%", e.g. "23%"
    CM_fill,            // Key: '*'
    CM_fixed,           // Key: '10' (absolute value)
  };

  LUIBaseLayout(PyObject* self);

  PUBLISHED:
  void add(PT(LUIBaseElement) object, const string& cell_mode = "?");
  void add(PT(LUIBaseElement) object, float cell_height);

public:

  void update_dimensions_upstream();

protected:
  struct Cell {
    // Cell mode, determines the cell size
    CellMode mode;

    // Stores stuff like percentage, pixels, content depends on mode
    double payload;

    // Container node
    PT(LUIBaseElement) node;
  };

  void add_cell(PT(LUIBaseElement) object, Cell cell);

  // Interface
  virtual void init_container(LUIObject* container) = 0;

  // Interface to set the metrics, for a horizontal layout the metric
  // is the element width, for a vertical layout it is the height.
  virtual float get_metric(LUIBaseElement* element) = 0;
  virtual void set_metric(LUIBaseElement* element, float metric) = 0;
  virtual void set_offset(LUIBaseElement* element, float offset) = 0;

  void update_layout();

  Cell construct_cell(const string& cell_mode);
  pvector<Cell> _cells;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    LUIObject::init_type();
    register_type(_type_handle, "LUIBaseLayout", LUIObject::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif // LUI_BASE_LAYOUT_H
