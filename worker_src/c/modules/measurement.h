#pragma once

#include <pebble_worker.h>
#include "storage.h"

#define UPDATE_INTERVAL_IN_MINUTES 15
#define HEART_RATE_THRESHOLD 40

typedef struct {
    int Time;
    int CurrentActivity;
    int AverageLightLevel;
    int AverageHeartRate;
    int VectorMagnitudeCounts;
    int AverageAccX;
    int VarianceAccX;
    int AverageAccY;
    int VarianceAccY;
    int AverageAccZ;
    int VarianceAccZ;
} HealthMeasure;

extern HealthMeasure perform_measurement();
extern void measurement_iteration();

extern void accel_raw_data_handler(AccelRawData *data, uint32_t num_samples, uint64_t timestamp);
extern void init_measurement();
extern void deinit_measurement();
