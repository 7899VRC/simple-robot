#ifndef UTILS_H
#define UTILS_H


// Degrees -> radians
inline float toRadians(float degrees) {
  return degrees * M_PI / 180.0;
}


// Radians -> degrees
inline float toDegrees(float radians) {
  return radians * 180.0 / M_PI;
}


// Fix angle so that -180 < angle <= 180
inline float normalizeAngle(float degrees) {
  while (degrees <= -180) {
    degrees += 360;
  }
  while (degrees > 180) {
    degrees -= 360;
  }
  return degrees;
}


float clamp(float value, float low, float high) {
  return fmax(low, fmin(value, high));
}


#endif