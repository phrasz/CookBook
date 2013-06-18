OBJS = $(addprefix obj/, $(addsuffix .o, $(basename $(wildcard *.cpp))))
HEADERS = $(wildcard *.h)

GPP = g++

#ALCONF = /usr/people/hein/allegro-4.0.3/bin/allegro-config
ALCONF = allegro-config

## COMPILER OPTIONS

# optimized build (unix)
CPPFLAGS = -O3 -Wall `$(ALCONF) --cflags`

# optimized build for DJGPP/MINGW
#CPPFLAGS = -O3 -Wall

# for profiling
#CPPFLAGS = -O3 -Wall -pg

# debug build
#CPPFLAGS = -g -Wall `$(ALCONF) --cflags`


## LIBRARIES

# optimized build (unix)
LIBS = `$(ALCONF) --libs release`

# debug build (unix)
#LIBS = `$(ALCONF) --libs debug`

# for DJGPP/MINGW
#LIBS = -lalleg


all: othello # TAGS

othello: $(OBJS)
	$(GPP) $(CPPFLAGS) -o $@ $(OBJS) $(LIBS)

obj/%.o: %.cpp $(HEADERS)
	$(GPP) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) othello core.*

TAGS:	*.cpp *.h
	etags --language=c++ *.cpp *.h
