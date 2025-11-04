CC = gcc
EXE = SDL3_template 
SOURCES = main.c
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CFLAGS += -g -Wall -Wformat -std=c23

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux)
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `pkg-config --libs sdl3` -lSDL3_image -lSDL3_ttf
endif

ifeq ($(shell uname), Darwin)
	ECHO_MESSAGE = "Darwin"
	LIBS += `pkg-config --libs sdl3` -lSDL3_image -lSDL3_ttf
endif

ifeq ($(findstring MINGW64,$(UNAME_S)),MINGW64)
	LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --libs sdl3` -lSDL3_image -lSDL3_ttf
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(EXE)
	@echo build complete

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)