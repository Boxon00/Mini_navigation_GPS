# SOLID principi — detaljna dokumentacija

Ovaj dokument objasnjava kako je svaki SOLID princip primenjen
u projektu, sa konkretnim primerima iz koda.

---

## S — Single Responsibility Principle (SRP)

> "Klasa treba da ima samo jedan razlog za promenu."
> — Robert C. Martin

### Pre refaktorisanja (narušen SRP)

U originalnoj verziji, `navigation_system.cpp` je radio sve:
- cuvao strukturu grafa
- implementirao Dijkstra i A*
- ispisivao rezultate na konzolu
- ucitavao podatke o gradovima

Svaka promena (novi UI, novi algoritam, novi podaci) dirala je
isti fajl.

### Posle refaktorisanja (ispunjen SRP)

Svaka klasa ima tacno jedan razlog za promenu:

```
RoadGraph          → menja se ako se menja struktura grafa
DijkstraPathfinder → menja se ako se menja Dijkstra algoritam
AStarPathfinder    → menja se ako se menja A* algoritam
Navigator          → menja se ako se menja logika orkestracije
SerbiaMapBuilder   → menja se ako se menjaju podaci o Srbiji
ConsoleUI          → menja se ako se menja izgled konzole
haversineKm()      → menja se ako se menja formula heuristike
```

---

## O — Open/Closed Principle (OCP)

> "Softverski entiteti treba da budu otvoreni za prosirenje,
>  ali zatvoreni za modifikaciju."

### Kako je primenjen

`IPathfinder` interfejs je tacka prosirenja sistema.
Da bi se dodao novi algoritam, sistem se **prosiruje** (nova klasa),
ali se ne **modifikuje** (stari kod ostaje nepromenjen).

```cpp
// Bellman-Ford — nova klasa, nista staro se ne menja:
class BellmanFordPathfinder : public IPathfinder {
public:
    RouteResult findRoute(const RoadGraph& g,
                          const std::string& s,
                          const std::string& e) const override {
        // ... implementacija ...
    }
    std::string name() const override { return "Bellman-Ford"; }
};
```

```cpp
// main.cpp — samo jedan novi red:
navigator.registerAlgorithm(std::make_unique<BellmanFordPathfinder>());
```

Meni u `ConsoleUI` se automatski azurira jer je dinamicki generisan
na osnovu `navigator_.algorithmNames()` — ne postoje hardkodovani
stringovi "Dijkstra" ili "A*" u UI kodu.

Isto vazi za mape: `IMapBuilder` omogucava dodavanje nove mape
(npr. Evropa) bez menjanja `Navigator` ili `ConsoleUI`.

---

## L — Liskov Substitution Principle (LSP)

> "Objekti podklase moraju moci da zamene objekte bazne klase
>  bez narusavanja ispravnosti programa."

### Kako je primenjen

`DijkstraPathfinder` i `AStarPathfinder` nasledjuju `IPathfinder`.
Obe klase postuju ugovor koji `IPathfinder` propisuje:
- `findRoute()` uvek vraca `RouteResult`
- `found = true` samo ako put postoji
- `path` je uvek validna sekvenca od starta do cilja
- `name()` uvek vraca neprazan string

`Navigator::run()` koristi `IPathfinder*` polimorfno i ne zna
koji je algoritam unutra — a program se ponasa ispravno u oba slucaja:

```cpp
// Navigator.cpp — polimorfan poziv, LSP garantuje ispravnost:
return algorithms_[algoIndex]->findRoute(graph, start, end);
```

### Sta bi narusilo LSP

```cpp
// LOSE — podklasa menja ocekivano ponasanje:
class BadPathfinder : public IPathfinder {
    RouteResult findRoute(...) const override {
        // Vraca found=true cak i kad puta nema — narušava ugovor!
        return {true, {}, 0, 0, name()};
    }
};
```

---

## I — Interface Segregation Principle (ISP)

> "Klijenti ne smeju da zavise od metoda koje ne koriste."

### Kako je primenjen

`IPathfinder` je namerno minimalan — samo dve metode:

