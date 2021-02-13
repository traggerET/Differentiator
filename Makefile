start: differentiator.exe

differentiator.exe: objectsFiles/diffMain.o objectsFiles/diffFuncs.o objectsFiles/diffClasses.o
	g++ objectsFiles/diffMain.o objectsFiles/diffFuncs.o objectsFiles/diffClasses.o -o differ.out

objectsFiles/diffMain.o: mainP/main_prog.cpp
	g++ -c mainP/main_prog.cpp -o objectsFiles/diffMain.o

objectsFiles/diffFuncs.o: funcs/functions_def.cpp
	g++ -c funcs/functions_def.cpp -o objectsFiles/diffFuncs.o

objectsFiles/diffClasses.o: Classes/Class_defining.cpp
	g++ -c Classes/Class_defining.cpp -o objectsFiles/diffClasses.o

clear: 
	rm objectsFiles/diffClasses.o
	rm objectsFiles/diffFuncs.o
	rm objectsFiles/diffMain.o

