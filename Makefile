mysort : mysort.o
	cc -o mysort mysort.o -pthread
 
   mysort.o : mysort.c
	cc -c mysort.c -lm

clean:
	rm -f *.o mysort


