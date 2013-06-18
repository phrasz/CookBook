@echo off
%rem This is an example tree batch script
%rem 	It will find all directories, subdirectories, and files from c:\
%rem	Also, it finds solely the exe's (or fill in your desired file type...) 
tree c:\ /A /F >C:\Users\dOOBER\Desktop\batch\tree_from_c.txt
tree c:\ /A /F | findstr /c:\ /c:+ /c:exe  >C:\Users\dOOBER\Desktop\batch\tree_from_c_exe.txt 
pause

