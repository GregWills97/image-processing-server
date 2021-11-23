#Tool definitions
CC=gcc
CXX=g++

# paths
ROOTDIR = .
SRCDIR = ${ROOTDIR}/src
OUTDIR = ${ROOTDIR}/output


#includes, libs, and flags
INCS = -I/usr/include/opencv4 -I${ROOTDIR}/include

LIBS = -lopencv_core \
       -lopencv_imgproc \
       -lopencv_highgui \
       -lopencv_imgcodecs \

CCFLAGS = -Wall --std=c++11 ${INCS} -DOUTPUT_DIR='"${OUTDIR}/"'
CXXFLAGS = ${CCFLAGS}
LDFLAGS = ${LIBS}


SRC = ${SRCDIR}/image_process.cpp
OBJ = ${SRC:.cpp=.o}

TARGET = test

all : ${TARGET}

${OBJ} : ${SRC}
	${CXX} -o $@ -c $< ${CXXFLAGS} 

${TARGET} : ${OBJ}
	${CXX} -o $@ $^ ${LDFLAGS}

clean:
	rm -f ${TARGET}
	rm -f ${OBJ}
	rm -rf ${OUTDIR}


.PHONY : clean all
