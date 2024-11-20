// For SPI mode, we need a CS pin
#define BNO08X_CS 10
#define BNO08X_INT 9

// #define FAST_MODE

// For SPI mode, we also need a RESET
// #define BNO08X_RESET 5
// but not for I2C or UART
#define BNO08X_RESET -1

struct euler_t {
    float yaw;
    float pitch;
    float roll;
} ypr;

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

#ifdef FAST_MODE
// Top frequency is reported to be 1000Hz (but freq is somewhat variable)
sh2_SensorId_t reportType = SH2_GYRO_INTEGRATED_RV;
long reportIntervalUs = 2000;
#else
// Top frequency is about 250Hz but this report is more accurate
sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
long reportIntervalUs = 5000;
#endif
void setReports(sh2_SensorId_t reportType, long report_interval) {
    Serial.println("Setting desired reports");
    if (!bno08x.enableReport(reportType, report_interval)) {
        Serial.println("Could not enable stabilized remote vector");
    }
}

void IMUInit() {
    Serial.println("Adafruit BNO08x test!");

    // Try to initialize!
    if (!bno08x.begin_I2C()) {
        // if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300
        // byte UART buffer! if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
        Serial.println("Failed to find BNO08x chip");
        setColor(RED);
        while (1) {
            delay(10);
        }
    }
    Serial.println("BNO08x Found!");

    setReports(reportType, reportIntervalUs);

    Serial.println("Reading events");
    delay(100);
}

void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t *ypr,
                       bool degrees = false) {
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
        ypr->yaw *= RAD_TO_DEG;
        ypr->pitch *= RAD_TO_DEG;
        ypr->roll *= RAD_TO_DEG;
    }
}

void quaternionToEulerRV(sh2_RotationVectorWAcc_t *rotational_vector,
                         euler_t *ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i,
                      rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void quaternionToEulerGI(sh2_GyroIntegratedRV_t *rotational_vector,
                         euler_t *ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i,
                      rotational_vector->j, rotational_vector->k, ypr, degrees);
}

float getAngle(int16_t offset = 0) {
    if (bno08x.wasReset()) {
        Serial.print("sensor was reset ");
        setReports(reportType, reportIntervalUs);
    }

    if (bno08x.getSensorEvent(&sensorValue)) {
        // in this demo only one report type will be received depending on
        // FAST_MODE define (above)
        switch (sensorValue.sensorId) {
            case SH2_ARVR_STABILIZED_RV:
                quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr,
                                    true);
            case SH2_GYRO_INTEGRATED_RV:
                // faster (more noise?)
                quaternionToEulerGI(&sensorValue.un.gyroIntegratedRV, &ypr,
                                    true);
                break;
        }
    }
    float angle = ypr.yaw + 180 - offset;
    while (angle >= 360) angle -= 360;
    while (angle < 0) angle += 360;
    return (angle);
}
