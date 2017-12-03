@setlocal
@set TMPFAIL=
@set TMPFIL=bldlog-1.txt
@set TMPCHK=temp-%TMPFIL%
@echo.

@set TMPEXE=Release\md5_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@set TMPOUT=%TMPCHK%.MD5
@if EXIST %TMPOUT% @del %TMPOUT%
@echo Doing: %TMPEXE% %TMPFIL% -o %TMPOUT%
@%TMPEXE% %TMPFIL% -o %TMPOUT%
@type %TMPOUT%
@echo.
@echo Doing: %TMPEXE% -c %TMPOUT%
@%TMPEXE% -c %TMPOUT%
@if ERRORLEVEL 1 goto FAILED1
@goto DNONE1
:FAILED1
@set TMPFAIL=%TMPFAIL% MD5
:DNONE1
@echo.

@set TMPEXE=Release\sha1_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@set TMPOUT=%TMPCHK%.SHA1
@if EXIST %TMPOUT% @del %TMPOUT%
@echo Doing: %TMPEXE% %TMPFIL% -o %TMPOUT%
@%TMPEXE% %TMPFIL% -o %TMPOUT%
@type %TMPOUT%
@echo.
@echo Doing: %TMPEXE% -c %TMPOUT%
@%TMPEXE% -c %TMPOUT%
@if ERRORLEVEL 1 goto FAILED2
@goto DNONE2
:FAILED2
@set TMPFAIL=%TMPFAIL% SHA1
:DNONE2
@echo.

@set TMPEXE=Release\sha256_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@set TMPOUT=%TMPCHK%.SHA256
@if EXIST %TMPOUT% @del %TMPOUT%
@echo Doing: %TMPEXE% %TMPFIL% -o %TMPOUT%
@%TMPEXE% %TMPFIL% -o %TMPOUT%
@type %TMPOUT%
@echo.
@echo Doing: %TMPEXE% -c %TMPOUT%
@%TMPEXE% -c %TMPOUT%
@if ERRORLEVEL 1 goto FAILED3
@goto DNONE3
:FAILED3
@set TMPFAIL=%TMPFAIL% SHA256
:DNONE3
@echo.

@if "%TMPFAIL%x" == "x" (
@echo Appears ALL 'checks' PASSED
) else (
@echo 'check' %TMPFAIL% FAILED
)
@echo.

@goto END

:NOEXE
@echo Error: Can NOT locate %TMPEXE%! *** FIX ME ***
@goto END

:END
