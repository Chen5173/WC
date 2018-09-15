#相关WC.exe程序使用说明
##1.使用方法：wc.exe [parameter] [file_name]
  
##2.基本功能：  
- 1 .wc.exe -c file.c:返回文件file.c的可显字符数  
- 2 .wc.exe -w file.c:返回文件file.c的单词数
- 3 .wc.exe -l file.c:返回文件file.c的行数
  
##3.扩展功能：
- 1.wc.exe -a file.c:返回文件file.c的复杂数据，包括注释行、空行、代码行  
- 2.wc.exe -s -c/w/l/a **.cpp/txt/*:返回wc.exe所在路径的所有符合文件的所有相关信息