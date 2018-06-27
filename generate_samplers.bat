echo off

echo %CD%\output\bin\win64\Debug\flatc_ue4.exe -u -o %CD%\samplers\ %CD%\samplers\Common.fbs --ue4-pch-header-file Lab01.h --ue4-use-tab true
%CD%\output\bin\win64\Debug\flatc_ue4.exe -u -o %CD%\samplers\ %CD%\samplers\Common.fbs --ue4-pch-header-file Lab01.h --ue4-use-tab true
echo %ERRORLEVEL%

echo %CD%\output\bin\win64\Debug\flatc_ue4.exe -u -o %CD%\samplers\ %CD%\samplers\Equipment.fbs --ue4-pch-header-file Lab01.h --ue4-indent-space-count 2
%CD%\output\bin\win64\Debug\flatc_ue4.exe -u -o %CD%\samplers\ %CD%\samplers\Equipment.fbs --ue4-pch-header-file Lab01.h --ue4-indent-space-count 2
echo %ERRORLEVEL%

echo %CD%\output\bin\win64\Debug\flatc_ue4.exe -u -o %CD%\samplers\ %CD%\samplers\Monster.fbs --ue4-pch-header-file Lab01.h
%CD%\output\bin\win64\Debug\flatc_ue4.exe -u -o %CD%\samplers\ %CD%\samplers\Monster.fbs --ue4-pch-header-file Lab01.h
echo %ERRORLEVEL%