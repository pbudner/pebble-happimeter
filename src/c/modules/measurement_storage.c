#include "measurement_storage.h"

/***********************************
* Removes the measure for          *
* the given id and the given type  *
***********************************/
void remove_measure(int id, int type) {
  int key = id * 100 + type;
  persist_delete (key);
}


/***********************************
* Removes all measures for         *
* the given id                     *
***********************************/
void remove_all_measures(int id) {
  remove_measure(id, 10);
  remove_measure(id, 11);
  remove_measure(id, 12);
  remove_measure(id, 13);
  remove_measure(id, 14);
  remove_measure(id, 15);
  remove_measure(id, 16);
  remove_measure(id, 17);
  remove_measure(id, 18);
  remove_measure(id, 19);
  remove_measure(id, 20);
  remove_measure(id, 21);
}

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
