#include "hdc2010.h"
#include "cmsis_os2.h"
#include "stm32_hal_legacy.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_def.h"
#include "stm32l4xx_hal_i2c.h"

// Macros taken from https://github.com/espressif/esp-iot-solution/blob/23c164ec7f748957e41c2f39a6ed9a6d4b4af56a/components/sensors/humiture/hdc2010/include/hdc2010.h#L31
#define HDC2010_TEMP_LOW                0x00             /*Temperature [7:0]   */
#define HDC2010_TEMP_HIGH               0x01             /*Temperature [15:8]  */
#define HDC2010_HUM_LOW                 0x02             /*Humidity [7:0] */
#define HDC2010_HUM_HIGH                0x03             /*Humidity [15:8]*/
#define HDC2010_INTERRUPT               0x04             /*DataReady and interrupt configuration*/
#define HDC2010_TEMPE_MAX               0x05             /*Max temperature value measured (peak detector)*/
#define HDC2010_HUM_MAX                 0x06             /*Max humidity value measured (peak detector)*/
#define HDC2010_INT_MASK                0x07             /*Interrupt Mask*/
#define HDC2010_TEMP_OFFSET             0x08             /*Temperature offset adjustment*/
#define HDC2010_HUM_OFFSET              0x09             /*Humidity offset adjustment*/
#define HDC2010_TEMP_THR_L              0x0A             /*Temperature Threshold Low*/
#define HDC2010_TEMP_THR_H              0x0B             /*Temperature Threshold High*/
#define HDC2010_HUM_THR_L               0x0C             /*Humidity threshold Low*/
#define HDC2010_HUM_THR_H               0x0D             /*Humidity threshold High*/
#define HDC2010_RESET_INT_CONF          0x0E             /*Soft Reset and Interrupt Configuration*/
#define HDC2010_MEASURE_CONF            0x0F             /*Measurement configuration*/
#define HDC2010_MANUFACTURER_ID_L       0xFC             /*Manufacturer ID Low*/
#define HDC2010_MANUFACTURER_ID_H       0xFD             /*Manufacturer ID High*/
#define HDC2010_DEVICE_ID_L             0xFE             /*Device ID Low*/
#define HDC2010_DEVICE_ID_H             0xFF             /*Device ID High*/

HAL_StatusTypeDef hdc2010_init(void) {
    // Initialization procedure for HDC2010 sensor
    // https://www.ti.com/lit/ug/snau250/snau250.pdf?ts=1786056106867#page=9

    // Interrupt based workflow:
    // https://www.ti.com/lit/ug/snau250/snau250.pdf?ts=1786056106867#page=11

    // select the desired Auto Measurement Mode (AMM) in CONFIG register (0x0E)
    // AMM Conversion Rate is 1Hz
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Write_DMA(&HDC2010_I2C_PORT, HDC2010_ADDR, HDC2010_RESET_INT_CONF, I2C_MEMADD_SIZE_8BIT, (uint8_t[]) {0x50}, 1);
    if (ret != HAL_OK)
        return ret;
    
    // select the Temperature and Humidity resolutions and the temperature + humidity 
    // measurement configuration in MEAS_CONFIG register (0x0F)
    ret = HAL_I2C_Mem_Write_DMA(&HDC2010_I2C_PORT, HDC2010_ADDR, HDC2010_MEASURE_CONF, I2C_MEMADD_SIZE_8BIT, (uint8_t[]) {0x01}, 1);
    if (ret != HAL_OK)
        return ret;

    // enable interrupt write register 0x07 -> 0x80
    // Interrupt pin is active low
    return HAL_I2C_Mem_Write_DMA(&HDC2010_I2C_PORT, HDC2010_ADDR, HDC2010_INT_MASK, I2C_MEMADD_SIZE_8BIT, (uint8_t[]) {0x80}, 1);
}

HAL_StatusTypeDef hdc2010_read_temperature_humidity(float *temperature, float *humidity) {
    uint8_t data[4] = {0};
    // Will read from registers 0x00 to 0x03, which are the temperature and humidity registers
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read_DMA(&HDC2010_I2C_PORT, HDC2010_ADDR, HDC2010_TEMP_LOW, I2C_MEMADD_SIZE_8BIT, data, 4);
    if (status != HAL_OK)
        return status;

    // Convert register output
    // Reference: https://www.ti.com/lit/ug/snau250/snau250.pdf?ts=1786056106867#page=12
    *temperature = ((data[1] << 8) | data[0]) * 165.0 / 65536.0 - 40.0;
    *humidity = ((data[3] << 8) | data[2]) * 100.0 / 65536.0;

    return HAL_OK;
}