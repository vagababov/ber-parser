INTERNAL_DIR = ./internal

CXXFLAGS += -Wall -Wextra

LIBS = -lgflags -lglog
LIBRARIES = buffer.o window.o
TESTS = window-test
BIN_DIR = ../obj

# GTest related definitions.
GTEST_DIR = ../gtest
GTEST_INCLUDES = $(GTEST_DIR)/include
GTEST_HEADERS = $(GTEST_DIR)/include/*.h $(GTEST_DIR)/include/internal/*.h
GTEST_SOURCES = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h

all : pre $(LIBRARIES)
all-tests: pre $(TESTS)

pre :
	mkdir -p $(BIN_DIR)

clean :
	rm -rf $(BIN_DIR)/*

buffer.o : $(INTERNAL_DIR)/buffer.cc
	$(CXX) $(CXXFLAGS) -c $(INTERNAL_DIR)/buffer.cc -o $(BIN_DIR)/$@

window.o : buffer.o $(INTERNAL_DIR)/window.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(INTERNAL_DIR)/window.cc -o $(BIN_DIR)/$@

# Build gtest libraries
gtest-all.o: $(GTEST_SOURCES)
	$(CXX) -I$(GTEST_INCLUDES) -I$(GTEST_DIR) $(CPPFLAGS) $(CXXFLAGS)\
			-c $(GTEST_DIR)/src/gtest-all.cc -o $(BIN_DIR)/$@

gtest-all.a: gtest-all.o
	$(AR) $(ARFLAGS) $(BIN_DIR)/$@ $(BIN_DIR)/$^

gtest-main.o: $(GTEST_SOURCES)
	$(CXX) -I$(GTEST_INCLUDES) $(CXXFLAGS) $(CPPFLAGS)\
			-c $(GTEST_DIR)/src/gtest_main.cc -o $(BIN_DIR)/$@

gtest-main.a: gtest-main.o
	$(AR) $(ARFLAGS) $(BIN_DIR)/$@ $^


window-test : window.o buffer.o gtest-all.a 
	$(CXX) -I$(GTEST_INCLUDES) $(CXXFLAGS) $(CPPFLAGS)\
			$(INTERNAL_DIR)/window-test.cc $(addprefix $(BIN_DIR)/, $^) $(LIBS) -lpthread\
      -o $(BIN_DIR)/$@
