@echo off
del /f /s /q *.pbd;*.ilk;*.pdb;*.ncb;*.%username%.user
del /f /s /q /ah *.suo
rmdir /s /q .\src\ExhaustiveAnalyzeSearch\Release;.\src\ExhaustiveAnalyzeSearch\Debug;.\src\ExhaustiveAnalyzeSearch\x64