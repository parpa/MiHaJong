rmdir /s /q release

mkdir release
mkdir release\mihajong
copy mihajong\*.exe release\mihajong
del /q release\mihajong\logger.exe
copy mihajong\*.dll release\mihajong
copy mihajong\readme.txt release\mihajong
copy mihajong\license.txt release\mihajong
copy mihajong\history.txt release\mihajong
copy mihajong\playing.txt release\mihajong
mkdir release\mihajong\bgm
rem mkdir release\mihajong\dat
rem copy work\dat\*.* release\mihajong\dat
mkdir release\mihajong\haifu
copy work\haifu\haifu.css release\mihajong\haifu
mkdir release\mihajong\img
copy work\img\*.* release\mihajong\img
mkdir mihajong\sound
copy work\sound\*.* release\mihajong\sound
call archive_source.bat
copy source.zip release\mihajong
del /q mihajong.zip
cd release
"C:\Program Files\7-zip\7z.exe" a -mx=9 ..\mihajong.zip mihajong
cd ..
