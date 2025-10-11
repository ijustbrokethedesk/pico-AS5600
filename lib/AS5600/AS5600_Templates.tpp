template<> struct AS5600::angle<RawData> { typedef uint16_t dataType; };
template<> struct AS5600::angle<Degrees> { typedef float    dataType; };
template<> struct AS5600::angle<Radians> { typedef float    dataType; };  


template<> inline bool AS5600::setZPosition<RawData>(uint16_t angle)  { return _setZPosition(angle);                };
template<> inline bool AS5600::setZPosition<Degrees>(float angle   )  { return _setZPosition(angle * degreesToRaw); };
template<> inline bool AS5600::setZPosition<Radians>(float angle   )  { return _setZPosition(angle * radiansToRaw); };

template<> inline bool AS5600::setMPosition<RawData>(uint16_t angle)  { return _setMPosition(angle);                };
template<> inline bool AS5600::setMPosition<Degrees>(float angle   )  { return _setMPosition(angle * degreesToRaw); };
template<> inline bool AS5600::setMPosition<Radians>(float angle   )  { return _setMPosition(angle * radiansToRaw); };

template<> inline bool AS5600::setMaxAngle<RawData>(uint16_t angle )  { return _setMaxAngle(angle);                 };
template<> inline bool AS5600::setMaxAngle<Degrees>(float angle    )  { return _setMaxAngle(angle * degreesToRaw);  };
template<> inline bool AS5600::setMaxAngle<Radians>(float angle    )  { return _setMaxAngle(angle * radiansToRaw);  };


template<> inline uint16_t  AS5600::getZPosition<RawData>()  { return _getZPosition();                };
template<> inline float     AS5600::getZPosition<Degrees>()  { return _getZPosition() * rawToDegrees; };
template<> inline float     AS5600::getZPosition<Radians>()  { return _getZPosition() * rawToRadians; };

template<> inline uint16_t  AS5600::getMPosition<RawData>()  { return _getMPosition();                };
template<> inline float     AS5600::getMPosition<Degrees>()  { return _getMPosition() * rawToDegrees; };
template<> inline float     AS5600::getMPosition<Radians>()  { return _getMPosition() * rawToRadians; };

template<> inline uint16_t  AS5600::getMaxAngle<RawData>()   { return _getMaxAngle();                 };
template<> inline float     AS5600::getMaxAngle<Degrees>()   { return _getMaxAngle() * rawToDegrees;  };
template<> inline float     AS5600::getMaxAngle<Radians>()   { return _getMaxAngle() * rawToRadians;  };


template<> inline uint16_t  AS5600::readAngleRaw<RawData>()  { return _readAngleRaw();                };
template<> inline float     AS5600::readAngleRaw<Degrees>()  { return _readAngleRaw() * rawToDegrees; };
template<> inline float     AS5600::readAngleRaw<Radians>()  { return _readAngleRaw() * rawToRadians; };

template<> inline uint16_t  AS5600::readAngle<RawData>()     { return _readAngle();                   };
template<> inline float     AS5600::readAngle<Degrees>()     { return _readAngle() * scaleToDegrees;  };
template<> inline float     AS5600::readAngle<Radians>()     { return _readAngle() * scaleToRadians;  }; 