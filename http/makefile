++ =g++
arch =-m64
object= httpserver.o run.o httpRequest.o
header=httpserver.h httpRequest.h httpResponse.h httpHeader.h
libPath=
lib=
IncludePaht=
c++11= -std=c++11

run: $(object)
	$(++) $(arch) -o server $(object) $(header) $(c++11) -pthread -fpermissive
run.o: run.cpp
	$(++) $(arch) -c run.cpp $(c++11) -pthread -fpermissive
httpserver.o: httpserver.h httpserver.cpp
	$(++) $(arch) -c httpserver.cpp  $(c++11) -pthread -fpermissive
httpRequest.o: httpRequest.h httpRequest.cpp
	$(++) $(arch) -c httpRequest.cpp  $(c++11) -pthread -fpermissive
httpRequest.h:
httpRequest.cpp:

run.cpp: 
httpserver.h:
httpserver.cpp:

clean:
	rm -rf ./*.o
