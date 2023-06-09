#include <cstdio>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <iomanip>

std::string exec(const char* cmd) {
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    char buffer[128];
    try {
        while (fgets(buffer, sizeof(buffer), pipe))
            result += buffer;
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

std::vector<std::string> getWifiList() {
    std::string cmd = "netsh wlan show profile";
    std::string result = exec(cmd.c_str());
    std::stringstream ss(result);
    std::vector<std::string> wifi;
    std::string input;
    while (std::getline(ss, input))
        if (input.find("All User Profile") != std::string::npos &&
            input.find(":") != std::string::npos)
            wifi.push_back(input.substr(input.find(":") + 2));
    return wifi;
}

std::vector<std::string> getPassword(std::string ssid) {
    std::string cmd = "netsh wlan show profile \"" + ssid + "\" key=clear";
    std::string result = exec(cmd.c_str());
    std::stringstream ss(result);
    std::vector<std::string> wifi;
    std::string input;
    while (std::getline(ss, input))
        if (input.find("Key Content") != std::string::npos &&
            input.find(":") != std::string::npos)
            wifi.push_back(input.substr(input.find(":") + 2));
    return wifi;
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::vector<std::string> wifiList = getWifiList();
    std::cout << "\033[1;36m" << std::left << std::setw(30) << "Wi-Fi Name" << "\033[0m  \033[1;34m|\033[0m  \033[1;36m" << "Password\n";
    std::cout << "\033[1;34m" << "═════════════════════════════════════════════\n" << "\033[0m";
    for (const auto& wifi : wifiList) {
        std::vector<std::string> passwords = getPassword(wifi);
        std::string password = passwords.empty() ? "" : passwords[0];
        std::cout << "\033[1;32m" << std::left << std::setw(30) << wifi << "\033[0m  \033[1;34m|\033[0m  \033[1;31m" << password << "\033[0m\n";
    }
    return 0;
}
