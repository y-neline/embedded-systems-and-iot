#include <iostream>
#include "IMUSensor.h"
#include "MadgwickIMU.h"

int main() {
    // 1. Mandatory CGI Header
    std::cout << "Content-type: text/html\n\n";

    // 2. Initialize Sensor (Reusing your existing classes)
    IMUSensor imu(0x6B, 1);
    imu.configure();
    MadgwickIMU filter;

    // 3. Get one measurement
    auto a = imu.getAccelerometerData();
    auto g = imu.getGyroscopeData();
    filter.update(g.gx, g.gy, g.gz, a.ax, a.ay, a.az);

    // 4. Output as HTML for the browser
    std::cout << "<html><body>";
    std::cout << "<h1>IMU Quaternion Data</h1>";
    std::cout << "<p>W: " << filter.q0 << "</p>";
    std::cout << "<p>X: " << filter.q1 << "</p>";
    std::cout << "<p>Y: " << filter.q2 << "</p>";
    std::cout << "<p>Z: " << filter.q3 << "</p>";
    std::cout << "</body></html>";
    
    return 0;
}
