@echo off
for /L %%i in (1,1,6) do (
    for /L %%j in (4,1,21) do (
       MultiplePatternMatching %%i %%j RandomDNA_1G.txt
       timeout /t 3 /nobreak >nul
    )
)
pause
cmd /k