#ifndef _MUDDY_COMMON_CONFIGURATION_HPP_INCLUDED_
#define _MUDDY_COMMON_CONFIGURATION_HPP_INCLUDED_

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
	struct Argument {
		bool mandatory;
		bool valueGiven;
		std::string value;

		explicit Argument(bool mandatory)
				: mandatory(mandatory), valueGiven(false) {}
	};

	typedef std::map<std::string, Argument> Map;

	std::string section;
	Map option;
	std::list<std::string> remaining;

	void parseConfigFile(const char* fileName);
	void parseStringSequence(const char* str);
};

} // namespace muddy

#endif // _MUDDY_COMMON_CONFIGURATION_HPP_INCLUDED_
