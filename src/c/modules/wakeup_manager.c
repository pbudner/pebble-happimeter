#include "wakeup_manager.h"

/***********************************
* Delays the timer to let the user *
* answer later.                    *
***********************************/
void delay_timer()
{
  // Let the timestamp be 1 minutes from now
  const time_t future_timestamp = time(NULL) + (10 * SECONDS_PER_MINUTE);

  // Choose a 'cookie' value representing the reason for the wakeup
  const int cookie = 0;

  // If true, the user will be notified if they missed the wakeup
  // (i.e. their watch was off)
  const bool notify_if_missed = true;

  //cancel all wakeups
  wakeup_cancel_all();

  // Schedule wakeup event
  WakeupId id = wakeup_schedule(future_timestamp, cookie, notify_if_missed);

  // Check the scheduling was successful
  if (id >= 0)
  {
    // Persist the ID so that a future launch can query it
    const int wakeup_id_key = 43;
    persist_write_int(wakeup_id_key, id);
  }

  // This will be set by wakeup_query()
  time_t wakeup_timestamp = 0;

  // Is the wakeup still scheduled?
  if (wakeup_query(id, &wakeup_timestamp))
  {
    // Get the time remaining
    int seconds_remaining = wakeup_timestamp - time(NULL);
    APP_LOG(APP_LOG_LEVEL_INFO, "%d seconds until wakeup", seconds_remaining);
  }
}

/***********************************
* Wakeup event occurred while the  *
* was already open                 *
***********************************/
static void wakeup_handler(WakeupId id, int32_t reason)
{
  window_stack_push(introduction_window_get_window(), true);
}

/***********************************
* Almost Random Wakeup                           *
***********************************/
static void wakeup(){
  int randomhour_1;
  int randomhour_2;
  int randomhour_3;
  int randomhour_4;

  int randomminute_1 = rand() % 60;
  int randomminute_2 = rand() % 60;
  int randomminute_3 = rand() % 60;
  int randomminute_4 = rand() % 60;

  time_t t = time(0);   // get time now
  struct tm * tmp = localtime( & t );
  
  //First random time: time slot 9-11(:59) o'clock
  while (true){
    if(tmp->tm_hour < 12){
      randomhour_1 = 9;
      randomminute_1 = 0;
      break;
    }
    //simple way to change the range to be an integer between 0 and n-1 inclusive
    randomhour_1 = rand() % 12;
    if(randomhour_1 >= 9){
      break;
    }
  }

  //Second random time: time slot 12-14(:59) o'clock
   while (true){
     if(tmp->tm_hour < 15 && tmp->tm_hour >= 12){
      randomhour_2 = 12;
      randomminute_2 = 0;
      break;
    }
    //simple way to change the range to be an integer between 0 and n-1 inclusive
    randomhour_2 = rand() % 15;
    if(randomhour_2 >= 12){
      break;
    }
  }

  //Third random time: time slot 15-17(:59) o'clock
   while (true){
     if(tmp->tm_hour < 18 && tmp->tm_hour >= 15){
      randomhour_3 = 15;
      randomminute_3 = 0;
      break;
    }
    //simple way to change the range to be an integer between 0 and n-1 inclusive
    randomhour_3 = rand() % 18;
    if(randomhour_3 >= 15){
      break;
    }
  }

  //Fourth random time: time slot 18-20(:59) o'clock
   while (true){
     if(tmp->tm_hour < 21 && tmp->tm_hour >= 18){
      randomhour_4 = 18;
      randomminute_4 = 0;
      break;
    }
    //simple way to change the range to be an integer between 0 and n-1 inclusive
    randomhour_4 = rand() % 21;
    if(randomhour_4 >= 18){
      break;
    }
  }

  time_t timestamp_1 = clock_to_timestamp(TODAY, randomhour_1, randomminute_1);
  time_t timestamp_2 = clock_to_timestamp(TODAY, randomhour_2, randomminute_2);
  time_t timestamp_3 = clock_to_timestamp(TODAY, randomhour_3, randomminute_3);
  time_t timestamp_4 = clock_to_timestamp(TODAY, randomhour_4, randomminute_4);

  // Choose a 'cookie' value representing the reason for the wakeup
  //const int cookie = 0;

  // If true, the user will be notified if they missed the wakeup
  // (i.e. their watch was off)
  const bool notify_if_missed = true;

  //SUPERWICHTIG, // Cancel all wakeups
  wakeup_cancel_all();

  // Schedule wakeup event
  WakeupId id_1 = wakeup_schedule(timestamp_1 , 1 , notify_if_missed);
  WakeupId id_2 = wakeup_schedule(timestamp_2 , 2 , notify_if_missed);
  WakeupId id_3 = wakeup_schedule(timestamp_3 , 3 , notify_if_missed);
  WakeupId id_4 = wakeup_schedule(timestamp_4 , 4 , notify_if_missed);

  // Check the scheduling was successful
   if(id_1 >= 0 && id_2 >= 0 && id_3 >=0 && id_4 >= 0){
     // Persist the ID so that a future launch can query it
     const int wakeup_id_key = 43;
     persist_write_int(wakeup_id_key, id_1);
   }
  
    // Is the wakeup still scheduled?
  if(wakeup_query(id_1, &timestamp_1)) {
    // Get the time remaining
    int seconds_remaining = timestamp_1 - time(NULL);
    APP_LOG(APP_LOG_LEVEL_INFO, "%d seconds until wakeup", seconds_remaining);
  }
}

/***********************************
* Initiates the wakeup funtionality*
***********************************/
void init_wakeup()
{
  //If the app is launched due to a previously scheduled wakeup event
  if (launch_reason() == APP_LAUNCH_WAKEUP)
  {
    // The app was started by a wakeup event.
    WakeupId id = 0;
    int32_t reason = 0;

    // Get details and handle the event appropriately
    wakeup_get_launch_event(&id, &reason);
  }

  // Subscribe to wakeup service
  wakeup_service_subscribe(wakeup_handler);

  if(launch_reason() != APP_LAUNCH_WORKER){
    wakeup();
  }
}
