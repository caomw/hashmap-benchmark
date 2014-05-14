#include "util.h"

#include <cassert>

void replace_string(std::string& s, const std::string& searchString, const std::string& replaceString)
{
	size_t spos = s.find(searchString);
	assert(spos != std::string::npos);
	s = s.substr(0, spos) + replaceString + s.substr(spos + searchString.size());
}
