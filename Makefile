CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

TARGET = main.exe
HOANG_DEMO = hoang_demo.exe
RUN_FILES = run.exe
CHECK_IND = check.exe

all: $(TARGET) $(HOANG_DEMO) $(RUN_FILES) $(CHECK_IND)

$(TARGET): src/main.cpp src/page_table.cpp src/address_translation.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

$(HOANG_DEMO): examples/hoang_page_table_demo.cpp src/page_table.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

$(RUN_FILES): examples/run_from_files.cpp src/page_table.cpp src/address_translation.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CHECK_IND): examples/check_independent.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TARGET)
	./$(TARGET) --test

clean:
	rm -f $(TARGET) $(HOANG_DEMO) $(RUN_FILES) $(CHECK_IND) *.o *.obj
