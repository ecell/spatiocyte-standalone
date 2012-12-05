#$ gcc -fpic -o hello.o -c hello.c
#$ gcc -fpic -I/usr/include/python -o helloWrapper.o -c helloWrapper.c
#$ gcc -shared hello.o helloWrapper.o -o hellomodule.so

#g++ -fpic -o tes
g++ -shared test.cpp -fpic -o test.so 
