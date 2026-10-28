#include "IMUSensor.h"
#include "MadgwickIMU.h"
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12345

IMUSensor* g_imu = nullptr;
MadgwickIMU g_filter;

float bgx = 0, bgy = 0, bgz = 0;
std::chrono::steady_clock::time_point g_last;

struct Quat {
    double w, x, y, z;
};

void initIMU() {
    g_imu = new IMUSensor(0x6B, 1);

    if (!g_imu->checkWhoAmI()) {
        std::cerr << "ERROR: Not LSM6DS33 at 0x6B\n";
        std::exit(1);
    }

    g_imu->configure();
    std::cout << "Calibrating gyro... keep IMU still.\n";

    const int N = 200;
    for (int i = 0; i < N; i++) {
        auto g = g_imu->getGyroscopeData();
        bgx += g.gx;
        bgy += g.gy;
        bgz += g.gz;
        std::this_thread::sleep_for(std::chrono::milliseconds(38));
    }

    bgx /= N;
    bgy /= N;
    bgz /= N;

    std::cout << "Gyro bias: " << bgx << " " << bgy << " " << bgz << "\n";
    g_last = std::chrono::steady_clock::now();
}

Quat getNormalizedQuaternion() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - g_last;
    g_last = now;

    g_filter.dt = elapsed.count();
    if (g_filter.dt <= 0.0f) {
        g_filter.dt = 1.0f / 26.0f;
    }

    auto a = g_imu->getAccelerometerData();
    auto g = g_imu->getGyroscopeData();

    g.gx -= bgx;
    g.gy -= bgy;
    g.gz -= bgz;

    g_filter.update(g.gx, g.gy, g.gz, a.ax, a.ay, a.az);

    Quat q;
    q.w = g_filter.q0;
    q.x = g_filter.q1;
    q.y = g_filter.q2;
    q.z = g_filter.q3;

    double n = std::sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
    if (n > 1e-12) {
        q.w /= n;
        q.x /= n;
        q.y /= n;
        q.z /= n;
    } else {
        q = {1.0, 0.0, 0.0, 0.0};
    }

    return q;
}

int main() {
    initIMU();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt failed\n";
        close(server_fd);
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t addrlen = sizeof(client_addr);

        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        if (client_socket < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }

        std::cout << "Client connected!\n";

        while (true) {
            Quat q = getNormalizedQuaternion();

            std::string data =
                std::to_string(q.w) + "," +
                std::to_string(q.x) + "," +
                std::to_string(q.y) + "," +
                std::to_string(q.z) + "\n";

            ssize_t bytes_sent = send(client_socket, data.c_str(), data.length(), 0);
            if (bytes_sent <= 0) {
                std::cout << "Client disconnected.\n";
                close(client_socket);
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(38));
        }
    }

    close(server_fd);
    return 0;
}
