#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "cmakeConfig.hpp"
#include "myExceptions.hpp"
#include "utils.hpp"
#include <pthread/pstream.h>



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
						  << "." << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_MINOR
						  << "." << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_PATCH
						  << "." << CFG_MAKE_ALTERNATIVE_VERSIONS_VERSION_TWEAK << " "
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

	std::string commandStringPreview = "update-alternatives --install ";

	appInfo master;

	std::vector<appInfo> slaves;

	std::cout << "Name of App: ";
	getline(std::cin, master.name);

	std::cout << "Master link target: ";
	getline(std::cin, master.linkTarget);


	while (utl::quarryInputYesNo("Have slave?(Yes/No): "))
	{
		slaves.emplace_back();
		std::cout << "Name of slave: ";
		getline(std::cin, slaves.back().name);

		std::cout << "Slave link target: ";
		getline(std::cin, slaves.back().linkTarget);
	}

	std::cout << "Command preview: \n";
	utl::alternativesCommandCoutPreview(master, slaves);

	const size_t						  slavesSize = slaves.size();
	std::vector<std::string>			  masterSources;
	std::vector<std::string>			  appPriority;
	std::vector<std::vector<std::string>> slavesSources;

	while (utl::quarryInputYesNo("Add new version?(Yes/No): "))
	{
		masterSources.emplace_back();
		appPriority.emplace_back();
		slavesSources.emplace_back(slavesSize);

		std::cout << "App priority: ";
		getline(std::cin, appPriority.back());

		std::cout << "Master source file: ";
		getline(std::cin, masterSources.back());

		for (size_t i = 0; i < slavesSize; ++i)
		{
			std::cout << "Slave source file for " << slaves[i].name << ": ";
			getline(std::cin, slavesSources.back()[i]);
		}

		std::cout << "Command preview:\n";
		utl::alternativesCommandCoutPreview(master,
											slaves,
											appPriority.back(),
											masterSources.back(),
											slavesSources.back());
	}

	// Use info to run update-alternatives

	std::string commandString;
	std::string line;
	
	const size_t appSize = appPriority.size();
	
	for(size_t i = 0; i < appSize; ++i)
	{
		commandString = "update-alternatives --install ";
		commandString.append(master.linkTarget)
			.append(" ")
			.append(master.name)
			.append(" ")
			.append(masterSources[i])
			.append(" ")
			.append(appPriority[i]);
		
		for (size_t j = 0; j < slavesSize; ++j)
		{
			commandString.append(" --slave ")
				.append(slaves[j].linkTarget)
				.append(" ")
				.append(slaves[j].name)
				.append(" ")
				.append(slavesSources[i][j]);
		}

		redi::ipstream proc(commandString, redi::pstream::pstdout | redi::pstream::pstderr);

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
	}

	std::cout << "make app versions done." << std::endl;

	return 0;
}
