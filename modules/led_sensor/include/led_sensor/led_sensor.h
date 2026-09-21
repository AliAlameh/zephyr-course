/*
 * LED Sensor Driver - Public API
 *
 * The driver is used through the standard Zephyr sensor API
 * (sensor_sample_fetch / sensor_channel_get). This header adds a custom
 * extension function for setting the driver's internal toggle counter.
 */
#ifndef LED_SENSOR_H
#define LED_SENSOR_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Overwrite the driver's internal LED toggle counter.
 *
 * @param dev   LED sensor device instance.
 * @param count New value for the toggle counter kept in the driver's data struct.
 * @return 0 on success, -ENOTSUP if dev is not a led_sensor device.
 */
int led_sensor_set_toggle_count(const struct device *dev, uint32_t count);

#ifdef __cplusplus
}
#endif

#endif /* LED_SENSOR_H */
