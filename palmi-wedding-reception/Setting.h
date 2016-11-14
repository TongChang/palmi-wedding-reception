#ifndef __Setting
#define __Setting

#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/PropertyFileConfiguration.h>

using namespace std;

class Setting {
public:

	std::string welcomeSpeech;

	Setting() {}
	~Setting() {}

	void setDefault() {
		welcomeSpeech = "";
	}

	void load(std::string& filePath) {
		Poco::AutoPtr<Poco::Util::AbstractConfiguration> settings(new Poco::Util::PropertyFileConfiguration(filePath));
		string empty = "";

		welcomeSpeech = settings->getString(string("welcomeSpeech"), string(""));
	}
};

#endif