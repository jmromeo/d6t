TARGET			 = libd6t.a
SHARED			 = libd6t.so
OBJS				 = d6t.o i2c-reg.o
SRCS				 = $(OBJS:.o=.c)
INCLUDEDIRS	+= -I../include
LIBDIRS			+=
CFLAGS			 = $(INCLUDEDIRS) -W -Wall -O2 -fpic

CC			= gcc
AR			= ar

all: $(TARGET) $(SHARED)

$(TARGET): $(OBJS)
	$(AR) rs $@ $^
	cp $(TARGET) ../lib

$(SHARED): $(OBJS)
	$(CC) -shared -o $@ $^
	cp $(SHARED) ../lib

.c.o:
	$(CC) -c $< $(CFLAGS)

install:
	cp $(SHARED) /usr/lib/

clean:
	rm -f $(OBJS) $(TARGET) $(SHARED) ../lib/$(TARGET) ../lib/$(SHARED)

dep:
	gccmakedep $(SRCS)

