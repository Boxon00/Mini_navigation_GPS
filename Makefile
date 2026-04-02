# ============================================================
#  Mini GPS — SOLID verzija  |  Makefile
#
#  make          → debug build
#  make release  → optimizovani build
#  make run      → build + pokretanje
#  make clean    → brisanje build/ i izvrsnog fajla
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
TARGET   := gps

SRC := main.cpp \
       src/graph.cpp \
       src/heuristic.cpp \
       src/dijkstra.cpp \
       src/astar.cpp \
       src/navigator.cpp \
       src/map_builder.cpp \
       src/console_ui.cpp

OBJ := $(patsubst %.cpp, build/%.o, $(SRC))

.PHONY: all release run clean

all: CXXFLAGS += -g
all: $(TARGET)

release: CXXFLAGS += -O2 -DNDEBUG
release: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo ""
	@echo "  Kompajliranje uspesno!  →  ./$(TARGET)"
	@echo ""

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: all
	./$(TARGET)

clean:
	rm -rf build/ $(TARGET)
	@echo "  Ocisceno."
