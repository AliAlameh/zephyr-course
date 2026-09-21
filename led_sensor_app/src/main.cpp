#include <led_sensor/led_sensor.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 1000
#define TOGGLE_COUNT_RESET_EVERY 4

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	const struct device *dev = DEVICE_DT_GET_ANY(led_sensor);
	int iterations = 0;

	if (dev == NULL || !device_is_ready(dev)) {
		LOG_ERR("LED sensor device not ready");
		return 0;
	}

	while (1) {
		struct sensor_value val;

		sensor_sample_fetch(dev);
		LOG_INF("Fetched sample: LED ON");
		k_msleep(SLEEP_TIME_MS);

		sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
		LOG_INF("Channel get: LED was %s, now OFF", val.val1 ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);

		if (++iterations >= TOGGLE_COUNT_RESET_EVERY) {
			LOG_INF("Resetting driver's toggle_count via extension API");
			led_sensor_set_toggle_count(dev, 0);
			iterations = 0;
		}
	}

	return 0;
}
