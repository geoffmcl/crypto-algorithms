@setlocal
@set TMPFIL=bldlog-1.txt
@set TMPCHK=temp-%TMPFIL%

@set TMPEXE=Release\md5_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@set TMPOUT=%TMPCHK%.MD5
@if EXIST %TMPOUT% @del %TMPOUT%
@echo Doing: %TMPEXE% %TMPFIL% to %TMPOUT%
@%TMPEXE% %TMPFIL% > %TMPOUT%
@type %TMPOUT%
@echo Doing: %TMPEXE% -c %TMPOUT%
@%TMPEXE% -c %TMPOUT%
@echo.

@set TMPEXE=Release\sha1_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@set TMPOUT=%TMPCHK%.SHA1
@if EXIST %TMPOUT% @del %TMPOUT%
@echo Doing: %TMPEXE% %TMPFIL% to %TMPOUT%
@%TMPEXE% %TMPFIL% > %TMPOUT%
@type %TMPOUT%
@echo Doing: %TMPEXE% -c %TMPOUT%
@%TMPEXE% -c %TMPOUT%
@echo.

@set TMPEXE=Release\sha256_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@set TMPOUT=%TMPCHK%.SHA256
@if EXIST %TMPOUT% @del %TMPOUT%
@echo Doing: %TMPEXE% %TMPFIL% to %TMPOUT%
@%TMPEXE% %TMPFIL% > %TMPOUT%
@type %TMPOUT%
@echo Doing: %TMPEXE% -c %TMPOUT%
@%TMPEXE% -c %TMPOUT%
@echo.

@goto END

:NOEXE
@echo Error: Can NOT locate %TMPEXE%! *** FIX ME ***
@goto END

:END
