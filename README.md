# RaspberryPi Pico AS5600 Library

A lightweight C++ driver for the **AMS AS5600** magnetic rotary position sensor. Designed for use with the **RaspberryPi Pico** series and fully compatible with the official Pico C/C++ SDK.

## Contents

- [Basic Usage](#basic-usage)

   - [Driver Initialization](#driver-initialization)
   - [Units](#units)
   - [Reading Angles](#reading-angles)
   - [Setting Configurations](#setting-configurations)
   - [Example Code](#example-code)

- [Functions](#functions)

## Basic Usage

### Driver Initialization
To use this driver, first initialize your I²C interface on the Pico. The AS5600 can support I²C speeds up to 1MHz.
The example below shows how to initialize **I2C0**, using pins GP0 (SDA) and GP1 (SCL).  
After that, an `AS5600` object is created, ready for communication.

```
i2c_init(i2c0, 1000000);             // Set I2C Fast Mode+ @ 1 MHz
gpio_set_function(0, GPIO_FUNC_I2C); // SDA
gpio_set_function(1, GPIO_FUNC_I2C); // SCL
gpio_pull_up(0);
gpio_pull_up(1);

AS5600 sensor(i2c0);
```

The library automatically uses the AS5600 default I²C address (`0x36`).

### Units
The AS5600 sensor measures angles internally as 12-bit unsigned integers (0 to 4096).  
This library allows you to utilize certain functions with **degrees** or **radians** using template tags.

| Tag | Description | Data Type |
|------|--------------|------------|
| `RawData` | Use function with raw data (0-4096) | `uint16_t` |
| `Degrees` | Use function in degrees (0°-360°) | `float` |
| `Radians` | Use function in radians (0-2π) | `float` |

### Reading Angles
The AS5600 can return both **unscaled** and **scaled** angles.  
- **Unscaled angles** represent the absolute angle reading, independent of the user defined configuration.  
- **Scaled angles** are adjusted based on the configured angle limits (ZPOS, MPOS, and MAXANGLE).

The following code reads **unscaled** data:

```
uint16_t unscaledAngleRawData = sensor.readAngleRaw<RawData>();
float    unscaledAngleDegrees = sensor.readAngleRaw<Degrees>();
float    unscaledAngleRadians = sensor.readAngleRaw<Radians>();
```

The following code reads **scaled** angle values, which reflect the range set by your own configuration.

```
uint16_t scaledAngleRawData = sensor.readAngle<RawData>();
float    scaledAngleDegrees = sensor.readAngle<Degrees>();
float    scaledAngleRadians = sensor.readAngle<Radians>();
```

### Setting Configurations
The AS5600 output can be configured easily using this library.  

You can modify each setting individually:

```
sensor.setPowerMode(LOW_POWER_MODE1);
sensor.setHysteresis(HYST_1LSB);
sensor.setOutputMode(ANALOG_90PERCENT);
sensor.setPWMFrequency(PWM_230HZ);
sensor.setSlowFilter(SLOW_FILTER_8x);
sensor.setFastFilter(FAST_FILTER_6LSB);
sensor.setWatchdog(WD_ON);
```

Alternatively, you can use the `AS5600::Config` struct to set all parameters in one operation:

```
AS5600::Config config;

config.powerMode   = POWER_NORMAL;
config.hysteresis  = HYST_2LSB;
config.outputStage = ANALOG_100PERCENT;
config.pwmFreq     = PWM_460HZ;
config.slowFilter  = SLOW_FILTER_4x;
config.fastFilter  = FAST_FILTER_10LSB;
config.watchdog    = WD_OFF;

sensor.setConfiguration(config);
```

This is convenient when you want to set multiple settings in one operation.  

You can also get the current configuration by doing:

```
AS5600::Config current;
sensor.getConfiguration(current);
```

### Example Code
An example demonstrating initialization, configuration, and angle measurement.

```
#include "pico/stdlib.h"
#include "AS5600.h"

int main() {
    stdio_init_all();

    i2c_init(i2c0, 1000000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    AS5600 sensor(i2c0);

    sensor.setPowerMode(POWER_NORMAL);
    sensor.setHysteresis(HYST_1LSB);
    sensor.setFastFilter(FAST_FILTER_10LSB);

    while (true) {
        float angleDeg = sensor.readAngle<Degrees>();
        printf("Angle: %.2f°\n", angleDeg);
        sleep_ms(200);
    }

    return 0;
}
```

## Functions


### AS5600
- **Description:** Creates an AS5600 object using the specified I²C interface (default: `i2c0`).
- **Parameters:**  
  - `i2c` - Pointer to the I²C instance.
- **Returns:** None.

### getLastErrorCode
- **Description:** Returns the last I²C or register access error code.
- **Parameters:** None.
- **Returns:** `uint8_t` - The last error code.


### setZPosition
- **Description:** Sets the start (zero) position in the specified unit.
- **Parameters:**  
  - `pos` - Zero position in `RawData`, `Degrees`, or `Radians`.
- **Returns:** `bool` - `true` if successful.

### getZPosition
- **Description:** Reads the current zero position value.
- **Parameters:** None.
- **Returns:** Template type - Zero position in the selected unit.


### setMPosition
- **Description:** Sets the maximum (end) position in the specified unit.
- **Parameters:**  
  - `pos` - Maximum position in `RawData`, `Degrees`, or `Radians`.
- **Returns:** `bool` - `true` if successful.

### getMPosition
- **Description:** Reads the configured maximum position.
- **Parameters:** None.
- **Returns:** Template type - Maximum position value.


### setMaxAngle
- **Description:** Sets the maximum measurable angle range.
- **Parameters:**  
  - `pos` - Maximum angle in `RawData`, `Degrees`, or `Radians`.
- **Returns:** `bool` - `true` if successful.

### getMaxAngle
- **Description:** Reads the configured maximum angle range.
- **Parameters:** None.
- **Returns:** Template type - Maximum angle.


### readAngleRaw
- **Description:** Reads the raw, unscaled angle value.
- **Parameters:** None.
- **Returns:** Template type - Raw angle data.

### readAngle
- **Description:** Reads the scaled angle in the specified unit.
- **Parameters:** None.
- **Returns:** Template type - Angle in `RawData`, `Degrees`, or `Radians`.


### getZMCO
- **Description:** Reads the ZMCO register, showing how many times zero has been programmed.
- **Parameters:** None.
- **Returns:** `uint8_t` - ZMCO register value.

### getStatus
- **Description:** Reads the sensor’s status byte, including magnet and system health flags.
- **Parameters:** None.
- **Returns:** `uint8_t` - Status register value.

### readAGC
- **Description:** Reads the automatic gain control (AGC) value.
- **Parameters:** None.
- **Returns:** `uint8_t` - AGC level.

### readMagnitude
- **Description:** Reads the magnetic field strength magnitude.
- **Parameters:** None.
- **Returns:** `uint16_t` - Magnitude of magnetic field.


### setConfiguration
- **Description:** Writes all configuration parameters from a `Config` struct to the device.
- **Parameters:**  
  - `conf` - Reference to a `Config` structure.
- **Returns:** `bool` - `true` if successful.

### getConfiguration
- **Description:** Reads all configuration parameters into a `Config` struct.
- **Parameters:**  
  - `conf` - Reference to a `Config` structure to populate.
- **Returns:** `bool` - `true` if successful.


### setPowerMode
- **Description:** Sets the power mode configuration.
- **Parameters:**  
  - `powerMode` - Power mode enum value (`POWER_NORMAL`, `LOW_POWER_MODE1`, etc.).
- **Returns:** `bool` - `true` if successful.

### getPowerMode
- **Description:** Reads the current power mode setting.
- **Parameters:** None.
- **Returns:** `uint8_t` - Power mode value.


### setHysteresis
- **Description:** Sets the hysteresis configuration.
- **Parameters:**  
  - `hysteresis` - Hysteresis enum value (`HYST_OFF`, `HYST_1LSB`, etc.).
- **Returns:** `bool` - `true` if successful.

### getHysteresis
- **Description:** Reads the current hysteresis configuration.
- **Parameters:** None.
- **Returns:** `uint8_t` - Hysteresis setting.


### setOutputMode
- **Description:** Configures the output stage mode.
- **Parameters:**  
  - `outputMode` - Output mode (`ANALOG_100PERCENT`, `ANALOG_90PERCENT`, `PWM`).
- **Returns:** `bool` - `true` if successful.

### getOutputMode
- **Description:** Reads the current output stage configuration.
- **Parameters:** None.
- **Returns:** `uint8_t` - Output mode value.


### setPWMFrequency
- **Description:** Sets the PWM output frequency.
- **Parameters:**  
  - `pwmFreq` - PWM frequency enum (`PWM_115HZ`, `PWM_230HZ`, etc.).
- **Returns:** `bool` - `true` if successful.

### getPWMFrequency
- **Description:** Reads the PWM frequency configuration.
- **Parameters:** None.
- **Returns:** `uint8_t` - PWM frequency value.


### setSlowFilter
- **Description:** Sets the slow filter configuration.
- **Parameters:**  
  - `slowFilter` - Slow filter setting (`SLOW_FILTER_16x`, `SLOW_FILTER_8x`, etc.).
- **Returns:** `bool` - `true` if successful.

### getSlowFilter
- **Description:** Reads the current slow filter configuration.
- **Parameters:** None.
- **Returns:** `uint8_t` - Slow filter setting.


### setFastFilter
- **Description:** Sets the fast filter configuration.
- **Parameters:**  
  - `fastFilter` - Fast filter setting (`FAST_FILTER_OFF`, `FAST_FILTER_6LSB`, etc.).
- **Returns:** `bool` - `true` if successful.

### getFastFilter
- **Description:** Reads the current fast filter configuration.
- **Parameters:** None.
- **Returns:** `uint8_t` - Fast filter setting.


### setWatchdog
- **Description:** Enables or disables the watchdog feature.
- **Parameters:**  
  - `watchdog` - Watchdog setting (`WD_ON`, `WD_OFF`).
- **Returns:** `bool` - `true` if successful.

### getWatchdog
- **Description:** Reads the watchdog configuration.
- **Parameters:** None.
- **Returns:** `uint8_t` - Watchdog setting.


### burnAngle
- **Description:** Permanently stores the current zero position into EEPROM.
- **Parameters:** None.
- **Returns:** None.

### burnSetting
- **Description:** Permanently stores the configuration settings into EEPROM.
- **Parameters:** None.
- **Returns:** None.