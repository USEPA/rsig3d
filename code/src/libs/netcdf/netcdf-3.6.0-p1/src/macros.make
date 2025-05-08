# $Id: macros.make.in,v 1.38 2004/12/18 16:32:32 ed Exp $

# The purpose of this file is to contain common make(1) macros.
# It should be processed by every execution of that utility.


# POSIX shell.  Shouldn't be necessary -- but is under IRIX 5.3.
SHELL		= /bin/sh


# Installation Directories:
SRCDIR		= /Users/plessel/projects/eutrophic/code/src/libs/NetCDF/netcdf-3.6.0-p1/src
prefix		= /Users/plessel/projects/eutrophic/code/src/libs/NetCDF/netcdf-3.6.0-p1
exec_prefix	= $(prefix)
INCDIR		= $(exec_prefix)/include
LIBDIR		= $(exec_prefix)/lib
BINDIR		= $(exec_prefix)/bin
MANDIR		= $(prefix)/man


# Preprocessing:
M4		= m4
M4FLAGS		= -B10000
CPP		= gcc -E
CPPFLAGS	= $(INCLUDES) $(DEFINES)  -Df2cFortran
FPP		= 
FPPFLAGS	= 
CXXCPPFLAGS	= $(CPPFLAGS)


# Compilation:
CC		= gcc
CXX		= g++
FC		= gfortran
F90		= gfortran
CFLAGS		= -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -g -arch ppc -arch ppc64 -arch i686 -arch x86_64
CXXFLAGS	= -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -g -arch ppc -arch ppc64 -arch i686 -arch x86_64
FFLAGS		= -m64 -fopenmp 
F90FLAGS	= 
CC_MAKEDEPEND	= false
COMPILE.c	= $(CC) -c $(CFLAGS) $(CPPFLAGS)
COMPILE.cxx	= $(CXX) -c $(CXXFLAGS) $(CXXCPPFLAGS)
COMPILE.f	= $(FC) -c $(FFLAGS)
COMPILE.F90	= $(F90) -c $(F90FLAGS)
# The following command isn't available on some systems; therefore, the
# `.F.o' rule is relatively complicated.
COMPILE.F	= $(COMPILE.f) $(FPPFLAGS)


# Linking:
MATHLIB		= 
FLIBS		= 
F90LIBS		= 
LIBS		= 
F90LDFLAGS	= $(LDFLAGS)
LINK.c		= $(CC) -o $@ $(CFLAGS) $(LDFLAGS)
LINK.cxx	= $(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS)
LINK.F		= $(FC) -o $@ $(FFLAGS) $(FLDFLAGS)
LINK.f		= $(FC) -o $@ $(FFLAGS) $(FLDFLAGS)
LINK.F90	= $(F90) -o $@ $(F90FLAGS) $(F90LDFLAGS)


# Manual pages:
WHATIS		= whatis
# The following macro should be empty on systems that don't
# allow users to create their own manual-page indexes.
MAKEWHATIS_CMD	= 


# Misc. Utilities:
AR		= libtool -static -o 
ARFLAGS		=
RANLIB		= ranlib
TARFLAGS	= -chf


# Dummy macros: used only as placeholders to silence GNU make.  They are
# redefined, as necessary, in subdirectory makefiles.
HEADER		= dummy_header
HEADER1		= dummy_header1
HEADER2		= dummy_header2
HEADER3		= dummy_header3
LIBRARY		= dummy_library.a
MANUAL		= dummy_manual
PROGRAM		= dummy_program


# Distribution macros:
FTPDIR		= /home/ftp/pub/$(PACKAGE)
FTPBINDIR	= /home/ftp/pub/binary/dummy_system
VERSION		= dummy_version
COMPRESS        = compress
ZIP        = gzip
INSTALL		= /usr/bin/install -c
