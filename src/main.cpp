#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "cmakeConfig.hpp"
#include "myExceptions.hpp"
#include <pthread/pstream.h>


struct appInfo
{
	std::string name;
	std::string endFile;
	std::string sourceFile;
};


bool getYesNo(std::string str)
{
	for (auto& ele : str) { ele = std::tolower(ele); }

	if (str == "yes" || str == "y")
	{
		return true;
	}
	else if (str == "no" || str == "n")
	{
		return false;
	}
	else
	{
		throw mex::exceptionIllFormYesNo();
	}
}

int main(int argc, char* argv[])
{
	// go through all command line arguments
	for (int i = 0; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			if (strcmp(argv[i], "-v") == 0)
			{
				std::cout << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_MAJOR << "."
						  << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_MINOR << "."
						  << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_PATCH << "."
						  << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_TWEAK << std::endl;

				return 0;
			}
			else if (strcmp(argv[i], "--version") == 0)
			{
				std::cout << "make-alternative-versions version: " << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_MAJOR
						  << "." << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_MINOR << "."
						  << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_PATCH << "."
						  << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_TWEAK << " "
						  << "Build: " << CFG_BUILD_TYPE << std::endl;

				return 0;
			}
			else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			{
				std::cout << "Provide:\n"
							 "    name of the application,\n"
							 "    where the link file should be(end file),\n"
							 "    where the link file should point to(source "
							 "file).\n\n"
							 "If there are other associated file that should "
							 "also be changed\n"
							 " enter \"Y\" or \"yes\" when asked if have "
							 "slave(default no).\n"
							 " Then provide end files and source files "
							 "location when prompted.\n";

				return 0;
			}
			else
			{
				throw mex::exceptionIllFormCommand();
			}
		}
	}

	appInfo		mainApp;
	std::string appPriority;

	std::string haveSlaveStr;
	bool		haveSlave;

	std::vector<appInfo> slaveApps;

	std::cout << "Name of app: ";
	getline(std::cin, mainApp.name);

	std::cout << "End file: ";
	getline(std::cin, mainApp.endFile);

	std::cout << "Source file: ";
	getline(std::cin, mainApp.sourceFile);

	std::cout << "App priority: ";
	getline(std::cin, appPriority);


	while (true)
	{
		try
		{
			std::cout << "Have slave?(Yes/[No]): ";
			getline(std::cin, haveSlaveStr);

			haveSlave = getYesNo(haveSlaveStr);
		}
		catch (mex::exceptionIllFormYesNo& e)
		{
			std::cout << "Please enter yes or no.\n";
			continue;
		}

		if (haveSlave)
		{
			slaveApps.emplace_back();
			std::cout << "Name of slave: ";
			getline(std::cin, slaveApps.back().name);

			std::cout << "End file: ";
			getline(std::cin, slaveApps.back().endFile);

			std::cout << "Source file: ";
			getline(std::cin, slaveApps.back().sourceFile);
		}
		else
		{
			break;
		}
	}

	// Use info to run update-alternatives
	std::string commandString = "update-alternatives --install ";
	commandString.append(mainApp.endFile)
		.append(" ")
		.append(mainApp.name)
		.append(" ")
		.append(mainApp.sourceFile)
		.append(" ")
		.append(appPriority);

	for (const auto& ele : slaveApps)
	{
		commandString.append(" --slave ")
			.append(ele.endFile)
			.append(" ")
			.append(ele.name)
			.append(" ")
			.append(ele.sourceFile);
	}

	redi::ipstream proc(commandString, redi::pstream::pstdout | redi::pstream::pstderr);

	std::string line;
	// read child's stdout
	while (std::getline(proc.out(), line))
	{
		std::cout << "stdout: " << line << '\n';
	}
	// if reading stdout stopped at EOF then reset the state:
	if (proc.eof() && proc.fail())
	{
		std::cout << "make app failed.";
		proc.clear();
	}
	// read child's stderr
	while (std::getline(proc.err(), line))
	{
		std::cout << "stderr: " << line << '\n';
	}
	
	std::cout << "make app versions done." << std::endl;

	return 0;
}
