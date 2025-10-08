#include "output.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

output::output(/* args */)
{
}

output::~output()
{
}

void output::drawCockpit(double t, double h, double v, double hMax, bool intact)
{
    const int barLength = 20;
    const int width = 31;       // Inner width for value lines
    const int labelWidth = 15;  // Width for labels
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
              << std::right << std::setw(valueWidth) << std::fixed << std::setprecision(2) << v << " m/s |\n";

    // Altitude bar
    int hFilled = static_cast<int>(std::round((h / hMax) * barLength));
    hFilled = std::clamp(hFilled, 0, barLength);
    std::cout << "| Altitude bar: [";
    for (int i = 0; i < hFilled; ++i) std::cout << "#";
    for (int i = hFilled; i < barLength; ++i) std::cout << " ";
    std::cout << "] |\n";

    // Spacecraft status
    std::string status = intact ? "OPERATIONAL" : "\033[5mDAMAGED\033[0m"; // blinking if damaged
    int statusPadding = width - 2 - status.size(); // -2 for the spaces around label
    std::cout << "| Status: " << status 
              << std::string(statusPadding - 8, ' ') << "|\n"; // -8 for label "Status: "

    std::cout << "+" << std::string(width, '-') << "+\n";
}