
# MakeFile for restChat
# server using C++ Microservice
# sudo mkdir /var/www/html/restChat
# sudo chown ubuntu /var/www/html/restChat
CC=g++
CFLAGS= -std=c++14 -Wno-deprecated-declarations
RM= /bin/rm -f

all: restChat PutHTML

ChatEntry.o: ChatEntry.cpp ChatEntry.h
	$(CC) -c $(CFLAGS) ChatEntry.cpp

ChatDB.o: ChatDB.cpp ChatDB.h
	$(CC) -c $(CFLAGS) -I/usr/include/cppconn ChatDB.cpp



restChat.o: restChat.cpp httplib.h
	$(CC) -c $(CFLAGS) restChat.cpp

restChat: restChat.o ChatDB.o ChatEntry.o 
	$(CC) restChat.o ChatDB.o ChatEntry.o -o restChat -L/usr/local/lib -lmariadbcpp
PutHTML:
	cp restChat.html /var/www/html/restChat/
	cp restChat.css /var/www/html/restChat/
	cp restChat.js /var/www/html/restChat/
	cp chatapp_Icon.png /var/www/html/restChat/

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/restChat/

clean:
	rm restChat *.o
