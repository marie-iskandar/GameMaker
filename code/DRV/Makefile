clean:
	rm *.o *.exe

VIDDRIVE.o: VIDDRIVE.HPP VIDDRVC.CPP
	clang++ -g -O0 -c VIDDRVC.CPP -o VIDDRIVE.o

TRY.exe: TRY.CPP VIDDRIVE.o
	clang++ -g -O0 -o TRY.exe TRY.CPP VIDDRIVE.o