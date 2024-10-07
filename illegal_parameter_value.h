//
//  illegal_parameter_value.h
//  linear list
//
//  Created by Mingze Lee on 2024/10/7.
//

#ifndef ILLEGAL_PARAMETER_VALUE_H_
#define ILLEGAL_PARAMETER_VALUE_H_
#include <exception>
#include <string>


class illegalParameterValue : public std::exception
{
private:
    std::string message;
public:
    illegalParameterValue() { message = "Illegal Parameter Value"; };
    illegalParameterValue(const std::string & str) { message = str; };
    const std::string & what() { return message; };
};




#endif
