#include "AS5600.h"

// AS5600 Slave Address
static const uint8_t HARDWARE_ADDRESS = 0x36;

// Configuration Registers
static const uint8_t ZMCO           = 0x00;
static const uint8_t ZPOS           = 0x01;
static const uint8_t MPOS           = 0x03;
static const uint8_t MANG           = 0x05;
static const uint8_t CONF           = 0x07;

// Output Registers
static const uint8_t RAW_ANGLE      = 0x0C;
static const uint8_t ANGLE          = 0x0E;

// Status Registers
static const uint8_t STATUS         = 0x0B;
static const uint8_t AGC            = 0x1A;
static const uint8_t MAGNITUDE      = 0x1B;

// Burn Command
static const uint8_t BURN           = 0xFF;

// CONF Register Bitmasks
static const uint8_t BITMASK_PM     = 0xFC;
static const uint8_t BITMASK_HYST   = 0xF3;
static const uint8_t BITMASK_OUTS   = 0xCF;
static const uint8_t BITMASK_PWMF   = 0x3F;
static const uint8_t BITMASK_SF     = 0xFC;
static const uint8_t BITMASK_FTH    = 0xE3;
static const uint8_t BITMASK_WD     = 0xDF;


static bool reg_write(i2c_inst_t *i2c, const uint8_t reg, uint8_t *buf, uint8_t numBytes) {

    if (numBytes == 0) return false;

    uint8_t frame[++numBytes];

    frame[0] = reg;

    for (int i = 1; i < numBytes; ++i) {
        frame[i] = buf[i-1];
    }
    
    return (i2c_write_blocking(i2c, HARDWARE_ADDRESS, frame, numBytes, false) == numBytes);
    
}


static bool reg_read (i2c_inst_t *i2c, const uint8_t reg, uint8_t *buf, uint8_t numBytes) {

    if (numBytes == 0) return false;

    if (i2c_write_blocking(i2c, HARDWARE_ADDRESS, &reg, 1, true) != 1) return false;

    return (i2c_read_blocking(i2c, HARDWARE_ADDRESS, buf, numBytes, false) == numBytes);

}

// @brief  Get the error code of the previous AS5600 function
// @return AS5600 Error Code
AS5600::ERROR_CODE AS5600::getLastError() {return lastError;}

// @brief  Get value of ZMCO
// @return Number of writes to ZMCO
uint8_t AS5600::getZMCO() {
    uint8_t data;   lastError = AS5600_OK;

    if(!reg_read(i2c, ZMCO, &data, 1)) lastError = AS5600_ERROR_REGISTER_READ;

    return data;
}


