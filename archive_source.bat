rmdir /s /q release\source

mkdir release
mkdir release\source
del /q mihajong\_*.hsp
copy mihajong\*.hsp release\source

mkdir release\source\icons
copy mihajong\icons\*.ico release\source\icons

mkdir release\source\mjcore
copy mjcore\*.cpp release\source\mjcore
copy mjcore\*.h release\source\mjcore
copy mjcore\*.rc release\source\mjcore
copy mjcore\*.sln release\source\mjcore
copy mjcore\*.vcxproj release\source\mjcore
copy mjcore\*.vcxproj.filters release\source\mjcore

mkdir release\source\logger
copy logger\*.cpp release\source\logger
copy logger\*.h release\source\logger
copy logger\*.rc release\source\logger
copy logger\*.vcxproj release\source\logger
copy logger\*.vcxproj.filters release\source\logger

del /q source.zip
cd release
"C:\Program Files\7-zip\7z.exe" a -mx=9 ..\source.zip source
cd ..
