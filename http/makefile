++ =g++
arch =-m64
object= httpserver.o run.o httpRequest.o httpResponse.o
header=httpserver.h httpRequest.h httpResponse.h httpHeader.h 
libPath=
lib=
IncludePaht=
c++11= -std=c++11

run: $(object)
	$(++) $(arch) -o server $(object) $(header) $(c++11) -pthread -fpermissive -g -O0
run.o: run.cpp
	$(++) $(arch) -c run.cpp $(c++11) -pthread -fpermissive	-g -O0
httpserver.o: httpserver.h httpserver.cpp
	$(++) $(arch) -c httpserver.cpp  $(c++11) -pthread -fpermissive	-g -O0
httpRequest.o: httpRequest.h httpRequest.cpp
	$(++) $(arch) -c httpRequest.cpp  $(c++11) -pthread -fpermissive -g -O0
httpResponse.o: httpResponse.h httpResponse.cpp
	$(++) $(arch) -c httpResponse.cpp  $(c++11) -pthread -fpermissive -g -O0

httpRequest.h:

httpRequest.cpp:

httpResponse.h:

httpResponse.cpp:

run.cpp: 
httpserver.h:
httpserver.cpp:

clean:
	rm -rf ./*.o
