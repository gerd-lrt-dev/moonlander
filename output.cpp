#include "output.h"

#include <iostream>
#include <iomanip>
#include <cmath>

output::output(/* args */)
{
}

output::~output()
{
}

// Private Funktion zum Anzeigen des Cockpits

void output::drawCockpit(double t, double h, double v, double hMax) 
{
    const int barLength = 20;

    // Bildschirm löschen (ANSI Escape Codes)
    std::cout << "\033[2J\033[H";

    // Feste Breite für den Rahmen
    const int width = 29; // Innenbreite für die Wertezeilen

    // Cockpit-Rahmen
    std::cout << "+-----------------------------+\n";
    std::cout << "|         MONDLANDER          |\n";
    std::cout << "+-----------------------------+\n";

    // Anzeigen: fester Platz für Labels + Werte
    std::cout << "| Zeit:            "
              << std::setw(8) << std::fixed << std::setprecision(2) << t << " s |\n";

    std::cout << "| Höhe:            "
              << std::setw(8) << std::fixed << std::setprecision(2) << h << " m |\n";

    std::cout << "| Geschwindigkeit: "
              << std::setw(8) << std::fixed << std::setprecision(2) << v << " m/s |\n";

    // Höhenbalken
    int filled = static_cast<int>(std::round((h / hMax) * barLength));
    if (filled < 0) filled = 0;
    if (filled > barLength) filled = barLength;

    std::cout << "| Höhe-Balken: [";
    for (int i = 0; i < filled; ++i) std::cout << "#";
    for (int i = filled; i < barLength; ++i) std::cout << " ";
    std::cout << "] |\n";

    std::cout << "+-----------------------------+\n";
}