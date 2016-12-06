#include "measurement_storage.h"

/***********************************
* Returns the measure saved for    *
* the given id and the given type  *
* deletes the measure if wanted    *
***********************************/
int get_measure(int id, int type, int removeAfterRead) {
  int key = id * 100 + type;
  if(persist_exists(key)) {
    int value = persist_read_int(key);
    if(removeAfterRead) {
      persist_delete (key);
    }
    
    return value;
  }
  
  return -1;
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
