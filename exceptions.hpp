#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

class TypeException : public std::exception {
    public:
        const char* what () {
            return "bad type given";
        }
};

class StatsException : public std::exception {
    public:
        const char* what () {
            return "bad stats alloc";
        }
};

#endif