```cpp
class IPathfinder {
public:
    virtual RouteResult findRoute(const RoadGraph&,
                                  const std::string& start,
                                  const std::string& end) const = 0;
    virtual std::string name() const = 0;
    virtual ~IPathfinder() = default;
};
```

Konkretne implementacije nisu primorane da implementiraju metode
koje im ne trebaju (npr. `printDebugInfo()`, `getStepByStep()`...).

### Sta bi narusilo ISP

```cpp
// LOSE — "debeli" interfejs koji forsira nepotrebne metode:
class IPathfinder {
    virtual RouteResult findRoute(...)       = 0;
    virtual std::string name()               = 0;
    virtual void        printDebugLog()      = 0;  // nije svima potrebno
    virtual int         getMemoryUsage()     = 0;  // nije svima potrebno
    virtual void        exportToJSON()       = 0;  // nije svima potrebno
};
```

Svaka nova konkretna klasa bila bi primorana da implementira sve,
cak i prazne stubove.

---

## D — Dependency Inversion Principle (DIP)

> "Visoki nivoi ne smeju zavisiti od niskih nivoa.
>  Oba treba da zavise od apstrakcija."

### Kako je primenjen

```
VISOKI NIVO          APSTRAKCIJA          NISKI NIVO
───────────────────────────────────────────────────────
main.cpp         →  IMapBuilder      ←  SerbiaMapBuilder
Navigator        →  IPathfinder      ←  DijkstraPathfinder
Navigator        →  IPathfinder      ←  AStarPathfinder
ConsoleUI        →  Navigator&           (prima kroz DI)
ConsoleUI        →  RoadGraph&           (prima kroz DI)
```

`Navigator` nikada ne pise `new DijkstraPathfinder()` sam —
algoritmi su mu **ubrizgani** kroz `registerAlgorithm()`.

`ConsoleUI` nikada ne pise `Navigator nav;` sam —
prima ga kroz konstruktor:

```cpp
// Dependency Injection kroz konstruktor:
ConsoleUI::ConsoleUI(const Navigator& navigator, const RoadGraph& graph)
    : navigator_(navigator), graph_(graph)
{}
```

### main.cpp kao Composition Root

`main.cpp` je jedino mesto gde se kreiraju konkretni objekti.
Sav ostatak koda komunicira kroz interfejse i reference:

```cpp
int main() {
    // Jedino mesto sa "new" / make_unique — Composition Root
    std::unique_ptr<IMapBuilder> builder =
        std::make_unique<SerbiaMapBuilder>();     // konkretno
    RoadGraph graph = builder->build();

    Navigator navigator;
    navigator.registerAlgorithm(
        std::make_unique<DijkstraPathfinder>()); // konkretno
    navigator.registerAlgorithm(
        std::make_unique<AStarPathfinder>());    // konkretno

    ConsoleUI ui(navigator, graph);              // DI
    ui.run();
}
```

---

## OOP koncepti u projektu

### Enkapsulacija
`RoadGraph` cuva `cities_` i `adjList_` kao privatne clanove.
Spoljni kod ne moze direktno da menja strukturu grafa — mora da
koristi `addCity()` i `addRoad()`.

### Apstrakcija
`IPathfinder` i `IMapBuilder` su apstraktne klase (interfejsi).
Klijentski kod zna samo STA treba da se uradi (findRoute, build),
ne i KAKO.

### Nasledjivanje
```
IPathfinder  (apstraktna bazna klasa)
├── DijkstraPathfinder
└── AStarPathfinder

IMapBuilder  (apstraktna bazna klasa)
└── SerbiaMapBuilder
```

### Polimorfizam
`Navigator` poziva `algorithms_[i]->findRoute(...)` — u vreme
izvrsavanja (runtime), C++ automatski bira pravu implementaciju
(Dijkstra ili A*) na osnovu stvarnog tipa objekta.

```cpp
// Isti poziv — razlicito ponasanje zavisno od konkretnog tipa:
for (const auto& algo : algorithms_)
    results.push_back(algo->findRoute(graph, start, end));
```

### RAII i pametni pokazivaci
`unique_ptr<IPathfinder>` u `Navigator` garantuje da se objekti
automatski brisu kada `Navigator` izadje iz opsega — nema
rucnog `delete`, nema curenja memorije.
