#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= main.o kernel.o cylon-6.o

LIBS	= $(CIRCLEHOME)/lib/libcircle.a

include ../Rules.mk
