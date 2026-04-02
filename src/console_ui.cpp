#include "../include/console_ui.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>

// ------------------------------------------------------------
//  Konstruktor — prima zavisnosti kroz reference (DI)
// ------------------------------------------------------------
ConsoleUI::ConsoleUI(const Navigator& navigator, const RoadGraph& graph)
    : navigator_(navigator), graph_(graph)
{}

// ------------------------------------------------------------
//  Glavna petlja
// ------------------------------------------------------------
void ConsoleUI::run() {
    int choice = 0;
    while (true) {
        printMenu();
        std::cin >> choice;
        std::cin.ignore();

        const int count = navigator_.algorithmCount();

        // --- Izlaz ---
        if (choice == count + 3) {
            std::cout << "\n  Dovidjenja!\n\n";
            break;
        }

        // --- Prikaz grafa ---
        if (choice == count + 2) {
            printGraph();
            continue;
        }

        // --- Poredjenje svih algoritama ---
        if (choice == count + 1) {
            std::string start, end;
            if (!readCities(start, end)) continue;
            auto results = navigator_.runAll(graph_, start, end);
            printComparison(results);
            continue;
        }

        // --- Pojedinacni algoritam (1-based indeks) ---
        if (choice >= 1 && choice <= count) {
            std::string start, end;
            if (!readCities(start, end)) continue;
            printResult(navigator_.run(choice - 1, graph_, start, end));
            continue;
        }

        std::cout << "  Neispravan izbor. Pokusaj ponovo.\n";
    }
}

// ------------------------------------------------------------
//  Privatne metode
// ------------------------------------------------------------
void ConsoleUI::printSeparator() const {
    std::cout << "  " << std::string(58, '-') << "\n";
}

void ConsoleUI::printMenu() const {
    auto names = navigator_.algorithmNames();
    int  count = navigator_.algorithmCount();

    std::cout << "\n";
    std::cout << "  ╔══════════════════════════════════════╗\n";
    std::cout << "  ║     MINI GPS NAVIGACIONI SISTEM      ║\n";
    std::cout << "  ╚══════════════════════════════════════╝\n";

    // Dinamicki generisan meni — novi algoritam = novi red automatski
    for (int i = 0; i < count; ++i)
        std::cout << "  " << (i + 1) << ". Pronadji rutu  ("
                  << names[i] << ")\n";

    std::cout << "  " << (count + 1) << ". Uporedi sve algoritme\n";
    std::cout << "  " << (count + 2) << ". Prikazi gradove i puteve\n";
    std::cout << "  " << (count + 3) << ". Izlaz\n";
    std::cout << "  Izbor: ";
}

bool ConsoleUI::readCities(std::string& start, std::string& end) const {
    std::cout << "  Polazni grad : ";
    std::getline(std::cin, start);
    std::cout << "  Cilj         : ";
    std::getline(std::cin, end);

    if (!graph_.hasCity(start) || !graph_.hasCity(end)) {
        std::cout << "  Greska: grad nije pronadjen. Proveri pravopis.\n";
        return false;
    }
    if (start == end) {
        std::cout << "  Vec si na odredisnom gradu!\n";
        return false;
    }
    return true;
}

void ConsoleUI::printResult(const RouteResult& res) const {
    printSeparator();
    if (!res.found) {
        std::cout << "  NEMA PUTA izmedju zadatih gradova!\n";
        printSeparator();
        return;
    }

    std::cout << "  Algoritam    : " << res.algorithmName << "\n";
    std::cout << "  Ruta         : ";
    for (std::size_t i = 0; i < res.path.size(); ++i) {
        std::cout << res.path[i];
        if (i + 1 < res.path.size()) std::cout << " --> ";
    }
    std::cout << "\n";
    std::cout << "  Ukupno km    : " << res.totalDistance << " km\n";
    std::cout << "  Br. stanica  : " << res.path.size() - 1 << "\n";
    std::cout << "  Preg. cvorova: " << res.nodesVisited << "\n";

    int h = res.totalDistance / 80;
    int m = (res.totalDistance % 80) * 60 / 80;
    std::cout << "  Vreme voznje : ~" << h << " h " << m
              << " min  (avg 80 km/h)\n";
    printSeparator();
}

void ConsoleUI::printComparison(
        const std::vector<RouteResult>& results) const
{
    // Ispisi svaki rezultat
    for (const auto& r : results) {
        std::cout << "\n  >>> " << r.algorithmName << " <<<\n";
        printResult(r);
    }

    // Uporedna tabela efikasnosti
    std::cout << "\n  >>> POREDJENJE EFIKASNOSTI <<<\n";
    printSeparator();
    std::cout << "  " << std::left
              << std::setw(16) << "Algoritam"
              << std::setw(10) << "Km"
              << std::setw(18) << "Preg. cvorova"
              << "Efikasnost\n";
    printSeparator();

    int maxVisited = 0;
    for (const auto& r : results)
        maxVisited = std::max(maxVisited, r.nodesVisited);

    for (const auto& r : results) {
        int efikasnost = maxVisited > 0
            ? 100 - r.nodesVisited * 100 / maxVisited
            : 0;
        std::cout << "  " << std::left
                  << std::setw(16) << r.algorithmName
                  << std::setw(10) << (r.found
                                        ? std::to_string(r.totalDistance) + " km"
                                        : "N/A")
                  << std::setw(18) << r.nodesVisited
                  << "+" << efikasnost << "% stednje\n";
    }
    printSeparator();
}

void ConsoleUI::printGraph() const {
    auto cities = graph_.allCities();
    std::sort(cities.begin(), cities.end());

    std::cout << "\n  -- Gradovi (" << cities.size() << ") --\n";
    for (const auto& c : cities)
        std::cout << "    " << c << "\n";

    std::cout << "\n  -- Putevi --\n";
    for (const auto& c : cities)
        for (const auto& e : graph_.neighbors(c))
            if (c < e.to)
                std::cout << "    "
                          << std::setw(14) << std::left << c
                          << " <--> "
                          << std::setw(14) << std::left << e.to
                          << e.distance << " km\n";
}
