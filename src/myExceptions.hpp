//
// Created by ythan on 2021/2/3.
//

#include <exception>


#ifndef __MYEXCEPTIONS_HPP__
#define __MYEXCEPTIONS_HPP__


namespace mex
{


class exceptionIllFormYesNo : public std::exception
{
public:
    [[nodiscard]] const char* what() const noexcept override
    {
        return "Must be y yes n no in upper or lower cases.";
    }
};


class exceptionIllFormCommand : public std::exception
{
public:
    [[nodiscard]] const char* what() const noexcept override
    {
        return "Unknown command.";
    }
};


}    // namespace mex


#endif    // __MYEXCEPTIONS_HPP__
