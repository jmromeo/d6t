TARGET		= test
OBJS		= test.o
SRCS		= $(OBJS:.o=.c)
INCLUDEDIRS	+= -I../include
LIBDIRS		+= -L../lib
CFLAGS		= $(INCLUDEDIRS) -W -Wall -O2 -g
LDFLAGS		= $(LIBDIRS) -ld6t

CC			= gcc

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	
.c.o:
	$(CC) -c $< $(CFLAGS)
	
clean:
	rm -f $(OBJS) $(TARGET)

dep:
	gccmakedep $(SRCS)

