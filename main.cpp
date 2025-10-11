#include <stdio.h>
#include "pico/stdlib.h"
#include "AS5600/AS5600.h"

int main()
{
    stdio_init_all();

    // Initialize I2C0 with pins 0 (SDA) and 1 (SCL)

    i2c_init(i2c0, 1000000);             // Fast Mode+ @ 1MHz
    gpio_set_function(0, GPIO_FUNC_I2C); // Set GP0 to I2C function
    gpio_set_function(1, GPIO_FUNC_I2C); // Set GP1 to I2C function


    // Create AS5600 Object
    AS5600 sensor(i2c0);
    
    while (true) {
        printf("%d\n", sensor.readAngleRaw()); // Print raw angle data over serial
        sleep_ms(5);
    }
}