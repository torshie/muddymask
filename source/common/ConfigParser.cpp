#include <cctype>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <muddy/common/compat/memory>
#include <muddy/common/except.hpp>
#include <muddy/common/wrapper/wrapper.hpp>
#include <muddy/common/ConfigParser.hpp>

using namespace muddy;

void ConfigParser::parse(const char* fileName, const char* section) {
	dict.clear();
	FILE* file = wrapper::fopen_(fileName, "r");
	bool gotSection = false;
	char buffer[2048];
	int lineNumber = 1;
	for (char* line = getLine(file, buffer, sizeof(buffer)); line != NULL;
			line = getLine(file, buffer, sizeof(buffer)), ++lineNumber) {
		while (*line != '\0' && std::isspace(*line)) {
			++line;
		}
		if (*line == '\0' || line[0] == '#') {
			continue;
		}
		size_t length = std::strlen(line);
		char* e = line + length;
		while (std::isspace(e[-1])) {
			--e;
		}
		*e = '\0';

		if (line[0] == '[' && line[length - 1] == ']') {
			if (std::memcmp(line + 1, section, length - 2) == 0) {
				gotSection = true;
				continue;
			} else {
				if (gotSection) {
					break;
				}
			}
		}
		if (!gotSection) {
			continue;
		}
		parseLine(line, length, lineNumber);
	}
	if (!gotSection) {
		throw ConfigError("cannot find section \"%s\" in file %s",
				section, fileName);
	}
	wrapper::fclose_(file);
}

const char* ConfigParser::get(const char* name) const {
	Map::const_iterator i = dict.find(name);
	if (i == dict.end()) {
		return NULL;
	}
	return i->second.c_str();
}

char* ConfigParser::getLine(FILE* file, char* buffer, size_t size) {
	if (std::fgets(buffer, size, file) == NULL) {
		return NULL;
	}
	size_t length = std::strlen(buffer);
	if (length == size - 1 && buffer[length - 1] != '\n') {
		throw LineTooLong();
	}
	char* first = buffer;
	while (std::isspace(*first)) {
		++first;
	}
	char* last = buffer + length;
	while (last > first && std::isspace(last[-1])) {
		--last;
	}
	*last = '\0';
	return first;
}

void ConfigParser::parseLine(char* line, size_t length, int lineNumber) {
	char* equal = std::strchr(line, '=');
	if (equal == NULL) {
		throw ConfigError("invalid line(%d) in config file", lineNumber);
	}
	std::string name = getName(line, equal, lineNumber);
	std::string value = getValue(equal + 1, line + length, lineNumber);
	dict.insert(std::make_pair(name, value));
}

std::string ConfigParser::getName(const char* begin, const char* end,
		int lineNumber) {
	while (std::isspace(*begin) && begin < end) {
		++begin;
	}
	if (begin == end) {
		throw ConfigError("expected variable name at the "
				"beginning of line %d", lineNumber);
	}
	while (end > begin && std::isspace(end[-1])) {
		--end;
	}

	if (!((*begin >= 'a' && *begin <= 'z')
			|| (*begin >= 'A' && *begin <= 'Z')
			|| (end - begin == 2 && *begin == '-' && begin[1] == '-'))) {
		throw ConfigError("invalid variable name at line %d", lineNumber);
	}
	for (const char* p = begin + 1; p < end; ++p) {
		if (!((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')
				|| (*p >= '0' && *p <= '9')
				|| (end - begin == 2 && *begin == '-'
						&& begin[1] == '-'))) {
			throw ConfigError("invaid variable name at line %d",
					lineNumber);
		}
	}
	return std::string(begin, end);
}

std::string ConfigParser::getValue(const char* begin, const char* end,
		int lineNumber) {
	while (std::isspace(*begin) && begin < end) {
		++begin;
	}
	if (begin == end) {
		throw ConfigError("expected variable value after equal(=) "
				"sign at line %d.", lineNumber);
	}
	while (end > begin && std::isspace(end[-1])) {
		--end;
	}
	if (*begin == '"') {
		if (*end == '"' && end > begin + 1) {
			return parseQuotedString(begin, end, lineNumber);
		} else {
			throw ConfigError("invalid variable value at line %d",
					lineNumber);
		}
	} else {
		return std::string(begin, end);
	}
}

std::string ConfigParser::parseQuotedString(const char* begin,
		const char* end, int lineNumber) {
	std::tr1::shared_ptr<char> buffer((char*)malloc(end - begin + 1),
			std::free);
	int index = 0;
	for (const char* p = begin + 1; p < end - 1; ++p) {
		char ch = *p;
		if (*p == '\\') {
			if (p == end - 2) {
				throw ConfigError("invalid escape sequence at line %d",
						lineNumber);
			}
			++p;
			ch = getEscapedChar(*p);
		} else if (*p == '"') {
			throw ConfigError("invalid string value at line %d",
					lineNumber);
		}
		buffer.get()[index++] = ch;
	}
	buffer.get()[index] = '\0';
	return std::string(buffer.get(), buffer.get() + index);
}

char ConfigParser::getEscapedChar(char ch) {
	switch (ch) {
	case 'a': return '\a';
	case 'b': return '\b';
	case 'f': return '\f';
	case 'n': return '\n';
	case 'r': return '\r';
	case 't': return '\t';
	case '"': return '"';
	case 'v': return '\v';
	case '\\': return '\\';
	default: return ch;
	}
}
