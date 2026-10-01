#include <led_sensor/led_sensor.h>
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_string_conv.h>
#include <zephyr/usb/usb_device.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct device *const sensor_dev = DEVICE_DT_GET_ANY(led_sensor);

static int sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (sensor_dev == NULL || !device_is_ready(sensor_dev)) {
		shell_error(sh, "LED sensor device not ready");
		return -ENODEV;
	}

	int ret = sensor_sample_fetch(sensor_dev);

	if (ret < 0) {
		shell_error(sh, "Sample fetch failed (%d)", ret);
		return ret;
	}

	shell_print(sh, "Sample fetched: LED ON");
	return 0;
}

static int sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (sensor_dev == NULL || !device_is_ready(sensor_dev)) {
		shell_error(sh, "LED sensor device not ready");
		return -ENODEV;
	}

	struct sensor_value val;
	int ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_ALL, &val);

	if (ret < 0) {
		shell_error(sh, "Channel get failed (%d)", ret);
		return ret;
	}

	shell_print(sh, "LED was %s (value: %d), now OFF", val.val1 ? "ON" : "OFF", val.val1);
	return 0;
}

static int sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "Device: %s, ready: %s", sensor_dev ? sensor_dev->name : "not found",
		    sensor_dev && device_is_ready(sensor_dev) ? "yes" : "no");
	return 0;
}

static int sensor_set(const struct shell *sh, size_t argc, char **argv)
{
	if (argc != 2) {
		shell_error(sh, "Missing value (expected 0 to %u)", UINT32_MAX);
		return -EINVAL;
	}

	int err = 0;
	unsigned long long count = shell_strtoull(argv[1], 10, &err);

	if (err || argv[1][0] == '-' || count > UINT32_MAX) {
		shell_error(sh, "Invalid value: expected 0 to %u", UINT32_MAX);
		return -EINVAL;
	}

	if (sensor_dev == NULL || !device_is_ready(sensor_dev)) {
		shell_error(sh, "LED sensor device not ready");
		return -ENODEV;
	}

	int ret = led_sensor_set_toggle_count(sensor_dev, (uint32_t)count);

	if (ret < 0) {
		shell_error(sh, "Failed to set toggle count (%d)", ret);
		return ret;
	}

	shell_print(sh, "Toggle count set to %u", (uint32_t)count);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_commands,
	SHELL_CMD(fetch, NULL, "Fetch an LED sensor sample", sensor_fetch),
	SHELL_CMD(read, NULL, "Read the last LED sensor sample", sensor_read),
	SHELL_CMD(info, NULL, "Show LED sensor device information", sensor_info),
	SHELL_CMD_ARG(set, NULL, "Set LED toggle count (0 to 4294967295)", sensor_set, 1, 1),
	SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(sensor, &sensor_commands, "LED sensor commands", NULL);

int main(void)
{
	if (usb_enable(NULL)) {
		LOG_ERR("USB CDC initialization failed");
		return 0;
	}

	if (sensor_dev == NULL || !device_is_ready(sensor_dev)) {
		LOG_ERR("LED sensor device not ready");
		return 0;
	}

	LOG_INF("LED sensor ready; use sensor fetch, read, info, or set");

	return 0;
}
