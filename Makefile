# Objects
objects = main.o admin.o member.o trainer.o

# Default target
all: fitness_app.exe

# Link objects into executable
fitness_app.exe: $(objects)
	g++ -o fitness_app.exe $(objects) -L"C:/Program Files/PostgreSQL/16/lib" -lpq

# Compile each source file
main.o: main.cpp admin.hpp member.hpp trainer.hpp db.hpp
	g++ -c main.cpp

admin.o: admin.cpp admin.hpp db.hpp
	g++ -c admin.cpp

member.o: member.cpp member.hpp db.hpp
	g++ -c member.cpp

trainer.o: trainer.cpp trainer.hpp db.hpp
	g++ -c trainer.cpp

# Clean up compiled files
clean:
	rm -f fitness_app.exe *.o
