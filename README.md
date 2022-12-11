zlib 1.2.13 PS4 **port** using OOSDK.

changes:
zconf.h - added a pre-processor check for __OPENORBIS__ that defines ZEXPORT && ZEXPORTVA as "extern "C"".
gzguts.h - added a pre-processor check for __OPENORBIS__ that includes <orbis/libkernel.h> and defines open/read/write/close/lseek as the sceKernelXXXXXX equals.

#THIS IS THE WORK OF ZLIB.NET NOT ME!
**orginal un-altered software**: https://zlib.net/