#ifndef __AS5600__
#define __AS5600__

#include "stdio.h"
#include "hardware/i2c.h"

class AS5600 {

    public:

        enum WATCHDOG_CONFIG{
            WD_OFF,
            WD_ON 
        };

        enum HYSTERESIS_CONFIG  {
            HYST_OFF,
            HYST_1LSB,
            HYST_2LSB,
            HYST_3LSB
        };

        enum OUTPUT_CONFIG      {
            ANALOG_100PERCENT,
            ANALOG_90PERCENT,
            PWM
        };

        enum PWM_FREQ_CONFIG    {
            PWM_115HZ,
            PWM_230HZ,
            PWM_460HZ,
            PWM_920HZ
        };

        enum POWER_MODE_CONFIG  {
            POWER_NORMAL,
            LOW_POWER_MODE1,
            LOW_POWER_MODE2,
            LOW_POWER_MODE3
        };

        enum SLOW_FILTER_CONFIG {
            SLOW_FILTER_16x,
            SLOW_FILTER_8x,
            SLOW_FILTER_4x,
            SLOW_FILTER_2x
        };

        enum FAST_FILTER_CONFIG {
            FAST_FILTER_OFF,
            FAST_FILTER_6LSB,
            FAST_FILTER_7LSB,
            FAST_FILTER_9LSB,
            FAST_FILTER_18LSB, 
            FAST_FILTER_21LSB,
            FAST_FILTER_24LSB,
            FAST_FILTER_10LSB
        };

        struct Config           {
            POWER_MODE_CONFIG   powerMode   =  POWER_MODE_CONFIG::POWER_NORMAL;
            HYSTERESIS_CONFIG   hysteresis  =  HYSTERESIS_CONFIG::HYST_OFF;
            OUTPUT_CONFIG       outputStage =  OUTPUT_CONFIG::ANALOG_100PERCENT;
            PWM_FREQ_CONFIG     pwmFreq     =  PWM_FREQ_CONFIG::PWM_115HZ;
            SLOW_FILTER_CONFIG  slowFilter  =  SLOW_FILTER_CONFIG::SLOW_FILTER_16x;
            FAST_FILTER_CONFIG  fastFilter  =  FAST_FILTER_CONFIG::FAST_FILTER_OFF;
            WATCHDOG_CONFIG     watchdog    =  WATCHDOG_CONFIG::WD_OFF;
        };

        enum MAGNET_STATE {
            MAGNET_WEAK_FAULT,
            MAGNET_WEAK_OPERATING,
            MAGNET_NORMAL_OPERATING,
            MAGNET_STRONG_OPERATING,
            MAGNET_STRONG_FAULT
        };

        enum ERROR_CODE {
            AS5600_OK = 0,
            AS5600_ERROR_INIT = -1,
            AS5600_ERROR_REGISTER_READ = -2,
            AS5600_ERROR_REGISTER_WRITE = -3
        };

    private:

        ERROR_CODE  lastError;

        i2c_inst *i2c;

    public:

        AS5600(i2c_inst *i2c = i2c0) {
            AS5600::i2c = i2c;
        };

        ERROR_CODE  getLastError();

        uint8_t  getZMCO();

        bool     setZPosition(uint16_t pos);
        uint16_t getZPosition();

        bool     setMPosition(uint16_t pos);
        uint16_t getMPosition();

        bool     setMaxAngle(uint16_t pos);
        uint16_t getMaxAngle();
        
        uint16_t readAngleRaw();
        uint16_t readAngle();

        uint8_t  getStatus();
        uint8_t  readAGC();
        uint16_t readMagnitude();

        bool     setConfiguration(Config &conf);
        bool     getConfiguration(Config &conf);

        bool     setPowerMode(POWER_MODE_CONFIG powerMode);
        uint8_t  getPowerMode();

        bool     setHysteresis(HYSTERESIS_CONFIG hysteresis);
        uint8_t  getHysteresis();

        bool     setOutputMode(OUTPUT_CONFIG outputMode);
        uint8_t  getOutputMode();

        bool     setPWMFrequency(PWM_FREQ_CONFIG pwmFreq);
        uint8_t  getPWMFrequency();

        bool     setSlowFilter(SLOW_FILTER_CONFIG slowFilter);
        uint8_t  getSlowFilter();

        bool     setFastFilter(FAST_FILTER_CONFIG fastFilter);
        uint8_t  getFastFilter();

        bool     setWatchdog(WATCHDOG_CONFIG watchdog);
        uint8_t  getWatchdog();



        void     burnAngle();
        void     burnSetting();

};

#endif