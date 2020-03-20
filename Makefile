
# Commands

AS = as
CC = cc
CONCATENATE = cat
COPY = cp
ECHO = @echo
EXPORT = export
FIND = find
LD = ld
LINE = @echo ""
MAKEDIR = mkdir
REMOVE = rm

# Directories

Dir_Root = .

Dir_Debug = $(Dir_Root)/Debug
Dir_Release = $(Dir_Root)/Release
Dir_Source = $(Dir_Output)

Directories = $(Dir_Debug) $(Dir_Release) $(Dir_Source)

# Definitions

Sources = \
	bind \
	entry \
	game \
	gamesbind \
	gfx \
	host \
	item \
	itemsbind \
	key \
	level \
	levelaid \
	levelgen \
	levelsbind \
	main \
	menu \
	net \
	nethost \
	send \
	sound \
	tile \
	unit \
	unitsbind \
	unituse \
	view

Objects = 
Targets = Debug Release

# Output targets

Debug: 
	

Release: 

# Build targets

all: clean dirs build

build: $(Targets)
	$(ECHO) "Build completed."
	$(LINE)

clean:
	$(ECHO) "Cleaning output directories:"
	$(REMOVE) -fr $(Dir_Debug) $(Dir_Release)
	$(LINE)

dirs:
	$(ECHO) "Creating the directory structure:"
	$(MAKEDIR) -p $(Directories)
	$(LINE)

.PHONY: all build clean dirs
