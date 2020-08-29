#include <stdio.h>
#include <driver/gpio.h>
#include "sdkconfig.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <mpu_idf_esp32.h>
#include <driver/include/mltypes.h>
#include <driver/eMPL/inv_mpu_dmp_motion_driver.h>
#include <mllite/ml_math_func.h>

signed char gyro_orientation[9] = { 1, 0, 0,
                                    0, 1, 0,
                                    0, 0, 1};

void mpu_interrupt_cb(void * data) {
    unsigned long sensor_timestamp;
    short gyro[3], accel_short[3], sensors;
    unsigned char more;
    long quat[4];

    dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more);
    if (sensors & INV_XYZ_ACCEL) {
        //printf("accel: %d, %d, %d\n", accel_short[0], accel_short[1], accel_short[2]);
    }
}

static void tap_cb(unsigned char direction, unsigned char count)
{
    switch (direction) {
    case TAP_X_UP:
        MPL_LOGI("Tap X+ ");
        break;
    case TAP_X_DOWN:
        MPL_LOGI("Tap X- ");
        break;
    case TAP_Y_UP:
        MPL_LOGI("Tap Y+ ");
        break;
    case TAP_Y_DOWN:
        MPL_LOGI("Tap Y- ");
        break;
    case TAP_Z_UP:
        MPL_LOGI("Tap Z+ ");
        break;
    case TAP_Z_DOWN:
        MPL_LOGI("Tap Z- ");
        break;
    default:
        return;
    }
    MPL_LOGI("x%d\n", count);
    return;
}

static void android_orient_cb(unsigned char orientation)
{
	switch (orientation) {
	case ANDROID_ORIENT_PORTRAIT:
        MPL_LOGI("Portrait\n");
        break;
	case ANDROID_ORIENT_LANDSCAPE:
        MPL_LOGI("Landscape\n");
        break;
	case ANDROID_ORIENT_REVERSE_PORTRAIT:
        MPL_LOGI("Reverse Portrait\n");
        break;
	case ANDROID_ORIENT_REVERSE_LANDSCAPE:
        MPL_LOGI("Reverse Landscape\n");
        break;
	default:
		return;
	}
}

void app_main(void) {
    esp_err_t res;

    /*
     * Initialize esp32 I2C
     */
    res = esp32_i2c_init(I2C_NUM_0, GPIO_NUM_21, true, GPIO_NUM_22, true, 400000, 10);
    printf("i2c init res: %d\n", res);

    inv_error_t result;

    struct int_param_s int_param;
    int_param.interrupt_pin = (unsigned int) GPIO_NUM_13;
    int_param.cb = mpu_interrupt_cb;
    result = mpu_init(&int_param); // bad idea to use a stack variable here!
    if (result) {
        MPL_LOGE("error: %d, l:%d\n", result, __LINE__);
    }

    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
    mpu_set_sample_rate(50);

    dmp_load_motion_driver_firmware();
    dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
    dmp_register_tap_cb(tap_cb);
    dmp_register_android_orient_cb(android_orient_cb);
    unsigned short dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL;
    dmp_enable_feature(dmp_features);
    dmp_set_fifo_rate(20);
    mpu_set_dmp_state(1);

    for(;;) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
