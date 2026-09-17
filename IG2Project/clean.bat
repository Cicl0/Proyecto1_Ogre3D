@echo off
setlocal

set "ROOT=%~dp0"
set "TARGET=%ROOT%.vs"
set "DBG=%ROOT%x64\Debug"
set "REL=%ROOT%x64\Release"
set "X64_REL=%ROOT%IG2Project\x64\Release"
set "X64_DEB=%ROOT%IG2Project\x64\Debug"


echo Looking for: "%TARGET%"
if exist "%TARGET%" (
  attrib -h -s "%TARGET%" 2>nul
  echo Deleting "%TARGET%" ...
  rmdir /s /q "%TARGET%"
  if exist "%TARGET%" (
    echo Failed to delete "%TARGET%". Try running as Administrator.
  ) else (
    echo Deleted "%TARGET%".
  )
) else (
  echo No .vs directory found at "%TARGET%".
)

echo Cleaning binaries in:
echo  - "%DBG%"
echo  - "%REL%"
echo.

for %%D in ("%DBG%" "%REL%") do (
  if exist "%%~D" (
    echo Processing "%%~D" ...
    pushd "%%~D" >nul
    rem Remove hidden/system attributes then delete (recursive, quiet, force)
    attrib -h -s /s *.dll 2>nul
    attrib -h -s /s *.exe 2>nul
    attrib -h -s /s *.pdb 2>nul
    del /a /f /q /s *.dll 2>nul
    del /a /f /q /s *.exe 2>nul
    del /a /f /q /s *.pdb 2>nul
    popd >nul
  ) else (
    echo Skipped: "%%~D" (not found)
  )
)

rem --- Remove binary files from IG2Project\x64 folder (IG2Project\x64) ---
for %%D in ("%X64_REL%" "%X64_DEB%") do (
  if exist "%%~D" (
    echo Processing "%%~D" ...
    pushd "%%~D" >nul
    rem Remove hidden/system attributes then delete (recursive, quiet, force)
    attrib -h -s /s *.obj 2>nul
    attrib -h -s /s *.log 2>nul
    attrib -h -s /s *.recipe 2>nul
    attrib -h -s /s *.ilk 2>nul
    attrib -h -s /s *.txt 2>nul
    attrib -h -s /s *.idb 2>nul
    attrib -h -s /s *.pdb 2>nul
    attrib -h -s /s *.iobj 2>nul
    attrib -h -s /s *.ipdb 2>nul
    del /a /f /q /s *.obj 2>nul
    del /a /f /q /s *.log 2>nul
    del /a /f /q /s *.recipe 2>nul
    del /a /f /q /s *.ilk 2>nul
    del /a /f /q /s *.txt 2>nul
    del /a /f /q /s *.idb 2>nul
    del /a /f /q /s *.pdb 2>nul
    del /a /f /q /s *.iobj 2>nul
    del /a /f /q /s *.ipdb 2>nul
    popd >nul
  ) else (
    echo Skipped: "%%~D" (not found)
  )
)



echo.
echo Done.
pause