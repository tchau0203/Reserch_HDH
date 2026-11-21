CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

TARGET = main.exe
HOANG_DEMO = hoang_demo.exe

all: $(TARGET) $(HOANG_DEMO)

$(TARGET): src/main.cpp src/page_table.cpp src/address_translation.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

$(HOANG_DEMO): examples/hoang_page_table_demo.cpp src/page_table.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGET) $(HOANG_DEMO) run.exe check.exe *.o *.obj
