#include "notification_sender.h"

int UPLOAD_INTERVAL_IN_MINUTES = 60;

/***********************************
* Checks whether it is time to     *
* upload the saved data to the     *
* server.                          *
***********************************/
void check_for_upload_task()
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Checking for upload task...");
  time_t current_time = time(NULL);
  struct tm *local_time = localtime(&current_time);

  if ((local_time->tm_min + 1) % UPLOAD_INTERVAL_IN_MINUTES == 0)
  {
    APP_LOG(APP_LOG_LEVEL_INFO, "Background Worker initiates the upload task...");
    worker_launch_app(); // start the main app in order to upload the data
    psleep(1000);
    AppWorkerMessage message = {
        .data0 = 4711, // this is the code that says --> upload the latest data
    };
    app_worker_send_message(SOURCE_BACKGROUND, &message);
    APP_LOG(APP_LOG_LEVEL_INFO, "Background Worker sent task to foreground app...");
  }

  APP_LOG(APP_LOG_LEVEL_INFO, "Finished checking for upload task...");
}
