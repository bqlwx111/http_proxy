++ =g++
arch =-m64
object= httpserver.o run.o httpserver.h httpRequest.h httpResponse.h httpHeader.h
libPath=
lib=
IncludePaht=
c++11= -std=c++11

run: $(object)
	$(++) $(arch) -o run $(object) $(c++11) -pthread
run.o: run.cpp
	$(++) $(arch) -c run.cpp $(c++11)
httpserver.o: httpserver.h httpserver.cpp
	$(++) $(arch) -c httpserver.cpp  $(c++11)

run.cpp: 
httpserver.h:
httpserver.cpp:

clean:
	rm -rf ./*.o
