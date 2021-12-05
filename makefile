# $(target) is supposed to be passed when running make
CC = g++
LIBS = -lm
CFLAGS = -Wall -g -std=c++20

BINDIR=bin
SRCDIR=src
OBJDIR=$(BINDIR)/obj
LIBDIR=$(SRCDIR)/lib

TARGET=$(wildcard $(SRCDIR)/$(file)*.cpp)
EXECUTABLE_OBJ=$(TARGET:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXECUTABLE=$(BINDIR)/main

HEADERS = $(wildcard $(LIBDIR)/*.hpp) $(wildcard $(LIBDIR)/**/*.hpp)
SRCS = $(wildcard $(LIBDIR)/*.cpp) $(wildcard $(LIBDIR)/**/*.cpp)
OBJS = $(patsubst $(LIBDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

.PHONY: run clean compile $(EXECUTABLE_OBJ)
.PRECIOUS: $(EXECUTABLE) $(OBJS)

run: compile
	@echo -n "$$ "
	$(EXECUTABLE)

compile: $(OBJS) $(EXECUTABLE_OBJ)
	echo $(OBJS)
	echo $(EXECUTABLE_OBJ)
	echo $(file)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $(EXECUTABLE)

$(OBJS): $(OBJDIR)/%.o: $(LIBDIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS)

$(EXECUTABLE_OBJ): $(TARGET)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS)

clean:
	@echo "Cleaning..."
	@rm -rvf $(BINDIR)/*
	@echo "Cleaning done"