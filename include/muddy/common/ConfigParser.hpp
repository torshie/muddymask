#ifndef _MUDDY_COMMON_CONFIG_PARSER_HPP_INCLUDED_
#define _MUDDY_COMMON_CONFIG_PARSER_HPP_INCLUDED_

#include <cstdio>
#include <map>
#include <string>

namespace muddy {

class ConfigParser {
public:
	void parse(const char* fileName, const char* section);
	const char* get(const char* name) const;

private:
	typedef std::map<std::string, std::string> Map;

	Map dict;

	static char* getLine(FILE* file, char* buffer, size_t size);
	static std::string getName(const char* begin, const char* end,
			int lineNumber);
	static std::string getValue(const char* begin, const char* end,
			int lineNumber);
	static std::string parseQuotedString(const char* begin,
			const char* end, int lineNumber);
	static char getEscapedChar(char ch);

	void parseLine(char* line, size_t length, int lineNumber);
};

} // namespace muddy

#endif // _MUDDY_COMMON_CONFIG_PARSER_HPP_INCLUDED_
