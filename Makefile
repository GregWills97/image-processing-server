CC=g++
CFLAGS=-Iinclude -I/usr/include/opencv4 --std=c++11

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
	$(CC) -o $@ -c $< $(CFLAGS) $(CONF)

$(TARGET) : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
	rm -f $(OBJDIR)/*.o
