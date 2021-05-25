# $(target) is supposed to be passed when running make
CC = gcc
LIBS = -lm
CFLAGS = -Wall -g

BINDIR=bin
SRCDIR=src
OBJDIR=$(BINDIR)/obj
LIBDIR=$(SRCDIR)/lib

TARGET=$(wildcard $(SRCDIR)/$(file)*.c)
EXECUTABLE_OBJ=$(TARGET:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXECUTABLE=$(BINDIR)/$(notdir $(TARGET:%.c=%))

HEADERS = $(wildcard $(LIBDIR)/*.h) $(wildcard $(LIBDIR)/**/*.h)
SRCS = $(wildcard $(LIBDIR)/*.c) $(wildcard $(LIBDIR)/**/*.c)
OBJS = $(patsubst $(LIBDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY: run clean compile $(EXECUTABLE_OBJ)
.PRECIOUS: $(EXECUTABLE) $(OBJS)

run: compile
	@echo -n "$$ "
	$(EXECUTABLE)

compile: $(OBJS) $(EXECUTABLE_OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $(EXECUTABLE)

$(OBJS): $(OBJDIR)/%.o: $(LIBDIR)/%.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS)

$(EXECUTABLE_OBJ): $(TARGET)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS)

clean:
	@echo "Cleaning..."
	@rm -rvf $(BINDIR)/*
	@echo "Cleaning done"