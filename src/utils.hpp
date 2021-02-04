//
// Created by ythan on 2021/2/4.
//

#ifndef __UTILS_HPP__
#define __UTILS_HPP__


namespace utl
{


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


}	 // namespace utl


#endif	  // __UTILS_HPP__
