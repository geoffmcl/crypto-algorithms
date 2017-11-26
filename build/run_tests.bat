@setlocal
@set TMPOPTS=-T
@set TMPEXE=Release\md5_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@echo Doing: %TMPEXE% %TMPOPTS%
@%TMPEXE% %TMPOPTS%
@set TMPEXE=Release\sha1_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@echo Doing: %TMPEXE% %TMPOPTS%
@%TMPEXE% %TMPOPTS%
@set TMPEXE=Release\sha256_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@echo Doing: %TMPEXE% %TMPOPTS%
@%TMPEXE% %TMPOPTS%
@goto END

:NOEXE
@echo Error: Can NOT locate %TMPEXE%! *** FIX ME ***
@goto END

:END
