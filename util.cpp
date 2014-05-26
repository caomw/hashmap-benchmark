#include "util.h"

#include <cassert>
#include <fstream>


#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

std::string getcwd()
{
    const int kBufSize = 8192;
    char buf[kBufSize];
    buf[0] = 0;
#ifdef _WIN32
    _getcwd(buf, kBufSize);
#else
    getcwd(buf, kBufSize);
#endif
    return buf;
}

void replace_string(std::string& s, const std::string& searchString, const std::string& replaceString)
{
	size_t spos = s.find(searchString);
	assert(spos != std::string::npos);
	s = s.substr(0, spos) + replaceString + s.substr(spos + searchString.size());
}

void save_result(const char* rel_fname, const char* data)
{
	std::ofstream ofs(rel_fname);
	ofs << data;
    
    printf("Results saved to: %s/%s\n", getcwd().c_str(), rel_fname);
}
