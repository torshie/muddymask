#include <cctype>
#include <cstring>
#include <muddy/common/except.hpp>
#include <muddy/common/Configuration.hpp>
#include <muddy/common/ConfigParser.hpp>

using namespace muddy;

Configuration& Configuration::add(const char* name, bool mandatory,
		const char* defaultValue) {
	if (std::strcmp("config", name) == 0) {
		throw InvalidArgument("\"config\" is a special option, "
				"must not be used by clients");
	}
#if ENABLE_TRACING_CODE
	if (std::strcmp("instrument", name) == 0) {
		RAISE(InvalidArgument, "\"instrument\" is a special option,"
				" must not be used by clients");
	}
#endif

	Argument arg(mandatory);
	if (defaultValue != NULL) {
		arg.origin = kDefaultValue;
		arg.value = defaultValue;
	}

	std::string envName = std::string("muddy") + section + "_" + name;
	const char* value = std::getenv(envName.c_str());
	if (value != NULL) {
		arg.origin = kEnvironmentVariable;
		arg.value = value;
	}
	option.insert(std::make_pair(std::string(name), arg));
	return *this;
}

void Configuration::parse(int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-' && argv[i][1] == '-') {
			const char* p = argv[i] + 2;
			if (*p == '\0') {
				remaining.clear();
				for (int j = i + 1; j < argc; ++j) {
					remaining.push_back(argv[j]);
				}
				break;
			}
			Map::iterator x = option.find(p);
			if (x != option.end()) {
				x->second.origin = kCommandLine;
				if (argv[i + 1] != NULL && argv[i + 1][0] != '-') {
					x->second.value = argv[i + 1];
					++i;
				}
			} else {
				throw ConfigError("unknown option: --%s", p);
			}
		} else {
			throw ConfigError("bad string in command line: %s", argv[i]);
		}
	}
	Map::iterator i = option.find("config");
	if (i != option.end() && !i->second.value.empty()) {
		parseConfigFile(i->second.value.c_str());
	}
	for (Map::iterator i = option.begin(), e = option.end(); i != e; ++i) {
		if (i->second.mandatory && i->second.value.empty()) {
			throw ConfigError("Missing option --%s", i->first.c_str());
		}
	}
}

const char* Configuration::getString(const char* opt) const {
	Map::const_iterator i = option.find(opt);
	if (i != option.end()) {
		if (i->second.origin != kInvalidOrigin) {
			return i->second.value.c_str();
		} else {
			return NULL;
		}
	} else {
		throw Unpossible("Unknown option %s", opt);
	}
}

int Configuration::getInt(const char* opt) const {
	const char* str = getString(opt);
	int value;
	if (std::sscanf(str, "%d", &value) != 1) {
		throw ConfigError("Option --%s needs an integer argument", opt);
	}
	return value;
}

void Configuration::parseConfigFile(const char* fileName) {
	ConfigParser parser;
	parser.parse(fileName, section.c_str());
	for (Map::iterator i = option.begin(), e = option.end(); i != e; ++i) {
		if (i->second.origin < kConfigFile) {
			const char* v = parser.get(i->first.c_str());
			if (v != NULL) {
				i->second.origin = kConfigFile;
				i->second.value = v;
			}
		}
	}
	if (remaining.empty()) {
		const char* r = parser.get("--");
		if (r != NULL) {
			parseStringSequence(r);
		}
	}
}

void Configuration::parseStringSequence(const char* str) {
	const char* begin = str, *end = NULL;
	for (;;) {
		while (*begin != '\0' && std::isspace(*begin)) {
			++begin;
		}
		end = begin;
		while (*end != '\0' && !std::isspace(*end)) {
			++end;
		}
		if (begin == end) {
			break;
		}
		remaining.push_back(std::string(begin, end));
		begin = end;
	}
}
