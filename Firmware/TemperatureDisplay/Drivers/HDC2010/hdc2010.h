#ifndef __HDC2010_H__
#define __HDC2010_H__

#include "main.h"

// Define the I2C port for the HDC2010 sensor
#define HDC2010_I2C_PORT hi2c3
extern I2C_HandleTypeDef HDC2010_I2C_PORT;

// Set the I2C address of the HDC2010 sensor
#define HDC2010_ADDR    0x40 /*!< set address PIN select GND  */
// #define HDC2010_ADDR 0x41, /*!< set address PIN select VDD  */

/* Initialize the HDC2010 Sensor */
void hdc2010_init(void);

/**
 * @brief Retreive the temperature and humidity from the HDC2010 sensor
 * 
 * @param temperature pointer to temperature
 * @param humdity pointer to humidity
 * @return void
 */
void hdc2010_read_temperature_humidity(float *temperature, float *humidity);

#endif