#include <string>
#include <iostream>

extern "C"
{
	void __stdcall _ram2022_strcat(char* dest, char* from1, char* from2)
	{
		size_t l1 = strlen(from1);
		size_t l2 = strlen(from2);
		size_t n1 = l1 > 255 ? 255 : l1;
		for (unsigned __int16 i = 0; i < n1; i++) {
			dest[i] = from1[i];
		}
		size_t n2 = l2 > 255 - n1 ? 255 - n1 : l2;
		for (unsigned __int16 i = 0; i < n2; i++) {
			dest[n1 + i] = from2[i];
		}
		dest[n1 + n2] = '\0';
	}

	void _ram2022_strcpy(char* dest, char* from)
	{
		size_t n = strlen(from);
		n = n > 255 ? 255 : n;
		for (unsigned __int16 i = 0; i < n; i++) {
			dest[i] = from[i];
		}
		dest[n] = '\0';
	}

	__int32 _ram2022_strcmp(char* s1, char* s2)
	{
		return strcmp(s1, s2);
	}

	void _ram2022_readStr(char* buf)
	{
		std::cin.getline(buf, 255);
		buf[255] = '\0';
	}

	void _ram2022_writeInt(__int32 i)
	{
		std::cout << i;
	}

	void _ram2022_writeStr(char* s)
	{
		std::cout << s;
	}
}