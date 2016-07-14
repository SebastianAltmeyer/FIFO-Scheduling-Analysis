CC := g++
SRCDIR := src
BUILDDIR := objects
BUILDSUBDIR := $(shell cd $(SRCDIR); find -type d; cd ..)
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS :=  -Wall -g
INC := -I include

all: bin/rtAnalyser

bin/rtAnalyser: $(OBJECTS) $(BUILDDIR)/rtAnalyser.o
	@echo " Linking..."
	@echo " $(CC) $^ -o bin/rtAnalyser"; $(CC) $^ -o bin/rtAnalyser
	
$(BUILDDIR)/rtAnalyser.o: $(SRCDIR)/rtAnalyser.c
	$(CC) $(CFLAGS) $(SRCDIR)/rtAnalyser.c $(INC) -c -o $(BUILDDIR)/rtAnalyser.o
	
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@for p in $(BUILDSUBDIR); do mkdir -p $(BUILDDIR)/$$p; done
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)


.PHONY: clean