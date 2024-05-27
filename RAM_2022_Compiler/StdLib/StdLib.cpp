#include <time.h>
#include <math.h>

extern "C"
{
	char* stdGetDate(char* s)
	{
		time_t t = time(0);
		tm local;
		localtime_s(&local, &t);
		strftime(s, 256, "%d.%m.%Y", &local);
		return s;
	}

	char* stdGetTime(char* s)
	{
		time_t t = time(0);
		tm local;
		localtime_s(&local, &t);
		strftime(s, 256, "%H:%M:%S", &local);
		return s;
	}

	__int32 stdGetTimeCount()
	{
		return (__int32)time(0);
	}

	__int32 stdStrToInt(const char* s)
	{
		__int32 result = 0;
		__int32 k = 1;
		__int32 start = 0;
		while (s[start] == ' ') {
			start++;
		}
		if (s[start] == '-')
		{
			k = -1;
			start++;
		}
		unsigned __int32 end;
		for (end = start; s[end] >= '0' && s[end] <= '9'; end++) {
			result = result * 10 + (s[end] - '0') * k;
		}
		while (s[end] == ' ') {
			end++;
		}
		if (s[end]) {
			result = 0;
		}
		return result;
	}

	char* stdIntToStr(char* s, __int32 val)
	{
		char temp[16];
		char* tp = temp;
		unsigned __int32 v;
		unsigned __int32 len = 0;

		bool sign = val < 0;
		if (sign)
		{
			v = -val;
			*s++ = '-';
			len++;
		}
		else {
			v = (unsigned __int32)val;
		}

		unsigned __int32 i;
		while (v || tp == temp)
		{
			i = v % 10;
			v /= 10;
			*tp++ = (char)('0' + i);
		}
		
		len += tp - temp;

		while (tp > temp) {
			*s++ = *--tp;
		}
		s[len] = '\0';
		return s;
	}

	__int32 stdPow(__int32 x, __int32 y)
	{
		return (__int32)pow(x, y);
	}

	__int32 stdSqrt(__int32 x)
	{
		return x >= 0 ? (__int32)sqrt(x) : 0;
	}

	__int32 stdStrLength(const char* s)
	{
		unsigned __int16 n = 0;
		for (auto p = s; *p; p++) {
			n++;
		}
		return n;
	}

	char* stdSubstr(char* dest, const char* from, const __int32 POS, const __int32 COUNT)
	{
		unsigned __int16 n = 0;
		if (stdStrLength(from) > POS)
		{
			for (__int32 i = POS; i < 256 && from[i] && n < COUNT; i++, n++) {
				dest[n] = from[i];
			}
		}
		dest[n] = '\0';
		return dest;
	}
}