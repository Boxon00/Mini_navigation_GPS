# 🗺️ Mini GPS Navigacioni Sistem — SOLID + OOP verzija

Konzolni program u **C++17** koji pronalazi najkracu rutu izmedju gradova Srbije. Ova verzija je kompletno refaktorisana po **SOLID principima** i **OOP** paradigmi.

---

## Struktura foldera

```
mini-gps-solid/
│
├── main.cpp                    ← Composition Root (jedino mesto DI wiringa)
│
├── include/
│   ├── types.h                 ← City, Edge, RouteResult (podaci)
│   ├── graph.h                 ← RoadGraph klasa
│   ├── ipathfinder.h           ← IPathfinder apstraktni interfejs  ★
│   ├── dijkstra.h              ← DijkstraPathfinder : IPathfinder
│   ├── astar.h                 ← AStarPathfinder    : IPathfinder
│   ├── navigator.h             ← Navigator (orkestracija, DI)      ★
│   ├── map_builder.h           ← IMapBuilder + SerbiaMapBuilder    ★
│   ├── heuristic.h             ← haversineKm() heuristika
│   └── console_ui.h            ← ConsoleUI klasa
│
├── src/
│   ├── graph.cpp
│   ├── heuristic.cpp
│   ├── dijkstra.cpp
│   ├── astar.cpp
│   ├── navigator.cpp
│   ├── map_builder.cpp
│   └── console_ui.cpp
│
├── docs/
│   └── solid.md                ← detaljan opis svakog SOLID principa
│
├── Makefile
└── README.md
```

---

## Kako pokrenuti

```bash
# Kompajliranje
make release

# Pokretanje
./gps

# Ili u jednom koraku
make run

# Ciscenje
make clean
```

### Rucno (bez make)

```bash
g++ -std=c++17 -O2 \
    main.cpp \
    src/graph.cpp src/heuristic.cpp src/dijkstra.cpp \
    src/astar.cpp src/navigator.cpp src/map_builder.cpp \
    src/console_ui.cpp \
    -o gps
./gps
```

---

## SOLID principi u projektu

### S — Single Responsibility Principle
Svaka klasa ima tacno jednu odgovornost:

| Klasa / fajl         | Odgovornost                                 |
|----------------------|---------------------------------------------|
| `RoadGraph`          | Cuvanje i pristup podacima grafa            |
| `DijkstraPathfinder` | Dijkstra algoritam pretrage                 |
| `AStarPathfinder`    | A* algoritam pretrage                       |
| `Navigator`          | Orkestracija algoritama (pokretanje, uporedjivanje) |
| `SerbiaMapBuilder`   | Izgradnja konkretne mape Srbije             |
| `ConsoleUI`          | Sav unos i ispis na konzolu                 |
| `haversineKm()`      | Racunanje vazdusne udaljenosti              |

### O — Open/Closed Principle
Sistem je **otvoren za prosirenje, zatvoren za izmenu**.

Dodavanje novog algoritma (npr. Bellman-Ford) zahteva:
1. Novu klasu `BellmanFordPathfinder : public IPathfinder`
2. Jedan red u `main.cpp`: `navigator.registerAlgorithm(...)`
3. **Nista drugo se ne menja** — ni `Navigator`, ni `ConsoleUI`, ni `main.cpp` logika

### L — Liskov Substitution Principle
`DijkstraPathfinder` i `AStarPathfinder` mogu se koristiti bilo gde
gde se ocekuje `IPathfinder*` — bez ikakve promene ponasanja programa.

```cpp
// Ovo radi sa bilo kojom konkretnom implementacijom:
void printRoute(const IPathfinder& finder, const RoadGraph& g,
                const std::string& a, const std::string& b) {
    printResult(finder.findRoute(g, a, b));  // polimorfan poziv
}
```

### I — Interface Segregation Principle
`IPathfinder` je minimalan interfejs — samo ono sto klijent treba:

```cpp
class IPathfinder {
public:
    virtual RouteResult findRoute(...) const = 0;
    virtual std::string name()        const = 0;
    virtual ~IPathfinder()                  = default;
};
```

Klase ne moraju da implementiraju metode koje im ne trebaju.

### D — Dependency Inversion Principle
Visoki nivoi zavise od **apstrakcija**, ne od konkretnih klasa:

```
main.cpp  →  IPathfinder  ←  DijkstraPathfinder
              (apstrakcija)   AStarPathfinder
                              BellmanFordPathfinder  (novi)

main.cpp  →  IMapBuilder  ←  SerbiaMapBuilder
                              EuropeMapBuilder       (novi)
```

`Navigator` prima `unique_ptr<IPathfinder>` — ne zna koji je algoritam unutra.
`ConsoleUI` prima `Navigator&` i `RoadGraph&` kroz konstruktor (Dependency Injection).

---

## Kako dodati novi algoritam

```cpp
// 1. include/bellman_ford.h
class BellmanFordPathfinder : public IPathfinder {
public:
    RouteResult findRoute(const RoadGraph&,
                          const std::string& start,
                          const std::string& end) const override;
    std::string name() const override { return "Bellman-Ford"; }
};

// 2. src/bellman_ford.cpp
// ... implementacija ...

// 3. main.cpp — jedan red:
navigator.registerAlgorithm(std::make_unique<BellmanFordPathfinder>());
```

Meni se automatski azurira. Nista drugo se ne menja.

---

## Kako dodati novu mapu

```cpp
// 1. include/map_builder.h — dodaj klasu:
class EuropeMapBuilder : public IMapBuilder {
public:
    RoadGraph build() const override;
};

// 2. src/europe_map_builder.cpp — implementacija

// 3. main.cpp — promeni jedan red:
std::unique_ptr<IMapBuilder> builder =
    std::make_unique<EuropeMapBuilder>();  // ← umesto SerbiaMapBuilder
```

---

## Gradovi i putevi (Srbija)

12 gradova, 19 dvosmerna puta sa realnim udaljenostima u km:

```
Beograd  ↔  Novi Sad    (86)    Cacak     ↔  Kraljevo   (50)
Beograd  ↔  Pancevo     (14)    Cacak     ↔  Uzice      (70)
Beograd  ↔  Zrenjanin   (70)    Kraljevo  ↔  Nis        (130)
Beograd  ↔  Kragujevac (140)    Kraljevo  ↔  Uzice      (110)
Beograd  ↔  Nis        (240)    Nis       ↔  Leskovac   (45)
Novi Sad ↔  Subotica   (100)    Nis       ↔  Vranje     (125)
Novi Sad ↔  Zrenjanin   (55)    Leskovac  ↔  Vranje     (55)
Subotica ↔  Zrenjanin   (95)    Kragujevac↔  Nis        (135)
Pancevo  ↔  Zrenjanin   (60)    Kragujevac↔  Cacak      (80)
                                Kragujevac↔  Kraljevo   (90)
```
