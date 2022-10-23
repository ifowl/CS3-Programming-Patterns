//Lab 8: Part One - Meyer's singleton example that appends strings to a log file
//Isaac Fowler
//
//singleton.cpp


#include <fstream>
#include <string>
#include <cstdlib>

using std::string;

class Logger {
public:
	void report(const string&);

	//returns reference to access object:
	static Logger &instance() {
		static Logger s;
		return s;
	}
private:
	//prohibit creation and copying
	Logger(){}
	Logger(const Logger&){}
	Logger& operator=(const Logger&){}
};

void Logger::report(const string &str) {
	std::ofstream fout;
	fout.open("log.txt", std::fstream::out | std::fstream::app);
	if (fout.is_open()) {
		fout << str << std::endl;
	}
	fout.close();
}

//Demonstrate the operation of your object by invoking report() in at least two separate functions:

void invokeReport(const string &str) {
	Logger::instance().report(str);
}

void invokeReportRand() {
	string random;
	for (int i = 0; i < 10; ++i)
		random += 'a' + rand()%26;
	Logger::instance().report(random);
}

int main () {
	string line = "This is a sample line.";
	Logger::instance().report(line);
	string line2 = "Another line.";
	invokeReport(line2);
	invokeReportRand();
}
