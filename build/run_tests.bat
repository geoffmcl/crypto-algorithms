@setlocal
@set TMPFAIL=
@set TMPOPTS=-T

@set TMPTEST=md5
@set TMPEXE=Release\%TMPTEST%_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@echo Doing: %TMPEXE% %TMPOPTS%
@%TMPEXE% %TMPOPTS%
@if ERRORLEVEL 1 goto FAILED1
@goto DNTEST1
:FAILED1
@set TMPFAIL=%TMPFAIL% %TMPTEST%
:DNTEST1


@set TMPTEST=sha1
@set TMPEXE=Release\%TMPTEST%_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@echo Doing: %TMPEXE% %TMPOPTS%
@%TMPEXE% %TMPOPTS%
@if ERRORLEVEL 1 goto FAILED2
@goto DNTEST2
:FAILED2
@set TMPFAIL=%TMPFAIL% %TMPTEST%
:DNTEST2


@set TMPTEST=sha256
@set TMPEXE=Release\%TMPTEST%_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@echo Doing: %TMPEXE% %TMPOPTS%
@%TMPEXE% %TMPOPTS%
@if ERRORLEVEL 1 goto FAILED3
@goto DNTEST3
:FAILED3
@set TMPFAIL=%TMPFAIL% %TMPTEST%
:DNTEST3

@echo.
@if "%TMPFAIL%x" == "x" goto ALLOK
@echo NOTE Test %TMPFAIL% FAILED
@echo.
@goto END
:ALLOK
@echo Appears ALL tests PASSED
@echo.
@goto END

:NOEXE
@echo Error: Can NOT locate %TMPEXE%! *** FIX ME ***
@goto END

:END
