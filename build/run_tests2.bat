@setlocal

@REM Different: md5_test.exe, sha1_test.exe, sha256_test.exe

@for %%i in (aes arcfour base64 blowfish des md2 rot-13) do @(call :RUNTEST %%i)

@goto END

:RUNTEST
@set TMPEXE=%1_test.exe
@if NOT EXIST Release\%TMPEXE% goto NOTEST
@echo Doing: 'Release\%TMPEXE%'
@Release\%TMPEXE%
@goto :EOF
:NOTEST
@echo Can NOT locate 'Release\%TMPEXE%'
@goto :EOF

:END
