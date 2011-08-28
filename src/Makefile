INTERNAL_DIR = ./internal

CXXFLAGS += -Wall -Wextra


LIBS = -lgflags -lglog
LIBRARIES = buffer.o window.o
TESTS = window.test

# GTest related definitions.
GTEST_DIR = ../gtest
GTEST_INCLUDES = $(GTEST_DIR)/include
GTEST_HEADERS = $(GTEST_DIR)/include/*.h $(GTEST_DIR)/include/internal/*.h
GTEST_SOURCES = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h

all : $(LIBRARIES)
all-tests: $(TESTS)

clean :
	rm -rf $(LIBRARIES) $(TESTS) gtest* *.o *.a

buffer.o : $(INTERNAL_DIR)/buffer.cc
	$(CXX) $(CXXFLAGS) -c $(INTERNAL_DIR)/buffer.cc

window.o : $(INTERNAL_DIR)/window.cc buffer.o $(INTERNAL_DIR)/window.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(INTERNAL_DIR)/window.cc

# Build gtest libraries
gtest-all.o: $(GTEST_SOURCES)
	$(CXX) -I$(GTEST_INCLUDES) -I$(GTEST_DIR) $(CPPFLAGS) $(CXXFLAGS)\
			-c $(GTEST_DIR)/src/gtest-all.cc

gtest-all.a: gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest-main.o: $(GTEST_SOURCES)
	$(CXX) -I$(GTEST_INCLUDES) $(CXXFLAGS) $(CPPFLAGS)\
			-c $(GTEST_DIR)/src/gtest_main.cc -o gtest-main.o

gtest-main.a: gtest-main.o
	$(AR) $(ARFLAGS) $@ $^


window.test : window.o buffer.o gtest-all.a $(INTERNAL_DIR)/window-test.cc
	$(CXX) -I$(GTEST_INCLUDES) $(CXXFLAGS) $(CPPFLAGS)\
			$^ $(LIBS) -lpthread -o $@
