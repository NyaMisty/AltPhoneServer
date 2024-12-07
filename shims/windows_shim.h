#define _chmod chmod
#define _close close
#define _open ::open
#define _stat stat
#define _fstat fstat
#define _access access
#define _mkdir(x) mkdir(x, 0777)

char *_itoa(int val, char *buf, int base);

#include <limits.h>

#define __declspec(x)

#define OutputDebugStringW(x) (std::wcout << x)
#define OutputDebugStringA(x) (std::cout << x)

typedef struct timeval TIMEVAL;

#include <string.h>

#ifdef __cplusplus
#include <algorithm>
#include <memory>
#include <string>
extern std::string StringFromWideString(std::string wideString);
extern std::string WideStringFromString(std::string string);
#endif

#define Sleep(x) sleep((int)(x/1000))
