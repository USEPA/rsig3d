REM Compile this library and its test program on MINGW64 Windows platform.

cls
del ..\..\..\lib\Win64\libShapefile.a
xcopy /k /y shapefil.h ..\..\..\include\
gcc -m64 -Wall -DNDEBUG -static -O -c shpopen.c dbfopen.c
ar -rv ../../../lib/Win64/libShapefile.a shpopen.o dbfopen.o
ranlib ../../../lib/Win64/libShapefile.a
del *.o

