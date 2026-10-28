#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

class I2CDevice {
protected:
    int file{-1};
    unsigned char deviceAddress{0};

public:
    I2CDevice(unsigned char address, int bus = 1) : deviceAddress(address) {
        std::string name = "/dev/i2c-" + std::to_string(bus);

        file = open(name.c_str(), O_RDWR);
        if (file < 0) {
            std::cerr << "Failed to open I2C bus " << name << ": "
                      << std::strerror(errno) << "\n";
            std::exit(1);
        }

        if (ioctl(file, I2C_SLAVE, deviceAddress) < 0) {
            std::cerr << "Failed to connect to I2C device 0x"
                      << std::hex << (int)deviceAddress << std::dec
                      << ": " << std::strerror(errno) << "\n";
            std::exit(1);
        }
    }

    // Write 1 byte value into register
    virtual void writeRegister(unsigned char reg, unsigned char value) {
        unsigned char buffer[2] = {reg, value};
        if (write(file, buffer, 2) != 2) {
            std::cerr << "writeRegister failed: " << std::strerror(errno) << "\n";
            std::exit(1);
        }
    }

    // Read 1 byte from register
    virtual unsigned char readRegister(unsigned char reg) {
        if (write(file, &reg, 1) != 1) {
            std::cerr << "readRegister (set reg) failed: " << std::strerror(errno) << "\n";
            std::exit(1);
        }
        unsigned char value = 0;
        if (read(file, &value, 1) != 1) {
            std::cerr << "readRegister (read) failed: " << std::strerror(errno) << "\n";
            std::exit(1);
        }
        return value;
    }

    // Read N bytes starting at startReg (requires IF_INC enabled on sensor)
    virtual void readRegisters(unsigned char startReg, unsigned char* buffer, size_t len) {
        if (write(file, &startReg, 1) != 1) {
            std::cerr << "readRegisters (set start reg) failed: " << std::strerror(errno) << "\n";
            std::exit(1);
        }
        if (read(file, buffer, len) != (ssize_t)len) {
            std::cerr << "readRegisters (read block) failed: " << std::strerror(errno) << "\n";
            std::exit(1);
        }
    }

    virtual ~I2CDevice() {
        if (file >= 0) close(file);
    }
};

#endif
