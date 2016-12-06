#include <pebble_worker.h>
#include "storage.h"

/***********************************
* Saves a measure for the given id *
* and type.                        *
***********************************/
enum StatusCode save_measure(int type, int value, int id) {
  // INFO: each save is 4 bytes in the storage..
  // INFO: each app can save up to 4k in the storage..
  uint32_t data_key = id * 100 + type; // unique id for type
  enum StatusCode code = persist_write_int(data_key, value);
  return code;
}

/***********************************
* Returns the last inserted id     *
* 0, if no saved measure exist     *
***********************************/
int get_last_measure_id() {
  bool found = true;
  int id = 1;
  while(found) {
    found = persist_exists(id*100 + 10);
    if(found) {
      id++;
    }
  }
  
  return id - 1;
}
