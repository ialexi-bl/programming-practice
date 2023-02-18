# $(target) is supposed to be passed when running make
CC = g++
LIBS = -lm
CFLAGS = -Wall -g -std=c++2a -O0 -fsanitize=address

BINDIR=bin
SRCDIR=src
OBJDIR=$(BINDIR)/obj
LIBDIR=$(SRCDIR)/lib;./lib

TARGET=$(wildcard $(SRCDIR)/$(file)*.cpp)
EXECUTABLE_OBJ=$(TARGET:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXECUTABLE=$(BINDIR)/main

wildcard_each = $(wildcard $(foreach dir,$(subst ;, ,$(1)),$(subst %,$(dir),$(2))))

HEADERS = $(call wildcard_each,$(LIBDIR),%/*.hpp) $(call wildcard_each,$(LIBDIR),%/**/*.hpp)
SRCS = $(call wildcard_each,$(LIBDIR),%/*.cpp) $(call wildcard_each,$(LIBDIR),%/**/*.cpp)
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

.PHONY: run clean compile $(EXECUTABLE_OBJ)
.PRECIOUS: $(EXECUTABLE) $(OBJS)

abc:
	@echo "$(SRCS)"
	@echo "$(OBJS)"

run: compile
	@echo -n "$$ "
	$(EXECUTABLE) $(args)

compile: $(OBJS) $(EXECUTABLE_OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $(EXECUTABLE)

$(OBJS): $(OBJDIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS)

$(EXECUTABLE_OBJ): $(TARGET)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS)

clean:
	@echo "Cleaning..."
	@rm -rvf $(BINDIR)/*
	@echo "Cleaning done"