@echo off
set /p kie=Kie vi volas vortaro havi? (Situo devas en parametro PATH esti):

mkdir %kie%\vortdosiero
copy _vortaro.exe.win %kie%\vortdosiero\_vortaro.exe
echo @echo off > %kie%\vortaro.bat
echo %kie%\vortdosiero\_vortaro.exe %%* >> %kie%\vortaro.bat

echo @echo off > malinstvort.win.bat
echo del %kie%\vortaro.bat >> malinstvort.win.bat
echo rmdir /S /Q %kie%\vortdosiero >> malinstvort.win.bat
echo echo Aplikajxo 'vortaro' estas malinstalita... >> malinstvort.win.bat

echo Aplikajxo 'vortaro' estas instalita.
echo Nun vi povas konvertu csv-dosiero per elekto '-k' - legu helpon.


