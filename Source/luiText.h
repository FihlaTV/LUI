// Filename: luiText.h
// Created by:  tobspr (14Sep14)
//

#ifndef LUI_TEXT_H
#define LUI_TEXT_H

#include "pandabase.h"
#include "pandasymbols.h"
#include "luse.h"
#include "referenceCount.h"
#include "config_lui.h"
#include "luiObject.h"
#include "luiIterators.h"
#include "luiFontPool.h"
#include "luiSprite.h"
#include "dynamicTextFont.h"
#include "dynamicTextGlyph.h"
#include "textFont.h"
#include "texture.h"
#include "dcast.h"

class EXPCL_LUI LUIText : public LUIObject {

PUBLISHED:

  LUIText(float x = 0.0, float y = 0.0);
  LUIText(LUIObject *parent, float x = 0.0, float y = 0.0);
  LUIText(LUIObject *parent, const string &text, const string &font_name = "default", float font_size = 16.0, float x = 0.0, float y = 0.0);

  ~LUIText();

  INLINE void set_font(const string &font_name);
  INLINE void set_text(const string &text);
  INLINE void set_font_size(float size);
  
  virtual void ls(int indent = 0);
  
protected:

  void update_text();

  DynamicTextFont *_font;
  string _text;
  float _font_size;
  pvector<PT(DynamicTextGlyph)> _glyphs;


public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    LUIObject::init_type();
    register_type(_type_handle, "LUIText", LUIObject::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;


};

#include "luiText.I"

#endif