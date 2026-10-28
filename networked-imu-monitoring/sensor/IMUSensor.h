#ifndef IMU_SENSOR_H
#define IMU_SENSOR_H

#include "I2CDevice.h"
#include <cstdint>
#include <cmath>

class IMUSensor : public I2CDevice {
public:
    explicit IMUSensor(unsigned char address = 0x6B, int bus = 1)
        : I2CDevice(address, bus) {}

    // -------- LSM6DS33 registers --------
    static constexpr uint8_t WHO_AM_I  = 0x0F; // expect 0x69
    static constexpr uint8_t CTRL1_XL  = 0x10; // accel config
    static constexpr uint8_t CTRL2_G   = 0x11; // gyro config
    static constexpr uint8_t CTRL3_C   = 0x12; // common config
    static constexpr uint8_t OUTX_L_G  = 0x22; // gyro output start (L)
    static constexpr uint8_t OUTX_L_XL = 0x28; // accel output start (L)

    // LSB sensitivity (typical datasheet values)
    // ±2g:   0.061 mg/LSB
    // 245 dps: 8.75 mdps/LSB
    static constexpr float ACCEL_MG_PER_LSB_2G   = 0.061f;
    static constexpr float GYRO_MDPS_PER_LSB_500 = 17.50f;

    struct AccelG { float ax, ay, az; };        // in g
    struct GyroRad { float gx, gy, gz; };       // in rad/s

    bool checkWhoAmI() {
        uint8_t v = readRegister(WHO_AM_I);
        std::cout << "LSM6DS33 WHO_AM_I = 0x" << std::hex << (int)v << std::dec << "\n";
        return v == 0x69;
    }

    // Configure LSM6DS33: enable auto-increment + set 104Hz, ±2g, 245dps
void configure() {
        writeRegister(CTRL3_C, 0x44); // IF_INC = 1
        writeRegister(CTRL1_XL, 0x20); // 26 Hz, ±2g
        writeRegister(CTRL2_G,  0x24); // 26 Hz, 500 dps
        std::cout << "LSM6DS33 configured.\n";
    }

    // Read 3-axis accel, return in g
    AccelG getAccelerometerData() {
        uint8_t b[6]{};
        readRegisters(OUTX_L_XL, b, 6);

        int16_t x = (int16_t)(b[1] << 8 | b[0]);
        int16_t y = (int16_t)(b[3] << 8 | b[2]);
        int16_t z = (int16_t)(b[5] << 8 | b[4]);

        float g_per_lsb = (ACCEL_MG_PER_LSB_2G / 1000.0f); // mg -> g
        return { x * g_per_lsb, y * g_per_lsb, z * g_per_lsb };
    }

    // Read 3-axis gyro, return in rad/s
    GyroRad getGyroscopeData() {
        uint8_t b[6]{};
        readRegisters(OUTX_L_G, b, 6);

        int16_t x = (int16_t)(b[1] << 8 | b[0]);
        int16_t y = (int16_t)(b[3] << 8 | b[2]);
        int16_t z = (int16_t)(b[5] << 8 | b[4]);

        float dps_per_lsb = (GYRO_MDPS_PER_LSB_500 / 1000.0f); // mdps -> dps
        constexpr float DEG2RAD = 3.14159265358979323846f / 180.0f;

        return { x * dps_per_lsb * DEG2RAD,
                 y * dps_per_lsb * DEG2RAD,
                 z * dps_per_lsb * DEG2RAD };
    }
};

#endif
