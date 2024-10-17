@echo off

for /L %%i in (4,1,21) do (
   MultiplePatternMatching 4 %%i RandomDNA_1G.txt
   timeout /t 3 /nobreak >nul
)

pause
cmd /k