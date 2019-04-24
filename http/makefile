++ =g++
arch =-m64
object= httpserver.o run.o 
header=httpserver.h httpRequest.h httpResponse.h httpHeader.h
libPath=
lib=
IncludePaht=
c++11= -std=c++11

run: $(object)
	$(++) $(arch) -o run $(object) $(header) $(c++11) -pthread -fpermissive
run.o: run.cpp
	$(++) $(arch) -c run.cpp $(c++11) -pthread -fpermissive
httpserver.o: httpserver.h httpserver.cpp
	$(++) $(arch) -c httpserver.cpp  $(c++11) -pthread -fpermissive

run.cpp: 
httpserver.h:
httpserver.cpp:

clean:
	rm -rf ./*.o
