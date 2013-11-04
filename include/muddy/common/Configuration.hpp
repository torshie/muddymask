#ifndef MUDDY_COMMON_CONFIGURATION_HPP_INCLUDED_
#define MUDDY_COMMON_CONFIGURATION_HPP_INCLUDED_

#include <cstdlib>
#include <list>
#include <map>
#include <string>

namespace muddy {

class Configuration {
public:
	explicit Configuration(const char* section) : section(section) {
		option.insert(
				std::make_pair(std::string("config"), Argument(false)));
	}

	Configuration& add(const char* name, bool mandatory,
			const char* defaultValue = NULL);
	void parse(int argc, char** argv);
	const char* getString(const char* opt) const;
	int getInt(const char* opt) const;

	std::list<std::string> getRemaining() const {
		return remaining;
	}

private:
	enum ValueOrigin {
		kInvalidOrigin = 0, kDefaultValue, kConfigFile,
		kEnvironmentVariable, kCommandLine
	};

	struct Argument {
		bool mandatory;
		ValueOrigin origin;
		std::string value;

		explicit Argument(bool mandatory)
				: mandatory(mandatory), origin(kInvalidOrigin) {}
	};

	typedef std::map<std::string, Argument> Map;

	std::string section;
	Map option;
	std::list<std::string> remaining;

	void parseConfigFile(const char* fileName);
	void parseStringSequence(const char* str);
};

} // namespace muddy

#endif // MUDDY_COMMON_CONFIGURATION_HPP_INCLUDED_
