//
// Created by ythan on 2021/2/4.
//

#ifndef __UTILS_HPP__
#define __UTILS_HPP__


#include <string>
#include <vector>


struct appInfo
{
    std::string name;
    std::string linkTarget;
};


namespace utl
{


int8_t getYesNo(std::string str)
{
    for (auto& ele : str) { ele = std::tolower(ele); }

    if (str == "yes" || str == "y")
    {
        return 1;
    }
    else if (str == "no" || str == "n")
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


bool quarryInputYesNo(std::string question)
{
    std::string input;
    while (true)
    {
        std::cout << question;
        getline(std::cin, input);

        auto result = utl::getYesNo(input);

        if (result > 0)
        {
            return true;
        }
        else if (result < 0)
        {
            return false;
        }
    }
}


void alternativesCommandCoutPreview(
    const appInfo&                  master,
    const std::vector<appInfo>&     slaves,
    const std::string&              priority         = R"("Priority")",
    const std::string&              masterSourceFile = R"("masterSourceFile")",
    const std::vector<std::string>& slavesSourceFiles = std::vector<std::string>())
{
    std::cout << "make-alternatives --install " << master.linkTarget << " "
              << master.name << " " << masterSourceFile << " " << priority;

    if (slavesSourceFiles.empty())
    {
        for (const auto& ele : slaves)
        {
            std::cout << " --slave " << ele.linkTarget << " " << ele.name
                      << R"( "slaveSourceFile")";
        }
    }
    else
    {
        const size_t slavesSize = slaves.size();
        for (size_t i = 0; i < slavesSize; ++i)
        {
            std::cout << " --slave " << slaves[i].linkTarget << " "
                      << slaves[i].name << " " << slavesSourceFiles[i];
        }
    }

    std::cout << std::endl;
}


}    // namespace utl


#endif    // __UTILS_HPP__
