#include "OtokonokoPlatformRuntime.h"

#include <cstdio>
#include <string>

void Write(const char* String)
{
    printf("%s", String);
}

void WriteLine(const char* String)
{
    printf("%s\n", String);
}
