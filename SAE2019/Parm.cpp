#include "stdafx.h"

namespace Parm
{
	PARM GetParm(int argc, wchar_t* argv[])
	{
		PARM parameters;

		bool PARM_IN_wasFound = false;
		bool PARM_OUT_wasFound = false;
		bool PARM_LOG_wasFound = false;

		wchar_t* tempParm;

		for (int i = 1; i < argc; i++)
		{
			if (argv[i] && wcslen(argv[i]) < PARM_MAX_SIZE)
			{
				if (tempParm = wcsstr(argv[i], PARM_IN))
				{
					tempParm += wcslen(PARM_IN);
					wcscpy_s(parameters.in, tempParm);
					PARM_IN_wasFound = true;
				}
				if (tempParm = wcsstr(argv[i], PARM_OUT))
				{
					tempParm += wcslen(PARM_OUT);
					wcscpy_s(parameters.out, tempParm);
					PARM_OUT_wasFound = true;
				}
				if (tempParm = wcsstr(argv[i], PARM_LOG))
				{
					tempParm += wcslen(PARM_LOG);
					wcscpy_s(parameters.log, tempParm);
					PARM_LOG_wasFound = true;
				}
			}
			else
				throw ERROR_THROW(21);//превышена длинна параметра
		}

		if (!PARM_IN_wasFound)
			throw ERROR_THROW(20);//отсутствие параметра in

		if (!PARM_OUT_wasFound)
		{
			wcscpy_s(parameters.out, parameters.in);
			wcscat_s(parameters.out, PARM_OUT_DEFAULT_EXT);
		}

		if (!PARM_LOG_wasFound)
		{
			wcscpy_s(parameters.log, parameters.in);
			wcscat_s(parameters.log, PARM_LOG_DEFAULT_EXT);
		}

		return parameters;
	}
}