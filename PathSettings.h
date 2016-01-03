#ifndef PATH_SETTINGS_H
#define PATH_SETTINGS_H
#include<string>

using namespace std;

class PathSettings
{
public:
	PathSettings();
	~PathSettings();

	const string& getPath(void) const;
	void setPath(string& path){ m_Path = path; }
	void setUserDataPath(void);
	string getTimestamp(void) const;

private:
	string m_Path;
};

#endif