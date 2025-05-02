set PATH=%PATH%;D:\masm32\bin
set INCLUDE=D:\masm32\include
set LIB=D:\masm32\lib

ml /c /coff Asm_DLL.asm
# 生成可执行文件
# link /subsystem:windows DisplayTime.obj Project2.res
# 生成DLL
link /subsystem:windows /DLL /DEF:Asm_DLL.def Asm_DLL.obj 
