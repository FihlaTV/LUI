

#include "luiSprite.h"
#include "luiRoot.h"

int LUISprite::_instance_count = 0;
TypeHandle LUISprite::_type_handle;


NotifyCategoryDef(luiSprite, ":lui");

LUISprite::LUISprite(LUIBaseElement* parent) : 
  LUIBaseElement(),
  _chunk_descriptor(NULL)
{  

  _instance_count ++;

  if (luiSprite_cat.is_spam()) {
    luiSprite_cat.spam() << "Constructed new LUISprite, (active: " << _instance_count << ")" << endl;  
  }

  // Prevent recomputation of the position while we initialize the sprite
  begin_update_section();

  set_color(1.0, 1.0, 1.0, 1.0);
  set_uv_range(LVector2(0), LVector2(1));
  set_size(10, 10);
  set_top_left(0, 0); 
  set_parent(parent);
  set_relative_z_index(0);

  // A sprite cannot be created directly, so we don't have to call end_update_section()
  // Instead, the LUIObject constructs the sprite, and calls end_update_section() after
  // it's done modifying it

}

LUISprite::~LUISprite() {
  if (luiSprite_cat.is_spam()) {
    _instance_count --;
    luiSprite_cat.spam() << "Destructing LUISprite, instances left: " << _instance_count << endl;
  }

  if (_chunk_descriptor != NULL) {
    luiSprite_cat.spam() << "Released chunk descriptor, as sprite did not get detached" << endl;
    _chunk_descriptor->release();
    delete _chunk_descriptor;
    _chunk_descriptor = NULL;
  }
}

void LUISprite::on_bounds_changed() {
  _data[0].x = _pos_x;
  _data[0].z = _pos_y;
  recompute_vertices();
  update_vertex_pool();
}

void LUISprite::on_visibility_changed() {
  luiSprite_cat.error() << "Todo: Implement hide() / show()" << endl;
}


void LUISprite::on_detached() {
  unregister_events();
  if (_tex != NULL) {
    unassign_vertex_pool();
  }
  _root = NULL;
  _parent = NULL;
}

void LUISprite::on_z_index_changed() {
  for (int i = 0; i < 4; i++) {
    _data[i].y = -_z_index;
  }
  update_vertex_pool();
}

void LUISprite::ls(int indent) {
  cout << string(indent, ' ')  << "[LUISprite] pos = " 
      << _pos_x << ", " << _pos_y 
      << "; size = " << _size.get_x() << " x " << _size.get_y() 
      << "; tex = " << (_tex != NULL ? _tex->get_name() : "none")
      << "; z_index = " << _z_index << " (+ " << _local_z_index << ")";


  if (_chunk_descriptor == NULL) {
    cout << "; no chunk ";
  } else {
    cout << "; chunk = " << _chunk_descriptor ->get_slot();
  }
  cout << endl;
} 


void LUISprite::set_root(LUIRoot* root) {

  if (luiSprite_cat.is_spam()) {
    luiSprite_cat.spam() << "Root changed .." << endl;
  }

  if (_root != NULL && _root != root) {
    luiSprite_cat.warning() << "Unregistering from old LUIRoot .. you should detach the sprite from the old root first .." << endl;
    unassign_vertex_pool();
  }

  if (_root != root) {

    // Unregister from old root
    unregister_events();
    _root = root;

    // Register to new root
    register_events();

    if (_tex != NULL) {
      assign_vertex_pool();
    }
  }  
}

void LUISprite::assign_vertex_pool() {

  // This should never happen, as all methods which call this method
  // should check if the root is already set. Otherwise something
  // went really wrong.
  nassertv(_root != NULL);

  LUIVertexPool* pool = _root->get_vpool_by_texture(_tex);
  
  if (luiSprite_cat.is_spam()) {
    luiSprite_cat.spam() << "Got vertex pool location: " << pool << endl;
  }

  // This might occur sometimes (hopefully not), and means that get_vpool_by_texture
  // could not allocate a vertex pool for some reason. 
  nassertv(pool != NULL);

  // Delete old descriptor first
  if (_chunk_descriptor != NULL) {
    _chunk_descriptor->release();
    delete _chunk_descriptor;
    _chunk_descriptor = NULL;
  }

  _chunk_descriptor = pool->allocate_slot(this);
  if (luiSprite_cat.is_spam()) {
    luiSprite_cat.spam() << "Got chunk " << _chunk_descriptor->get_chunk() << ", slot = " << _chunk_descriptor->get_slot() << endl;
  }

  update_vertex_pool();

}

void LUISprite::update_vertex_pool() {
  if (_chunk_descriptor != NULL && _root != NULL && !_in_update_section) {
    
    // This should never happen, but it's good to check
    nassertv(_chunk_descriptor->get_chunk() != NULL);

    if (luiSprite_cat.is_spam()) {
      luiSprite_cat.spam() << "Updating vertex pool slot " << _chunk_descriptor->get_slot() << " in pool " << _chunk_descriptor->get_chunk() << endl;
    }

    void* write_pointer = _chunk_descriptor->get_write_ptr();

    // This also should never happen
    nassertv(write_pointer != NULL);

    memcpy(write_pointer, &_data, sizeof(LUIVertexData) * 4);
  }
}

void LUISprite::unassign_vertex_pool() {
  if (luiSprite_cat.is_spam()) {
    luiSprite_cat.spam() << "Unassign vertex pool" << endl;
  }

  if (_chunk_descriptor != NULL) {
    _chunk_descriptor->release();
    delete _chunk_descriptor;
    _chunk_descriptor = NULL;
  }
}
