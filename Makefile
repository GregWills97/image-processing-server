#Tool definitions
CC=gcc
CXX=g++

# paths
SRCDIR = ./src
BUILD = ./build
INCDIR = ./include
DOWNLOADS = ./downloads


#includes, libs, and flags
INCS = -I/usr/include/opencv4 -I$(INCDIR)

LIBS = -lopencv_core \
       -lopencv_imgproc \
       -lopencv_highgui \
       -lopencv_imgcodecs \
       -lpthread

CFLAGS = $(INCS)
CXXFLAGS = $(CFLAGS) --std=c++11
LDFLAGS = $(LIBS)

# Common source and object files
COMMON_SOURCES = $(shell find $(SRCDIR) -maxdepth 1 -type f -name *.c)
COMMON_OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILD)/%.o,$(COMMON_SOURCES))

# Server source and object files
SERVER_SOURCES = $(shell find $(SRCDIR)/server -maxdepth 1 -type f -name *.c)
SERVER_SOURCES++ = $(shell find $(SRCDIR)/server -maxdepth 1 -type f -name *.cpp)
SERVER_OBJS = $(patsubst $(SRCDIR)/server/%.c,$(BUILD)/server/%.o,$(SERVER_SOURCES))
SERVER_OBJS++ = $(patsubst $(SRCDIR)/server/%.cpp,$(BUILD)/server/%.o,$(SERVER_SOURCES++))
SERVER_OBJECTS = $(SERVER_OBJS) $(SERVER_OBJS++) $(COMMON_OBJECTS)

# Server source and object files
CLIENT_SOURCES = $(shell find $(SRCDIR)/client -maxdepth 1 -type f -name *.c)
CLIENT_OBJS = $(patsubst $(SRCDIR)/client/%.c,$(BUILD)/client/%.o,$(CLIENT_SOURCES))
CLIENT_OBJECTS = $(CLIENT_OBJS) $(COMMON_OBJECTS)

# Targets 

SERVER = ips-server
CLIENT = ips-client

all : directories $(SERVER) $(CLIENT)

directories: 
	@mkdir -p $(BUILD)/server
	@mkdir -p $(BUILD)/client
	@mkdir -p $(DOWNLOADS)

$(SERVER) : $(SERVER_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(CLIENT) : $(CLIENT_OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(BUILD)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f $(SERVER) $(CLIENT)
	rm -rf $(OUTDIR) $(BUILD) $(DOWNLOADS)

.PHONY : clean all
