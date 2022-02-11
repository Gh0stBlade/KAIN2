#include <stdio.h>

//0001:00031400       _PCinit                    00432400 f   libsn.obj
int	PCinit(void)
{
	return (fcloseall() != -1) - 1;
}
//0001 : 00031410       _PCopen                    00432410 f   libsn.obj
int	PCopen(char* name, int flags, int perms)
{
	static char *modes[] =
	{
		"rb",
		"r+b",
		"r+b"
	};

	int result = (int)fopen(name, modes[flags]);
	if (!result)
		return -1;
	return result;
}
//0001 : 00031450       _PCcreat                   00432450 f   libsn.obj
int	PCcreat(char* name, int perms)
{
	int result; // eax

	result = (int)fopen(name, "w+b");
	if (!result)
		return -1;
	return result;
}
//0001 : 00031470       _PClseek                   00432470 f   libsn.obj
int	PClseek(int fd, int offset, int mode)
{
	static char origin[] = { SEEK_SET, SEEK_CUR, SEEK_END };

	fseek((FILE*)fd, offset, origin[mode]);
	return ftell((FILE*)fd);
}
//0001 : 000314c0       _PCread                    004324c0 f   libsn.obj
int	PCread(int fd, char* buff, int len)
{
	return fread(buff, 1, len, (FILE*)fd);
}
//0001 : 000314e0       _PCwrite                   004324e0 f   libsn.obj
int	PCwrite(int fd, char* buff, int len)
{
	return fwrite(buff, 1, len, (FILE*)fd);
}
//0001 : 00031500       _PCclose                   00432500 f   libsn.obj
int	PCclose(int fd)
{
	return (fclose((FILE*)fd) != -1) - 1;
}
