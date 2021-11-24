#Tool definitions
CC=gcc
CXX=g++

# paths
SRCDIR = ./src
BUILD = ./build
OUTDIR = ./output


#includes, libs, and flags
INCS = -I/usr/include/opencv4 -I./include

LIBS = -lopencv_core \
       -lopencv_imgproc \
       -lopencv_highgui \
       -lopencv_imgcodecs \

CFLAGS = -Wall $(INCS) -DOUTPUT_DIR='"$(OUTDIR)/"'
CXXFLAGS = $(CFLAGS) --std=c++11
LDFLAGS = $(LIBS)

# Source and Files
C_SOURCES = $(shell find $(SRCDIR) -maxdepth 1 -type f -name *.c)
CXX_SOURCES = $(shell find $(SRCDIR) -maxdepth 1 -type f -name *.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILD)/%.o,$(C_SOURCES)) $(patsubst $(SRCDIR)/%.cpp,$(BUILD)/%.o,$(CXX_SOURCES))


# Targets 
TARGET = test

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) -o $@ -c $< $(CFLAGS)

$(BUILD)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f $(TARGET) 
	rm -rf $(OUTDIR) $(BUILD)

.PHONY : clean all
