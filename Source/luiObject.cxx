
#include "luiObject.h"

NotifyCategoryDef(luiObject, ":lui");

int LUIObject::_instance_count = 0;
TypeHandle LUIObject::_type_handle;

LUIObject::LUIObject(PyObject *self, float x, float y, float w, float h) : LUIBaseElement(self) {
  init();

   // Prevent recomputation of the position while we initialize the object
  begin_update_section();
  
  set_size(w, h);
  set_pos(x, y);

  end_update_section();

}

LUIObject::LUIObject(PyObject *self, LUIObject *parent, float x, float y, float w, float h)  : LUIBaseElement(self) {
  init();

  // Prevent recomputation of the position while we initialize the object
  begin_update_section();
  set_size(w, h);
  set_pos(x, y);

  parent->add_child(this);
  end_update_section();
}

LUIObject::~LUIObject() {

  _instance_count --;
  if (luiObject_cat.is_spam()) {
    luiObject_cat.spam() << "Destructing LUIObject, instances left: " << _instance_count << endl;
  }

  _children.clear();
}

void LUIObject::init() {
  _instance_count ++;
  if (luiObject_cat.is_spam()) {
    luiObject_cat.spam() << "Constructing new LUIObject (active: " << _instance_count << ")" << endl;
  }
}


void LUIObject::set_root(LUIRoot* root) {
  if (_root != NULL && root != _root) {
    luiObject_cat.error() << "Object is already attached to another root!" << endl;
    return;
  }

  if (root != _root) {

    // Unregister from old root
    unregister_events();
    _root = root;

    // Register to new root
    register_events();

    for (lui_element_iterator it = _children.begin(); it!= _children.end(); ++it) {
      (*it)->set_root(_root);
    }

  }
}

void LUIObject::ls(int indent) {
  cout << string(indent, ' ')  << "[LUIObject] pos = " << _pos_x << ", " << _pos_y << "; size = " << _size.get_x() << " x " << _size.get_y() << "; z-index = " << _z_index << " (+ "<< _local_z_index << ")" << endl;

  for (lui_element_iterator it = _children.begin(); it!= _children.end(); ++it) {
   (*it)->ls(indent + 1);
  }

} 



INLINE bool compare_z_index(LUIBaseElement* a, LUIBaseElement* b) {
  return a->get_z_offset() < b->get_z_offset();
}

void LUIObject::render_recursive() {

  if (!_visible) return;


  _last_frame_visible = _root->get_frame_index();
  
  recompute_position();
  std::sort(_children.begin(), _children.end(), compare_z_index);

  // Render all children, sorted by their relative z-index
  for (lui_element_iterator it = _children.begin(); it!= _children.end(); ++it) {
    (*it)->render_recursive();
  }
}