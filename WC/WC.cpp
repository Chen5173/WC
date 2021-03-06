// WC.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <string.h>
#pragma warning(disable:4996)

//改变目录的函数
void cd(char *Addr, char *filename, int flag) {//flag==1时表示进入名为filename的文件夹，flag==0时表示退出名为filename的文件夹
	if (flag == 1) {                        //进入指定文件夹
		strcat(Addr, filename);
		strcat(Addr, "\\");
	}
	else if (flag == 0) {                  //从文件夹中退回到上一个文件夹
		memset(strrchr(Addr, '\\'), 0, sizeof(filename));
		memset(strrchr(Addr, '\\') + 1, 0, sizeof(filename));
	}
	else if (flag == 2) {                 //从文件的绝对路径退回到文件所在路径
		memset(strrchr(Addr, '\\') + 1, 0, sizeof(filename));
	}
}

//查找字符个数函数
int char_num(char *path) {
	FILE *pf = fopen(path, "r+");
	if (pf == NULL)
		printf("open the file failed!\n");
	char a;
	int num = 0;
	do {
		a = fgetc(pf);
		if (a >= 33 && a <= 126)
			num++;
	} while (a != -1);
	fclose(pf);
	return num;
}

//查找行数
int line_num(char *path) {
	FILE *pf = fopen(path, "r+");
	if (pf == NULL)
		printf("open the file failed!\n");
	int line = 0;
	char a;
	do {
		a = fgetc(pf);
		if (a == 0xa || a == -1)
			line++;
	} while (a != -1);
	fclose(pf);
	return line;
}

//查找单词数
int word_num(char *path) {
	FILE *pf = fopen(path, "r+");
	if (pf == NULL)
		printf("open the file failed!\n");
	char a, b;
	char *l;
	int word = 0;
	do {
		a = fgetc(pf);
		l = (char *)(pf->_Placeholder);
		b = *l;
		if (((a >= 'a'&&a <= 'z') || (a >= 'A' && a <= 'Z')) && ((b < 'a' || b > 'z') && (b < 'A' || b > 'Z')))
			word++;
	} while (a != -1);
	fclose(pf);
	return word;
}

//查找代码行、空行、注释行
void complex_line(char *path) {
	FILE *pf = fopen(path, "r+");
	if (pf == NULL)
		printf("open the file failed!\n");
	char a[100] = {0}, b;
	char *addr;
	int i = 0, flag = 0;
	int code_line = 0, space_line = 0, anno_line = 0;
	do {
		do {
			b = fgetc(pf);
			if (b >= 33 && b < 126)
				a[i++] = b;
		} while (b != 0xa && b != -1);
		flag = 0;
		i = 0;
		if (strlen(a) <= 1)
			space_line++;
		else {
			addr = strchr(a, '\/\/');
			if (addr == 0)
				code_line++;
			else {
				i = addr - a;
				while (addr != a) {
					addr--;
					if (*addr >= 33 && *addr <= 126) {
						flag++;
						if (flag > 1) {
							code_line++;
							break;
						}
					}
				}
				if (flag <= 1)
					anno_line++;
			}
			i = 0;
		}
		memset(a, NULL, 100);
	} while (b != -1);
	printf("    查看复杂属性:\n");
	printf("        空行数为:%d\n", space_line);
	printf("        注释行数为:%d\n", anno_line);
	printf("        代码行数为:%d\n", code_line);
	fclose(pf);
}

