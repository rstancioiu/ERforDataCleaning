CC := g++
CFLAGS := -Wall -O2 -std=c++11
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin
TARGET := bin/main
TARGET_TESTER := bin/tester
SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC := -I include



$(TARGET): $(OBJECTS) $(BUILDDIR)/main.o
	@echo "$(SOURCES)"
	@echo "$(OBJECTS)"
	@echo " Linking..."
	@mkdir -p $(TARGETDIR)
	@echo " $(CC) $(CFLAGS) $^ -o $(TARGET)"; $(CC) $(CFLAGS) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) 
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) -c $(INC) -c -o $@ $<


$(BUILDDIR)/main.o: main.cpp $(wildcard include/*.h)
	@echo "$(wildcard include/*.h)"
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) $< -c -o $@


clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGETDIR)

$(BUILDDIR)/tester.o: test/tester.cpp $(wildcard include/*.h)
	@mkdir -p $(BUILDDIR)	
	$(CC) $(CFLAGS) $(INC) $< -c -o $@
	
# Tests
tester: $(OBJECTS) $(BUILDDIR)/tester.o
	@echo " Testing..."
	@mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) $^ -o $(TARGET_TESTER)

.PHONY: clean