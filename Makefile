#Tool definitions
CC=gcc
CXX=g++

# paths
ROOTDIR = .
SRCDIR = $(ROOTDIR)/src
OUTDIR = $(ROOTDIR)/output


#includes, libs, and flags
INCS = -I/usr/include/opencv4 -I$(ROOTDIR)/include

LIBS = -lopencv_core \
       -lopencv_imgproc \
       -lopencv_highgui \
       -lopencv_imgcodecs \

CFLAGS = -Wall $(INCS) -DOUTPUT_DIR='"$(OUTDIR)/"'
CXXFLAGS = $(CFLAGS) --std=c++11
LDFLAGS = $(LIBS)


C_SRC = $(SRCDIR)/server.c $(SRCDIR)/csapp.c
CXX_SRC = $(SRCDIR)/image_process.cpp
OBJ = $(CXX_SRC:.cpp=.o) $(C_SRC:.c=.o)

TARGET = test

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)
	rm -rf $(OUTDIR)

.c.o:
	$(CC) -o $@ -c $< $(CFLAGS) 

.cpp.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS) 

.PHONY : clean all
