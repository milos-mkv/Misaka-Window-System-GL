#ifndef MISAKA_TYPES_H
#define MISAKA_TYPES_H

#include <string>
#include <memory>
#include <map>
#include <vector>

#define TRUE true
#define FALSE false

#define CreatePTR std::make_shared

namespace Misaka
{
    typedef std::string         STR;
    typedef const std::string   CSTR;
    typedef int                 INT;
    typedef bool                BOOL;
    typedef float               FLOAT;
    typedef double              DOUBLE;
    typedef char                CHAR;
    typedef void                VOID;
    typedef std::exception      EXCEPTION;

    template <typename T> 
    using PTR = std::shared_ptr<T>;
    template <typename T1, typename T2> 
    using MAP = std::map<T1, T2>;
    template <typename T> 
    using VEC = std::vector<T>;
}

#endif