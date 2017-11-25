@setlocal
@set TMPEXE=Release\md5_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@echo Doing: %TMPEXE% -t
@%TMPEXE% -t
@set TMPEXE=Release\sha1_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@echo Doing: %TMPEXE% -t
@%TMPEXE% -t
@set TMPEXE=Release\sha256_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@echo Doing: %TMPEXE% -t
@%TMPEXE% -t
@goto END

:NOEXE
@echo Error: Can NOT locate %TMPEXE%! *** FIX ME ***
@goto END

:END
