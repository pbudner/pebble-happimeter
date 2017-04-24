#include "measurement.h"

/***********************************
* Used for calc. online variance   *
***********************************/
static int acc_n = 0;
static int acc_mean_x = 0;
static int acc_m2_x = 0;
static int acc_mean_y = 0;
static int acc_m2_y = 0;
static int acc_mean_z = 0;
static int acc_m2_z = 0;

/***********************************
* Since accelerometer data is new  *
* store the variables for next     *
* measurement iteration.           *
***********************************/
static int last_acc_mean_x = 0;
static int last_acc_mean_y = 0;
static int last_acc_mean_z = 0;
static int last_acc_var_x = 0;
static int last_acc_var_y = 0;
static int last_acc_var_z = 0;
static int last_activity = 0;

/***********************************
* Calculates all measurement data  *
* and returns them in a struct     *
***********************************/
HealthMeasure perform_measurement() {
  time_t end = time(NULL);
  time_t start = end - (UPDATE_INTERVAL_IN_MINUTES * SECONDS_PER_MINUTE);
  struct tm * utc_time = gmtime(&end);
  int time = mktime(utc_time);

  // last 15 minutes may not be available
  start -= (15 * SECONDS_PER_MINUTE);
  end -= (15 * SECONDS_PER_MINUTE);

  HealthMeasure measure;
  measure.Time = time; // save the inspected time

  // set activity based on the last measurement iteration
  measure.CurrentActivity = last_activity;

  // get the current activity
  HealthActivityMask currentActivities = health_service_peek_current_activities();
  if(currentActivities & HealthActivityNone) {
    last_activity = 0; // none
  }
  else if ((currentActivities & HealthActivitySleep) || (currentActivities & HealthActivityRestfulSleep)) {
    last_activity = 1; // sleep
  }
  else if ((currentActivities & HealthActivityWalk) && !(currentActivities & HealthActivityRun)) {
    last_activity = 2; // walk
  }
  else if (currentActivities & HealthActivityRun) {
    last_activity = 3; // run
  } else {
    last_activity = 4; // generic
  }

  // use last calculated average and variance
  measure.AverageAccX = last_acc_mean_x;
  measure.AverageAccY = last_acc_mean_y;
  measure.AverageAccZ = last_acc_mean_z;
  measure.VarianceAccX = last_acc_var_x;
  measure.VarianceAccY = last_acc_var_y;
  measure.VarianceAccZ = last_acc_var_z;

  // set new variance and mean for current measurement duration
  last_acc_mean_x = acc_mean_x;
  last_acc_mean_y = acc_mean_y;
  last_acc_mean_z = acc_mean_z;
  last_acc_var_x = acc_m2_x / (acc_n - 1);
  last_acc_var_y = acc_m2_y / (acc_n - 1);
  last_acc_var_z = acc_m2_z / (acc_n - 1);

  // check data is available
  HealthServiceAccessibilityMask result = health_service_metric_accessible(HealthMetricStepCount, start, end);
  if(result == HealthServiceAccessibilityMaskAvailable) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Health Service is available..");
    const uint32_t max_records = UPDATE_INTERVAL_IN_MINUTES;
    HealthMinuteData *minute_data = (HealthMinuteData*) malloc(max_records * sizeof(HealthMinuteData));
    uint32_t num_records = health_service_get_minute_history(minute_data, max_records, &start, &end);
    APP_LOG(APP_LOG_LEVEL_INFO, "Num records: %d", (int)num_records);

    int number_of_valid_bpm = 0;
    measure.Steps = 0;
    measure.AverageLightLevel = 0;
    measure.AverageHeartRate = 0;
    measure.VectorMagnitudeCounts = 0;
    for (uint32_t i = 0; i < num_records; i++)
    {
      measure.Steps += (int)minute_data[i].steps;
      measure.AverageLightLevel += (int)minute_data[i].light;
      measure.VectorMagnitudeCounts += (int)minute_data[i].vmc;
      int heart_rate = (int)minute_data[i].heart_rate_bpm;
      if(heart_rate > HEART_RATE_THRESHOLD) {
        number_of_valid_bpm++;
        measure.AverageHeartRate += heart_rate;
      }
    }

    measure.AverageLightLevel = measure.AverageLightLevel / num_records;
    measure.AverageHeartRate =  measure.AverageHeartRate / number_of_valid_bpm;

    // free the array
    free(minute_data);

    // reset calculation variables used for online variance
    acc_n = 0;
    acc_mean_x = 0;
    acc_m2_x = 0;
    acc_mean_y = 0;
    acc_m2_y = 0;
    acc_mean_z = 0;
    acc_m2_z = 0;
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Health Service is not available..");
  }

  return measure;
}

