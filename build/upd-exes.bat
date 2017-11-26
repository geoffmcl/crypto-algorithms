@setlocal
@set TMPDST=C:\MDOS
@if NOT EXIST %TMPDST%\nul goto NODST
@set TMPEXE=Release\md5_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@call :CHKUPD %TMPEXE%
@set TMPEXE=Release\sha1_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@call :CHKUPD %TMPEXE%
@set TMPEXE=Release\sha256_test.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@call :CHKUPD %TMPEXE%
@goto END

:CHKUPD
@set TMPFIL=%~nx1
@set TMPTGT=%TMPDST%\%TMPFIL%
@if NOT EXIST %TMPTGT% goto :CHKCOPY
@fc4 -q -v0 %TMPEXE% %TMPTGT%
@if ERRORLEVEL 1 goto :CHKCOPY2
@echo.
@echo Files %TMPEXE% and %TMPTGT% are the SAME! No update required...
@echo.
@goto :EOF

:CHKCOPY
@echo Target %TMPTGT% does NOT exist
@echo Copy %TMPEXE% to %TMPDST%
@pause
@goto :DOUPDATE

:CHKCOPY2
@echo Target %TMPTGT% NOT the same
@echo Copy %TMPEXE% to %TMPTGT%
@pause
@goto :DOUPDATE

:DOUPDATE
@echo Doing update...
@echo copy %TMPEXE% %TMPTGT%
@copy %TMPEXE% %TMPTGT%
@echo Done update...
@echo.
@goto :EOF


:NOEXE
@echo Error: Can NOT locate %TMPEXE%! *** FIX ME ***
@goto END

:NODST
@echo Error: Can NOT locate %TMPDST%! *** FIX ME ***
@echo This is where I install ALL my utilities, AND
@echo this folder is in my PATH environment variable,
@echo so the utilities can be run from anywhere...
@goto END


:END
