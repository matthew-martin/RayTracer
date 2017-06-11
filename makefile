OBJS = main.o commandHandler.o misc.o implicitShape.o phongLightSource.o shape.o shapeCollection.o surfaceShape.o viewport.o 

all: project5

project5: $(OBJS)
	g++ -lglut -lGL $(OBJS) -o project5

main.o: main.cpp main.h
	g++ -c -Wall main.cpp


commandHandler.o: commandHandler.cpp commandHandler.h
	g++ -c -Wall commandHandler.cpp

implicitShape.o: implicitShape.cpp implicitShape.h
	g++ -c -Wall implicitShape.cpp

misc.o: misc.cpp misc.h
	g++ -c -Wall misc.cpp

phongLightSource.o: phongLightSource.cpp phongLightSource.h
	g++ -c -Wall phongLightSource.cpp

shape.o: shape.cpp shape.h
	g++ -c -Wall shape.cpp

shapeCollection.o: shapeCollection.cpp shapeCollection.h
	g++ -c -Wall shapeCollection.cpp

surfaceShape.o: surfaceShape.cpp surfaceShape.h
	g++ -c -Wall surfaceShape.cpp

viewport.o: viewport.cpp viewport.h
	g++ -c -Wall viewport.cpp


clean:
	rm -f *.o core project5