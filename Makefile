CXX ?= g++
BUILD_DIR ?= build
TARGET := $(BUILD_DIR)/restaurant_app
QT_PACKAGE := Qt6Widgets

CPP_SOURCES := $(shell find src -type f -name '*.cpp' -print)
OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/obj/%.o,$(CPP_SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)
CORE_SOURCES := $(shell find src/Models src/Repositories src/Services -type f -name '*.cpp' -print)
TEST_TARGET := $(BUILD_DIR)/restaurant_tests

CPPFLAGS += -Isrc $(shell pkg-config --cflags $(QT_PACKAGE) 2>/dev/null)
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
CXXFLAGS += -MMD -MP
LDFLAGS ?=
LDLIBS += $(shell pkg-config --libs $(QT_PACKAGE) 2>/dev/null)

.PHONY: all run test clean check-qt

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

check-qt:
	@command -v pkg-config >/dev/null 2>&1 || { echo "Error: pkg-config is required to build the Qt Widgets GUI." >&2; exit 1; }
	@pkg-config --exists $(QT_PACKAGE) || { echo "Error: Qt 6 Widgets development files are missing. Install qt6-base-dev and pkg-config." >&2; exit 1; }

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/obj/%.o: src/%.cpp | check-qt
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(TEST_TARGET): tests/core_tests.cpp $(CORE_SOURCES)
	mkdir -p $(dir $@)
	$(CXX) -Isrc $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPENDENCIES)
