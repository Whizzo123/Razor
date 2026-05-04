
set GENERATOR=%1

if "%GENERATOR%"=="" (
    set GENERATOR=vs2022
)

echo Generating project with %GENERATOR%...
call vendor\bin\premake\premake5.exe %GENERATOR%

pause