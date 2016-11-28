SYSCONF_LINK = gcc
CPPFLAGS     = `sdl-config --cflags` -std=c99 -Wall -Wextra -g 
LDFLAGS      =
LIBS         =  `sdl-config --libs` -lm #-lSDL_mixer

DESTDIR = ./
TARGET  = pacman

OBJECTS := $(addprefix $(DESTDIR),$(patsubst %.c,%.o,$(wildcard *.c)))

all: $(DESTDIR)$(TARGET)

$(DESTDIR)$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

$(OBJECTS): $(DESTDIR)%.o: %.c
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)




#sdlanim:	sdlanim.c
#	g++ -g sdlanim.c `sdl-config --cflags --libs`  -o sdlanim

#	g++ sdlanim.c -lSDL_mixer -lm `sdl-config --cflags --libs`  -o sdlanim 


#clean:
#	rm sdlanim
