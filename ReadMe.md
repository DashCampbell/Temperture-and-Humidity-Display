# Temperature & Humidity Display

A basic PCB displaying the temperature and humidity from a sensor to a OLED screen.

* Uses a STM32 microcontroller with SWD.
* Uses FreeRTOS to run tasks concurrently.
* Uses I2C to communicate with the OLED display and sensor.
* Uses PWM to controll a RGB addressable LED.