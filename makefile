CC := g++
CFLAGS := -Wall -O2 -std=c++11
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin
TARGET := bin/main
TARGET_TESTER := bin/tester
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(TARGETDIR)
	@echo " $(CC) $(CFLAGS) $^ -o $(TARGET)"; $(CC) $(CFLAGS) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) -c $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGETDIR)

# Tests
tester: $(OBJECTS)
	@echo " Testing..."
	@mkdir -p $(TARGETDIR)
	@echo "$(CC) $(CFLAGS) test/tester.cpp $(INC) -o $(TARGET_TESTER)"; $(CC) $(CFLAGS) test/tester.cpp $(INC) -o $(TARGET_TESTER)

.PHONY: clean