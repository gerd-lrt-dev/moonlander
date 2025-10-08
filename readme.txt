# 🌓 Moonlander – C++ Mondlandungssimulation

**Moonlander** ist ein Lern- und Spaßprojekt in C++, inspiriert vom Beispiel *„Die Mondlandung“* aus dem Buch **Coding for Fun**.  
Ziel ist es, die klassische Mondlandungssimulation nachzuprogrammieren und schrittweise um moderne Elemente zu erweitern – bis hin zu einer visuellen Darstellung mit **Qt**.

---

## 🚀 Projektbeschreibung

Die aktuelle Version simuliert die Mondlandung physikalisch vereinfacht mit:

- **Gravitation**, Geschwindigkeit und Höhe  
- **Steuerbarem Schub** (Thrust)  
- **Echtzeit-Simulation** über `std::chrono`  
- **ASCII-basiertem Cockpit**, das Telemetrie-Daten anzeigt  

Das Projekt ist vollständig objektorientiert aufgebaut und nutzt moderne C++-Features wie **Smart Pointers** und **Kapselung**.

---

## 🧩 Klassenstruktur

| Klasse        | Aufgabe |
|----------------|----------|
| `Physics`      | Berechnung von Geschwindigkeit und Höhe unter Einfluss der Gravitation |
| `Thrust`       | Modelliert Triebwerksschub mit Reaktionszeit und Zielwert |
| `Output`       | ASCII-Visualisierung des Cockpits |
| `SimControl`   | Hauptsteuerung der Simulation (Loop, Timing, Parameterprüfung) |

---

## 🧠 Zielsetzung

Das Projekt dient dazu, **C++-Grundlagen und moderne Programmiertechniken** an einem anschaulichen Beispiel zu trainieren:

- Physikalisches Denken → Simulation einfacher Bewegungsgleichungen  
- Objektorientiertes Design → klare Trennung von Logik und Darstellung  
- Erweiterbarkeit → Vorbereitung auf grafische Darstellung mit **Qt**  

---

## 🧰 Geplante Erweiterungen

- Qt-basierte Visualisierung (2D / 3D) der Mondlandung  
- Steuerung über Tastatur oder GUI-Schieberegler  
- Partikelsysteme und Animationen  
- Erweiterte Physik (Treibstoffverbrauch, Trägheit, Autopilot)  
- Speichern/Laden von Flügen und Statistiken  

---

## ⚙️ Build & Run

### Voraussetzungen
- **C++20** oder neuer  
- G++ oder Clang  
- Optional: **Qt6** für spätere Visualisierung  

### Kompilieren (Kommandozeile)
```bash
g++ -std=c++20 -Wall -Wextra -O2 main.cpp physics.cpp output.cpp simcontrol.cpp Thrust.cpp -o moonlander

