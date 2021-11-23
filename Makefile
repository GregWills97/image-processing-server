CC=gcc
CXX=g++
CCFLAGS=-Iinclude -I/usr/include/opencv4 --std=c++11
CXXFLAGS=-Iinclude -I/usr/include/opencv4 --std=c++11

ROOTDIR = .
OBJDIR = $(ROOTDIR)/obj
SRCDIR = $(ROOTDIR)/src
OUTDIR = $(ROOTDIR)/output

LIBS = -lopencv_core \
	   -lopencv_imgproc \
	   -lopencv_highgui \
	   -lopencv_imgcodecs \

CONF = -DOUTPUT_DIR='"$(OUTDIR)/"'

OBJ = $(OBJDIR)/image_process.o

TARGET = test

.PHONY : clean all

all : $(TARGET)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) -o $@ -c $< $(CCFLAGS) $(CONF)

$(TARGET) : $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
	rm -rf $(OBJDIR)
