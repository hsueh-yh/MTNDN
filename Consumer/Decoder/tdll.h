#ifndef _TDLL_
#define _TDLL_

#include <Windows.h>
#include <iostream>
using namespace std;

class Tdll
{
private:
	HMODULE hdll;
	void loadDll(const char *dllName);
public:
	bool ok;
	Tdll(const TCHAR *dllName1)
	{
		hdll = LoadLibrary(dllName1);
		if (!hdll)
		{
			hdll = NULL;
		}
		ok = (hdll != NULL);
	};

	~Tdll()
	{
		if (hdll)
			FreeLibrary(hdll);
	}

	void loadFunction(void **fnc, const char *name)
	{
		*fnc = GetProcAddress(hdll, name);
		//cout << *fnc << endl;
		ok &= (*fnc != NULL);
	}
};

#endif
