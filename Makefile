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

CFLAGS = -Wall ${INCS} -DOUTPUT_DIR='"${OUTDIR}/"'
CXXFLAGS = ${CFLAGS} --std=c++11
LDFLAGS = ${LIBS}


C_SRC = ${SRCDIR}/csapp.c
CXX_SRC = ${SRCDIR}/image_process.cpp
C_OBJ = ${C_SRC:.c=.o}
CXX_OBJ = ${CXX_SRC:.cpp=.o}

TARGET = test

all : ${TARGET}

${C_OBJ} : ${C_SRC}
	${CC} -o $@ -c $< ${CFLAGS} 

${CXX_OBJ} : ${CXX_SRC}
	${CXX} -o $@ -c $< ${CXXFLAGS} 

${TARGET} : ${C_OBJ} ${CXX_OBJ}
	${CXX} -o $@ $^ ${LDFLAGS}

clean:
	rm -f ${TARGET}
	rm -f ${C_OBJ} ${CXX_OBJ}
	rm -rf ${OUTDIR}


.PHONY : clean all
