#include "exo.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#include <fstream>

using namespace exo::data;

struct impl
{

};

bool Config::Value::is_new()
{
    std::ifstream fs(path);

    return fs.bad();
}

static void r_mkdir(const char *dir) {
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);

    if(tmp[len - 1] == '/') tmp[len - 1] = 0;

    for(p = tmp + 1; *p; p++)
    {
        if(*p == '/')
        {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    }

    mkdir(tmp, S_IRWXU);
}

void Config::Value::operator=(std::string value)
{
    std::fstream fs;

    r_mkdir(path);
    fs.open(path, std::ios::out | std::ios::trunc);
    fs << value;
    fs.close();
}

std::string Config::Value::operator()()
{
    std::ifstream fs(path);

    if (fs.bad()) return "";

    std::string str((std::istreambuf_iterator<char>(fs)),
                     std::istreambuf_iterator<char>());

    return str;
}
