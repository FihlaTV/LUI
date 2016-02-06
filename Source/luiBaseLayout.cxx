

#include "luiBaseLayout.h"

#include "pstrtod.h"
#include <sstream>

TypeHandle LUIBaseLayout::_type_handle;
NotifyCategoryDef(luiBaseLayout, ":lui");

LUIBaseLayout::LUIBaseLayout(PyObject* self) : LUIObject(self)
{
}

void LUIBaseLayout::add(PT(LUIBaseElement) object, const string& cell_mode) {
  if (object->has_parent()) {
    luiBaseLayout_cat.error() << "Cannot call add() with an object which already has a parent!" << endl;
    return;
  }

  add_cell(object, construct_cell(cell_mode));
}

void LUIBaseLayout::add(PT(LUIBaseElement) object, float cell_height) {
  Cell cell = { CM_fixed, cell_height, NULL };
  add_cell(object, cell);
}

void LUIBaseLayout::add_cell(PT(LUIBaseElement) object, Cell cell) {
  // Construct cell container object
  LUIObject* container = new LUIObject(NULL);
  add_child(container);
  container->add_child(object);
  container->set_debug_name("LUILayoutCell");
  cell.node = container;
  init_container(container);
  _cells.push_back(cell);
}

bool check_int(const string& str) {
  for (auto it = str.begin(); it != str.end(); ++it) {
    if (*it < '0' || *it > '9')
      return false;
  }
  return true;
}

int parse_int(const string& str) {
  // Assumes valid int
  int result = 0;
  istringstream( str ) >> result;
  return result;
}

LUIBaseLayout::Cell LUIBaseLayout::construct_cell(const string& cell_mode) {
  Cell cell = { CM_fit, 0.0f, NULL };

  // Fit
  if (cell_mode == "?") {
    cell.mode = CM_fit;
    return cell;
  }

  // Fill
  if (cell_mode == "*") {
    cell.mode = CM_fill;
    return cell;
  }

  // Percentage
  if (cell_mode.back() == '%') {
    cell.mode = CM_percentage;

    // Parse payload
    string val = cell_mode.substr(0, cell_mode.size() - 1);
    char* endptr;
    double d_val = pstrtod(val.c_str(), &endptr);

    if (*endptr != 0) {
      luiBaseLayout_cat.error() << "Could not parse float: '" << val << "'" << endl;
      d_val = 0.0;
    }
    cell.payload = (float)(d_val / 100.0);
    return cell;
  }

  // Assume pixels otherwise
  if (check_int(cell_mode)) {
    cell.mode = CM_fixed;
    cell.payload = parse_int(cell_mode);
    return cell;
  }

  luiBaseLayout_cat.error() << "Invalid cell mode: '" << cell_mode << "'" << endl;
  return cell;
}


void LUIBaseLayout::update_layout() {
  // First, get available space
  float available = get_metric(this);
  size_t num_fill_cells = 0;
  for (auto it = _cells.begin(); it != _cells.end(); ++it) {
    available -= get_metric((*it).node);
    if ((*it).mode == CM_fill)
      ++num_fill_cells;
  }

  if (available < 0.0f) {
    luiBaseLayout_cat.warning() << "Not enough space available! (Missing " << -available << " pixels)" << endl;
  }

  // Divide available space by amount of containers that specify the '*' flag
  available /= max(1, num_fill_cells);
  available = ceil(available);

  float this_metric = get_metric(this);

  float offset = 0.0;

  // Iterate over all cells and distribute them
  for (auto it = _cells.begin(); it != _cells.end(); ++it) {
    const Cell& cell = *it;
    set_offset(cell.node, offset);
    switch (cell.mode) {

      // Fixed metric
      case CM_fixed: {
        set_metric(cell.node, cell.payload);
        offset += cell.payload;
        break;
      }

      // Filler cell
      case CM_fill: {
        set_metric(cell.node, available);
        offset += available;
        break;
      }

      // Percentage
      case CM_percentage: {
        float pct_metric = this_metric * cell.payload;
        set_metric(cell.node, pct_metric);
        offset += pct_metric;
        break;
      }

      // Fit
      case CM_fit: {
        float cell_metric = get_metric(cell.node);
        offset += cell_metric;
        break;
      }

      default: {
        // Unkown metric type
        nassertv(false);
      }
    };
  }
}

void LUIBaseLayout::update_dimensions_upstream() {
  for (auto it = _children.begin(); it!= _children.end(); ++it) {
    (*it)->update_dimensions_upstream();
  }

  update_dimensions();
  update_layout();
  fit_dimensions();
}
