
DEBUG=-g

CSRC=$(shell ls *.c)
CPPSRC=$(shell ls *.cpp)

OBJS=$(CSRC:.c=.o) 

COMM_DIR = ../src

#-lr -lstdc++
COMM_LIB = $(COMM_DIR)/cm_lib.a -I$(COMM_DIR) 

ALL_BINARY = test_cm_cfg test_cm_log test_cm_shm test_cm_debug \
			 test_cm_binary test_cm_process test_cm_net

ARGS = -g -Wall -fno-strict-aliasing #-Qunused-arguments

CC = g++

.c.o:
	$(CC) -c $*.c $(COMM_LIB)

ALL : $(ALL_BINARY)

#$(ALL_BINARY) : $(OBJS)
#	$(CC) $(ARGS) -o $@ $^ $(COMM_LIB)

test_cm_cfg : test_cm_cfg.o
	$(CC) $(ARGS) -o $@ $^ $(COMM_LIB)
	
test_cm_log : test_cm_log.o
	$(CC) $(ARGS) -o $@ $^ $(COMM_LIB)

test_cm_shm : test_cm_shm.o
	$(CC) $(ARGS) -o $@ $^ $(COMM_LIB)

test_cm_debug : test_cm_debug.o
	$(CC) $(ARGS) -o $@ $^ $(COMM_LIB)

test_cm_binary : test_cm_binary.o
	$(CC) $(ARGS) -o $@ $^ $(COMM_LIB)

test_cm_process : test_cm_process.o
	$(CC) $(ARGS) -o $@ $^ $(COMM_LIB)

test_cm_net : test_cm_net.o
	$(CC) $(ARGS) -o $@ $^ $(COMM_LIB)

clean:
	rm -rf *.o *.a $(ALL_BINARY)