//递归查询函数
void view(char *FileAddr, char *type, char c)
{
	struct _finddata_t file; //定义结构体变量
	long handle;
	char *local;
	FILE *pf = NULL;
	char cFileAddr[300];
	strcpy(cFileAddr, FileAddr);
	strcat(cFileAddr, "*.*");
	handle = _findfirst(cFileAddr, &file);//查找所有文件
	if (handle == -1)//如果handle为－1, 表示当前目录为空, 则结束查找而返回如果handle为－1, 表示当前目录为空, 则结束查找而返回 
		return;
	else
	{
		if (file.attrib &_A_SUBDIR) //是目录
		{
			if (file.name[0] != '.') //文件名不是'.'或'..'时   "."表示本目录   ".."表示返回上级目录
			{
				memset(strrchr(cFileAddr, '\\') + 1, 0, sizeof(file.name));
				cd(cFileAddr, file.name, 1);
				view(cFileAddr, type, c);
				cd(cFileAddr, file.name, 0);//查查找完毕之后, 返回上一级目录找完毕之后, 返回上一级目录 
			}
		}
		else if (file.attrib &_A_ARCH) {
			local = strrchr(file.name, '.');
			if (!strcmp(local, type + 1)) {
				memset((cFileAddr + strlen(cFileAddr) - 3), 0, sizeof(3));
				strcat(cFileAddr, file.name);
				printf("符合条件的文件的位置为：\n    %s\n", cFileAddr);
				if (c == 'c')
					printf("        字符数为：%d\n", char_num(cFileAddr));
				if (c == 'w')
					printf("        单词数为：%d\n", word_num(cFileAddr));
				if (c == 'l')
					printf("        行数为：%d\n", line_num(cFileAddr));
				if (c == 'a')
					complex_line(cFileAddr);
				cd(cFileAddr, file.name, 0);
			}
		}
		while (!(_findnext(handle, &file)))
		{
			if (file.attrib &_A_SUBDIR) //是目录
			{
				if (file.name[0] != '.') //文件名不是'.'或'..'时
				{
					memset(strrchr(cFileAddr, '\\') + 1, 0, sizeof(file.name));
					cd(cFileAddr, file.name, 1);
					view(cFileAddr, type, c);
					cd(cFileAddr, file.name, 0);//查查找完毕之后, 返回上一级目录找完毕之后, 返回上一级目录 
				}
			}
			else if (file.attrib &_A_ARCH) {
				local = strrchr(file.name, '.');
				if (local != 0) {
					if (!strcmp(local, type + 1)) {
						if (!strcmp(&cFileAddr[strlen(cFileAddr) - 1], "*"))
							memset((cFileAddr + strlen(cFileAddr) - 3), 0, sizeof(3));
						strcat(cFileAddr, file.name);
						printf("符合条件的文件的位置为：\n    %s\n", cFileAddr);
						if (c == 'c')
							printf("        字符数为：%d\n", char_num(cFileAddr));
						if (c == 'w')
							printf("        单词数为：%d\n", word_num(cFileAddr));
						if (c == 'l')
							printf("        行数为：%d\n", line_num(cFileAddr));
						if (c == 'a')
							complex_line(cFileAddr);
						cd(cFileAddr, file.name, 2);
					}
				}
			}
		}
		_findclose(handle);
	}
}

int main(int argc, char *argv[])
{
	char type[10] = "0";
	char *path = NULL;
	char p;
	if (argc <= 1)
		p = '?';	
	else if (argv[1][1] == 's') {
		p = argv[2][1];
		path = getcwd(NULL, 0);    //获取当前工作环境
		strcpy(type, argv[3]);
		view(path, type, p);
		p = 0;
	}
	else {
		p = argv[1][1];
		path = argv[2];
		type[0] = '*';
		if (path != NULL)
			strcpy(type, strrchr(path, '.'));
	}
	switch (p)
	{
	case 'c':
		printf("字符数为：%d\n", char_num(path));
		break;
	case 'w':
		printf("单词数为：%d\n", word_num(path));
		break;
	case 'l':
		printf("行数为：%d\n", line_num(path));
		break;
	case 's':
		view(path, type, 0);
		break;
	case 0:
		break;
	case 'a':
		complex_line(path);
		break;
	case '?':
		printf("用法：wc.exe [parameter] [file_name]\n");
		printf("    -c 返回文件的字符数\n");
		printf("    -w 返回文件的词的数目\n");
		printf("    -l 返回文件的行数\n");
		printf("    -s 递归处理目录下符合条件的文件\n");
		printf("    -a 返回更复杂的数据（代码行 / 空行 / 注释行）\n");
		break;
	default:
		printf("用法：wc.exe [parameter] [file_name]\n");
		printf("    -c 返回文件的字符数\n");
		printf("    -w 返回文件的词的数目\n");
		printf("    -l 返回文件的行数\n");
		printf("    -s 递归处理目录下符合条件的文件\n");
		printf("    -a 返回更复杂的数据（代码行 / 空行 / 注释行）\n");
		break;
	}
	return 0;
}