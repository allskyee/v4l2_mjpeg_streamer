CC=gcc
CFLAGS=-I. -DMOTION_V4L2 
LDFLAGS=-ljpeg -lc -lpthread
SRCS=main.c picture.c webcam.c video2.c
INC=global.h
OBJS=$(SRCS:.c=.o)

mjpeg_streamer : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) 

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	rm -f *.o mjpeg_streamer
