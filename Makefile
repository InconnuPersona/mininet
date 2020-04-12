
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
Dir_Source = $(Dir_Root)/src

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
	netsend \
	send \
	sound \
	tile \
	unit \
	unitsbind \
	unituse \
	view
Debug_Objects = $(patsubst %, $(Dir_Debug)/%.o, $(Sources))
Release_Objects = $(patsubst %, $(Dir_Release)/%.o, $(Sources))

Target = mininet.exe

# Output targets

$(Dir_Debug)/$.o: $(Dir_Source)/%.c
	$(CC) -O0 -g3 -Wall -I"$(Dir_Source)" -o $@ -c $<

$(Dir_Debug)/$(Target): $(Debug_Objects)
	$(CC) -o $@ $<

$(Dir_Release)/$.o: $(Dir_Source)/%.c
	$(CC) -O3 -Wall -I"$(Dir_Source)" -o $@ -c $<

$(Dir_Release)/$(Target): $(Release_Objects)
	$(CC) -o $@ $<

Debug: $(Dir_Debug)/$(Target)
	$(ECHO) 'Finished compilation.'

Release: $(Dir_Release)/$(Target)
	$(ECHO) 'Finished compilation.'

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
