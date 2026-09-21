cmake_minimum_required(VERSION 3.20.0)

set(ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/../modules/led_sensor)

find_package(Zephyr REQUIRED)
project(led_sensor_app)

target_sources(app PRIVATE src/main.cpp)
