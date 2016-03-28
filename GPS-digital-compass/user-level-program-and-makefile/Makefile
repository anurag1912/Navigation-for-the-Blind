CC = arm-linux-gcc

OBJ = counterinfo.o

default: counterinfo

%.o: %.c
	$(CC) -c -o $@ $< 

counterinfo: $(OBJ)
	$(CC) -o $@ $^

clean:
	-rm -f $(OBJ)
	-rm -f counterinfo
