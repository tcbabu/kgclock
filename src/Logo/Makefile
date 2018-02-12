KULINA=/usr
#CC	=g++ -pthread
CC	=cc -pthread
AboutDia	: AboutDia.o AboutDiaCallbacks.o main.o
	 $(CC) -o AboutDia AboutDia.o AboutDiaCallbacks.o main.o -I$(KULINA)/include $(KULINA)/lib/libkulina.a $(KULINA)/lib/libgm.a -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread -lz -lbz2 -lGL
AboutDia.o	: AboutDia.c 
	 $(CC) -c AboutDia.c
AboutDiaCallbacks.o	: AboutDiaCallbacks.c 
	 $(CC) -c AboutDiaCallbacks.c
main.o	: main.c 
	 $(CC) -c main.c
clean	:  
	   rm -f *.o AboutDia