// @brief  Set AS5600 Configuration
// @param  conf Config Instance
bool AS5600::setConfiguration(Config &conf) {
    uint8_t data[2];    lastError = AS5600_OK;

    data[0] = (conf.watchdog << 5) | (conf.fastFilter  << 2) | (conf.slowFilter     ) ;
    data[1] = (conf.pwmFreq  << 6) | (conf.outputStage << 4) | (conf.hysteresis << 2) | (conf.powerMode);

    if (!reg_write(i2c, CONF, data, 2)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief  Get AS5600 Configuration
// @param  conf Config Instance
bool AS5600::getConfiguration(Config &conf) {
    uint8_t data[2];    lastError = AS5600_OK;

    if (!reg_read(i2c, CONF, data, 2)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    conf.fastFilter  = (FAST_FILTER_CONFIG) ((data[0] >> 2) & 7);
    conf.hysteresis  = (HYSTERESIS_CONFIG)  ((data[1] >> 2) & 3);
    conf.outputStage = (OUTPUT_CONFIG)      ((data[1] >> 4) & 3);
    conf.powerMode   = (POWER_MODE_CONFIG)  ((data[1])      & 3);
    conf.pwmFreq     = (PWM_FREQ_CONFIG)    ((data[1] >> 6) & 3);
    conf.slowFilter  = (SLOW_FILTER_CONFIG) ((data[0])      & 3);
    conf.watchdog    = (WATCHDOG_CONFIG)    ((data[0] >> 5) & 1); 

    return true;
}


// @brief  Set Power Mode
bool AS5600::setPowerMode(POWER_MODE_CONFIG powerMode) {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF + 1, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    data &= BITMASK_PM;
    data |= powerMode;

    if(!reg_write(i2c, CONF + 1, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief  Get Power Mode
uint8_t AS5600::getPowerMode() {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF + 1, &data, 1)) lastError = AS5600_ERROR_REGISTER_READ;

    return (data & 3);
}


// @brief  Set Hysteresis
bool AS5600::setHysteresis(HYSTERESIS_CONFIG hysteresis) {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF + 1, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    data &= BITMASK_HYST;
    data |= (hysteresis << 2);

    if (!reg_write(i2c, CONF + 1, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;        
    }

    return true;
}

// @brief  Get Hysteresis
uint8_t AS5600::getHysteresis() {
    uint8_t data;   lastError = AS5600_OK;    

    if (!reg_read(i2c, CONF + 1, &data, 1)) lastError = AS5600_ERROR_REGISTER_READ;

    return ((data >> 2) & 3);
}


// @brief  Set Output Mode
bool AS5600::setOutputMode(OUTPUT_CONFIG outputMode) {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF + 1, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    data &= BITMASK_OUTS;
    data |= (outputMode << 4);

    if(!reg_write(i2c, CONF + 1, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief  Get Output Mode
uint8_t AS5600::getOutputMode() {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF + 1, &data, 1)) lastError = AS5600_ERROR_REGISTER_READ;

    return ((data >> 4) & 3);
}


// @brief  Set PWM Frequency
bool AS5600::setPWMFrequency(PWM_FREQ_CONFIG pwmFreq) {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF + 1, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    data &= BITMASK_PWMF;
    data |= (pwmFreq << 6);

    if (!reg_write(i2c, CONF + 1, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief  Get PWM Frequency
uint8_t AS5600::getPWMFrequency() {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF + 1, &data, 1)) lastError = AS5600_ERROR_REGISTER_READ;

    return ((data >> 6) & 3);
}


// @brief  Set Slow Filter Settings
bool AS5600::setSlowFilter(SLOW_FILTER_CONFIG slowFilter) {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    data &= BITMASK_SF;
    data |= slowFilter;

    if(!reg_write(i2c, CONF, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief  Get Slow Filter Settings
uint8_t AS5600::getSlowFilter() {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF, &data, 1)) lastError = AS5600_ERROR_REGISTER_READ;

    return (data & 3);
}


// @brief  Set Fast Filter Settings
bool AS5600::setFastFilter(FAST_FILTER_CONFIG fastFilter) {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    data &= BITMASK_FTH;
    data |= (fastFilter << 2);

    if (!reg_write(i2c, CONF, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief  Get Fast Filter Settings
uint8_t AS5600::getFastFilter() {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF, &data, 1)) lastError = AS5600_ERROR_REGISTER_READ;

    return ((data >> 2) & 7);
}


// @brief  Set Watchdog Settings
bool AS5600::setWatchdog(WATCHDOG_CONFIG watchdog) {
    uint8_t data;   lastError = AS5600_OK;

    if (!reg_read(i2c, CONF, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    data &= BITMASK_WD;
    data |= (watchdog << 5);

    if(!reg_write(i2c, CONF, &data, 1)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief  Get Watchdog Settings
uint8_t AS5600::getWatchdog() {
    uint8_t data;       lastError = AS5600_OK;

    if (!reg_read(i2c, CONF, &data, 1))     lastError = AS5600_ERROR_REGISTER_READ;

    return ((data >> 5) & 1);
}


/* @brief  Get Magnet Status
 * @return  
 * - 4 Magnet Too Strong
 * 
 * - 3 Magnet Strong But Working
 * 
 * - 2 Detected Magnet Working
 * 
 * - 1 Magnet Weak But Working
 * 
 * - 0 Magnet Too Weak
 */
uint8_t AS5600::getStatus() {
    uint8_t data;       lastError = AS5600_OK;

    if (!reg_read(i2c, STATUS, &data, 1))   lastError = AS5600_ERROR_REGISTER_READ;

    data >>= 3;

    if      (data == 1) return 4;
    else if (data == 5) return 3;
    else if (data == 4) return 2;
    else if (data == 6) return 1;
    
    return 0;
}


// @brief Set Start Angle
bool AS5600::setZPosition(uint16_t pos) {
    uint8_t data[2];   lastError = AS5600_OK;

    data[0] = pos >> 8;
    data[1] = pos;

    if(!reg_write(i2c, ZPOS, data, 2)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief Get Start Angle
uint16_t AS5600::getZPosition() {
    uint8_t data[2];   lastError = AS5600_OK;

    if (!reg_read(i2c, ZPOS, data, 2)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    return ((data[0]<<8) | data[1]);
}


// @brief Set Stop Angle
// @note Angle Range = Stop Angle - Start Angle
bool AS5600::setMPosition(uint16_t pos) {
    uint8_t data[2];   lastError = AS5600_OK;

    data[0] = pos >> 8;
    data[1] = pos;

    if(!reg_write(i2c, MPOS, data, 2)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief Get Stop Angle
uint16_t AS5600::getMPosition() {
    uint8_t data[2];   lastError = AS5600_OK;

    if (!reg_read(i2c, MPOS, data, 2)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    return ((data[0]<<8) | data[1]);
}


// @brief Set Max Angle
// @note Angle Range = Start Angle + Max Angle
bool AS5600::setMaxAngle(uint16_t pos) {
    uint8_t data[2];   lastError = AS5600_OK;

    data[0] = pos >> 8;
    data[1] = pos;


    if(!reg_write(i2c, MANG, data, 2)) {
        lastError = AS5600_ERROR_REGISTER_WRITE;
        return false;
    }

    return true;
}

// @brief Get Max Angle
uint16_t AS5600::getMaxAngle() {
    uint8_t data[2];   lastError = AS5600_OK;

    if (!reg_read(i2c, MANG, data, 2)) {
        lastError = AS5600_ERROR_REGISTER_READ;
        return false;
    }

    return ((data[0]<<8) | data[1]);
}

// @brief Read Unscaled Angle (No Limits)
uint16_t AS5600::readAngleRaw() {
    uint8_t data[2];   lastError = AS5600_OK;

    if (!reg_read(i2c, RAW_ANGLE, data, 2)) lastError = AS5600_ERROR_REGISTER_READ;

    return ((data[0]<<8) | data[1]);
}

// @brief Read Scaled Angle (With Limits)
uint16_t AS5600::readAngle() {
    uint8_t data[2];   lastError = AS5600_OK;

    if (!reg_read(i2c, ANGLE, data, 2))     lastError = AS5600_ERROR_REGISTER_READ;

    return ((data[0]<<8) | data[1]);
}


// @brief Read Automatic Gain Control Value
// @note  Range is 0 - 255 in 5V Operation, 0 - 128 in 3.3V mode
uint8_t AS5600::readAGC() {
    uint8_t data;       lastError = AS5600_OK;

    if(!reg_read(i2c, AGC, &data, 1))       lastError = AS5600_ERROR_REGISTER_READ;

    return data;    
}


// @brief Read Magnitude of Magnetic Field
uint16_t AS5600::readMagnitude() {
    uint8_t data[2];   lastError = AS5600_OK;

    if(!reg_read(i2c, MAGNITUDE, data, 2))  lastError = AS5600_ERROR_REGISTER_READ;

    return ((data[0]<<8) | data[1]);    
}

// @brief Burn ZPOS and MPOS into non-volatile memory
// @warning THIS OPERATION CAN ONLY BE PERFORMED A MAXIMUM OF 3 TIMES
// @note Read ZMCO to see how many times ZPOS and MPOS have been written
void AS5600::burnAngle() {
    uint8_t data = 0x80;    lastError = AS5600_OK;

    if(!reg_write(i2c, BURN, &data, 1))     lastError = AS5600_ERROR_REGISTER_WRITE;
}

// @brief Burn MANG and CONFIG into non-volatile memory
// @warning THIS OPERATION CAN ONLY BE PERFORMED ONCE
// @note This operation will only be executed if ZMCO = 0
void AS5600::burnSetting() {
    uint8_t data = 0x40;    lastError = AS5600_OK;

    if(!reg_write(i2c, BURN, &data, 1))     lastError = AS5600_ERROR_REGISTER_WRITE;
}