/***********************************
* Performs a measurement iteration *
* and stores the data in storage   *
***********************************/
void measurement_iteration() {
  time_t current_time = time(NULL);
  struct tm *local_time = localtime(&current_time);
  
  if((local_time->tm_min + 1) % UPDATE_INTERVAL_IN_MINUTES == 0) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Measurement iteration started.");

    // get the measures for the current iteration
    HealthMeasure measure = perform_measurement();

    // output and save the measurements
    int data_set_id = get_last_measure_id() + 1;
    APP_LOG(APP_LOG_LEVEL_INFO, "Data Set ID: %d", data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Time: %d", measure.Time);
    APP_LOG(APP_LOG_LEVEL_INFO, "Saved Time: %d", save_measure(10, measure.Time, data_set_id));
    APP_LOG(APP_LOG_LEVEL_INFO, "Steps: %d", measure.Steps);
    save_measure(11, measure.Steps, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Activity: %d", measure.CurrentActivity);
    save_measure(12, measure.CurrentActivity, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Avg. Heart Rate: %d", measure.AverageHeartRate);
    save_measure(13, measure.AverageHeartRate, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Acc X: %d", measure.AverageAccX);
    save_measure(14, measure.AverageAccX, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Var X: %d", measure.VarianceAccX);
    save_measure(15, measure.VarianceAccX, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Acc Y: %d", measure.AverageAccY);
    save_measure(16, measure.AverageAccY, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Var Y: %d", measure.VarianceAccY);
    save_measure(17, measure.VarianceAccY, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Acc Z: %d", measure.AverageAccZ);
    save_measure(18, measure.AverageAccZ, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Var Z: %d", measure.VarianceAccZ);
    save_measure(19, measure.VarianceAccZ, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "VMC: %d", measure.VectorMagnitudeCounts);
    save_measure(20, measure.VectorMagnitudeCounts, data_set_id);
    APP_LOG(APP_LOG_LEVEL_INFO, "Avg. Light: %d", measure.AverageLightLevel);
    save_measure(21, measure.AverageLightLevel, data_set_id);

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Measurement iteration finished.");
  }
}

/***********************************
* Calculate average and variance   *
* using Welford's online algorithm *
***********************************/
void accel_raw_data_handler(AccelRawData *data, uint32_t num_samples, uint64_t timestamp) {
  for (uint32_t i = 0; i < num_samples; i++) {
    AccelRawData raw_data = data[i];
    acc_n++;
    int delta, delta2;
    
    // online variance for x
    delta = (int)raw_data.x - acc_mean_x;
    acc_mean_x += delta / acc_n;
    delta2 = (int)raw_data.x - acc_mean_x;
    acc_m2_x += delta * delta2;

    // online variance for y
    delta = (int)raw_data.y - acc_mean_y;
    acc_mean_y += delta / acc_n;
    delta2 = (int)raw_data.y - acc_mean_y;
    acc_m2_y += delta * delta2;

    // online variance for z
    delta = (int)raw_data.z - acc_mean_z;
    acc_mean_z += delta / acc_n;
    delta2 = (int)raw_data.z - acc_mean_z;
    acc_m2_z += delta * delta2;
  }
}

/***********************************
* Inits the measurement and sub-   *
* scribes to accelerometer         *
***********************************/
void init_measurement() {
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  accel_raw_data_service_subscribe(10, accel_raw_data_handler);
}

/***********************************
* Deinits the measurement and      *
* unsubscribes from accelerometer  *
***********************************/
void deinit_measurement() {
  accel_data_service_unsubscribe();
}
