

#include "luiSprite.h"
#include "luiRoot.h"

int LUISprite::_instance_count = 0;


LUISprite::LUISprite(LUIBaseElement* parent) : 
  LUIBaseElement(),
  _pool_slot(-1), 
  _vertex_pool(NULL)
{  

  _instance_count ++;

  if (lui_cat.is_spam()) {
    cout << "Constructed new LUISprite, (active: " << _instance_count << ")" << endl;  
  }

  set_color(1.0, 1.0, 1.0, 1.0);
  set_parent(parent);
  set_top_left(0, 0);
  set_z_index(0);
 
  set_uv_range(LVector2(0), LVector2(1));
  set_size(10, 10);
}

LUISprite::~LUISprite() {
  if (lui_cat.is_spam()) {
    _instance_count --;
    cout << "Destructing LUISprite, instances left: " << _instance_count << endl;
  }
}

void LUISprite::on_bounds_changed() {
  _data[0].x = _pos_x;
  _data[0].z = _pos_y;
  recompute_vertices();
  update_vertex_pool();
}

void LUISprite::on_visibility_changed() {

}


void LUISprite::on_detached() {
  if (_tex != NULL) {
    unassign_vertex_pool();
  }
  _root = NULL;
  _parent = NULL;
}


void LUISprite::ls(int indent) {
  cout << string(indent, ' ')  << "[LUISprite] pos = " 
      << _pos_x << ", " << _pos_y 
      << "; size = " << _size.get_x() << " x " << _size.get_y() 
      << "; tex = " << (_tex != NULL ? _tex->get_name() : "none") << endl;

} 


void LUISprite::set_root(LUIRoot* root) {

  if (lui_cat.is_spam()) {
    lui_cat.spam() << "LUISprite - root changed" << endl;
  }

  if (_root != NULL && _root != root) {
    lui_cat.warning() << "Unregistering from old LUIRoot" << endl;
    unassign_vertex_pool();
  }

  if (_root != root) {
    _root = root;

    if (_tex != NULL) {
      if (lui_cat.is_spam()) {
        cout << "Assigning vertex pool from set_root" << endl;
      }
      assign_vertex_pool();
    }

    if (lui_cat.is_spam()) {
      cout << "Root size is: " << _root->node()->get_size().get_x() << endl;
    }
  }
}


void LUISprite::assign_vertex_pool() {

  if (lui_cat.is_spam()) {
    lui_cat.spam() << "LUISprite:: Assign vertex pool" << endl;
  }

  // This should never happen, as all methods which call this method
  // should check if the root is already set. Otherwise something
  // went really wrong.
  nassertv(_root != NULL);

  LUIVertexPool* pool = _root->get_vpool_by_texture(_tex);
  
  if (lui_cat.is_spam()) {
    cout << "Got vertex pool handle: " << pool << endl;
  }

  // This might occur sometimes (hopefully not), and means that get_vpool_by_texture
  // could not allocate a vertex pool for some reason. VERY BAD.
  nassertv(pool != NULL);

  _vertex_pool = pool;
  _pool_slot = pool->allocate_slot();

  if (lui_cat.is_spam()) {
    cout << "Got vertex pool slot: " << _pool_slot << endl;
  }

  update_vertex_pool();

}

void LUISprite::update_vertex_pool() {
  if (_vertex_pool != NULL && _root != NULL) {
    
    if (lui_cat.is_spam()) {
      cout << "Updating vertex pool , pool slot is " << _pool_slot << endl;
    }

    void* write_pointer = _vertex_pool->get_sprite_pointer(_pool_slot);
    
    if (lui_cat.is_spam()) {
      cout << "Got vertex pool write pointer at " << write_pointer << endl;
    }

    //memcpy(write_pointer, &_data, sizeof(LUIVertexData) * 4);

  }
}

void LUISprite::unassign_vertex_pool() {

  if (lui_cat.is_spam()) {
    lui_cat.spam() << "LUISprite:: Unassign vertex pool" << endl;
  }

  if (_vertex_pool != NULL) {
    _vertex_pool->release_slot(_pool_slot);
    _vertex_pool = NULL;
    _pool_slot = 0;
  }
}
