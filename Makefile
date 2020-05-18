# subnet makefile

CC = gcc
CFLAGS = -O2 -Wall

SUBNET_OBJS = subnet.o 
SUBNET_OBJS2 = subnet2.o

SUBNET_EXE = subnet
SUBNET_EXE2 = subnet2
EXES = $(SUBNET_EXE) \
       $(SUBNET_EXE2)

all: ${EXES}

${SUBNET_EXE}: $(SUBNET_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(RM) $(SUBNET_OBJS)

${SUBNET_EXE2}: $(SUBNET_OBJS2)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(RM) $(SUBNET_OBJS2)

%.o: %.c
	${CC} -o $@ -c $< $(CFLAGS)

clean:
	$(RM) *.o $(EXES)
