#include "output.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <thread>
#include <chrono>

output::output(/* args */)
{
}

output::~output()
{
}

void output::drawCockpit(double t, double h, double v, double a, double hMax, double thrust, double targetThrust, double fuelConsumption, bool intact)
{
    const int barLength = 16;
    const int width = 34;       // Inner width for value lines
    const int labelWidth = 22;  // Width for labels
    const int valueWidth = 8;   // Width for numeric values

    // Clear screen (ANSI Escape Codes)
    std::cout << "\033[2J\033[H";

    // Title centered
    std::string title = "MOONLANDER";
    int padding = (width - title.size()) / 2;
    std::cout << "+" << std::string(width, '-') << "+\n";
    std::cout << "|" << std::string(padding, ' ') << title 
              << std::string(width - padding - title.size(), ' ') << "|\n";
    std::cout << "+" << std::string(width, '-') << "+\n";

    // Display values
    std::cout << "| " << std::left << std::setw(labelWidth) << "Time:" 
              << std::right << std::setw(valueWidth) << std::fixed << std::setprecision(2) << t << " s |\n";
    std::cout << "| " << std::left << std::setw(labelWidth) << "Altitude:" 
              << std::right << std::setw(valueWidth) << std::fixed << std::setprecision(2) << h << " m |\n";
    std::cout << "| " << std::left << std::setw(labelWidth) << "Velocity:" 
              << std::right << std::setw(valueWidth - 2) << std::fixed << std::setprecision(2) << v << " m/s |\n";
    std::cout << "| " << std::left << std::setw(labelWidth) << "Acceleration:" 
              << std::right << std::setw(valueWidth - 2) << std::fixed << std::setprecision(2) << a << " m/s^2 |\n";
    std::cout << "| " << std::left << std::setw(labelWidth) << "Thrust:" 
              << std::right << std::setw(valueWidth - 2) << std::fixed << std::setprecision(2) << thrust << " N |\n";
    std::cout << "| " << std::left << std::setw(labelWidth) << "T-Thrust:" 
              << std::right << std::setw(valueWidth - 2) << std::fixed << std::setprecision(2) << targetThrust << " N |\n";
    std::cout << "| " << std::left << std::setw(labelWidth) << "FuelConsumption:" 
              << std::right << std::setw(valueWidth - 2) << std::fixed << std::setprecision(2) << fuelConsumption << " kg/s |\n";

    // Altitude bar
    int hFilled = static_cast<int>(std::round((h / hMax) * barLength));
    hFilled = std::clamp(hFilled, 0, barLength);
    std::cout << "| Altitude bar: [";
    for (int i = 0; i < hFilled; ++i) std::cout << "#";
    for (int i = hFilled; i < barLength; ++i) std::cout << " ";
    std::cout << "] |\n";

    // Spacecraft status
    std::string status = intact ? "OPERATIONAL" : "\033[5mDAMAGED\033[0m"; // blinking if damaged
    int statusPadding = width - 1 - status.size(); // -2 for the spaces around label
    std::cout << "| Status: " << status 
              << std::string(statusPadding - 8, ' ') << "|\n"; // -8 for label "Status: "

    std::cout << "+" << std::string(width, '-') << "+\n";
}

void output::drawMissionFailed()
{
    std::cout << "\033[2J\033[H";
    std::cout << "\033[1;31m";

    std::string explosionFrames[] = {
        "      *       ",
        "     ***      ",
        "   *******    ",
        "  *********   ",
        "   *******    ",
        "     ***      ",
        "      *       "
    };

    for (const auto& frame : explosionFrames) {
        std::cout << "\033[2J\033[H"; // clear
        std::cout << "\n\n        " << frame << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }

    std::cout << "\033[1;31mMISSION FAILED - Spacecraft destroyed!\033[0m\n";

}