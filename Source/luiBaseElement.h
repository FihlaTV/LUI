// Filename: luiBaseElement.h
// Created by:  tobspr (30Aug14)
//

#ifndef LUI_BASE_ELEMENT_H
#define LUI_BASE_ELEMENT_H

#include "pandabase.h"
#include "pandasymbols.h"
#include "luse.h"
#include "referenceCount.h"
#include "config_lui.h"

class EXPCL_PANDASKEL LUIBaseElement {

PUBLISHED:

  LUIBaseElement();
  ~LUIBaseElement();

  // Position
  INLINE void set_top_left(float top, float left);
  INLINE void set_top(float top);
  INLINE void set_bottom(float bottom);
  INLINE void set_left(float left);
  INLINE void set_right(float right);

  // Size
  INLINE void set_size(const LVector2 &size);
  INLINE void set_size(float w, float h);
  INLINE void set_width(float w);
  INLINE void set_height(float h);
  INLINE const LVector2 &get_size() const;

  // Visible
  INLINE void set_visible(bool visible);
  INLINE bool is_visible();
  INLINE void hide();
  INLINE void show();

public:

  INLINE void set_parent(LUIBaseElement* parent);
  INLINE LUIBaseElement* get_parent();

protected:

  INLINE void recompute_position();

  // Interface
  virtual void on_position_changed() = 0;
  virtual void on_size_changed() = 0;
  virtual void on_visibility_changed() = 0;

  PN_stdfloat _offset_x, _offset_y;
  bool _stick_top, _stick_left;
  PN_stdfloat _pos_x, _pos_y;
  LVector2    _size;
  bool	      _visible;

  LUIBaseElement *_parent;

};

#include "luiBaseElement.I"

#endif