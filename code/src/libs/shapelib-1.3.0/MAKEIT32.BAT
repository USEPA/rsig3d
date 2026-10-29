REM Compile this library and its test program on MINGW32 Windows platform.

cls
del ..\..\..\lib\Win32\libShapefile.a
xcopy /k /y shapefil.h ..\..\..\include\
gcc -m32 -Wall -DNDEBUG -static -O -c shpopen.c dbfopen.c
ar -rv ../../../lib/Win32/libShapefile.a shpopen.o dbfopen.o
ranlib ../../../lib/Win32/libShapefile.a
del *.o

