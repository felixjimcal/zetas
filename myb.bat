@echo off

rem *** Win32 ***
DEL problem.exe
WCL386 -oneatx -ohirbk -ol -ol -oi -oi -ei -zp4 -6s -fpi87 -fp6 -mf -s -zm -d_MEMTRACE=5 /bt=nt /l=nt /fhwe /"OPTION ELIMINATE" /"OPTION QUIET" /"OPTION STACK=300000" main.c cola.c events.c
REN main.exe problem.exe

rem *** Win32 ***
DEL problemp.exe
WCL386 -oneatx -et -6s -d2 -d_MEMTRACE=5 /bt=nt /l=nt /fhwe /"OPTION ELIMINATE" /"OPTION VFREMOVAL" /"DEBUG WATCOM ALL" main.c cola.c events.c
REN main.exe problemp.exe

rem *** Win32 ***
DEL problemd.exe
WCL386 -oneatx -6s -d2 -d_MEMTRACE=5 /bt=nt /l=nt /fhwe /"OPTION ELIMINATE" /"OPTION VFREMOVAL" main.c cola.c events.c
REN main.exe problemd.exe


