#ifndef __AS5600__
#define __AS5600__

#include "stdio.h"
#include "hardware/i2c.h"

// Tag Structs

struct RawData {};
struct Degrees {};
struct Radians {};

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
            AS5600_ERROR_REGISTER_READ = -1,
            AS5600_ERROR_REGISTER_WRITE = -2
        };

    private:

        static constexpr float PI     = 3.14159265358979323846f ;
        static constexpr float rawToDegrees = 360.0f  / 4096.0f ;
        static constexpr float degreesToRaw = 4096.0f / 360.0f  ;
        static constexpr float rawToRadians = 2 * PI  / 4096.0f ;
        static constexpr float radiansToRaw = 4096.0f / (2 * PI);

        float scaleToDegrees = 1;
        float scaleToRadians = 1;

        uint8_t  lastError;

        i2c_inst *i2c;

        template<typename Unit> struct angle;

        bool     _setZPosition(uint16_t pos);
        uint16_t _getZPosition();

        bool     _setMPosition(uint16_t pos);
        uint16_t _getMPosition();

        bool     _setMaxAngle (uint16_t pos);
        uint16_t _getMaxAngle();

        uint16_t _readAngleRaw();
        uint16_t _readAngle();

    public:

        AS5600(i2c_inst *i2c = i2c0) {
            AS5600::i2c = i2c;
        };

        uint8_t  getLastErrorCode()   {
            return lastError;
        };
    
        template <typename Unit> bool setZPosition(typename angle<Unit>::dataType pos);
        template <typename Unit> typename angle<Unit>::dataType getZPosition();

        template <typename Unit> bool setMPosition(typename angle<Unit>::dataType pos);
        template <typename Unit> typename angle<Unit>::dataType getMPosition();

        template <typename Unit> bool setMaxAngle (typename angle<Unit>::dataType pos);
        template <typename Unit> typename angle<Unit>::dataType getMaxAngle();
        
        template <typename Unit> typename angle<Unit>::dataType readAngleRaw();
        template <typename Unit> typename angle<Unit>::dataType readAngle();

        uint8_t  getZMCO();
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

#include "AS5600_Templates.tpp"

#endif