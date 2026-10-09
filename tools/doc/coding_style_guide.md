# Nau Tools Coding Style Guide

## Introduction
This is the Nau Tools C++ coding style guide used when writing and reviewing code. Please refer to [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) for issues not covered in this guide. Any suggestions and additions are welcome.

## General
### Spaces vs Tabs
Use only spaces, and indent 4 spaces at a time.

### C++ standard
Use C++ 20.

### STL
Use std.
In some cases Nau Engine solutions must be used.

### Exceptions
Exceptions might be used.

### Smart pointers
In general case, use smart pointers from std. In individual cases (e.g. when performance is critical, etc.) it is acceptable to use raw pointers.

### Casting
In general, don't use C-style casts. Instead, use C++-style casts.

### Using auto
In general, you should avoid using auto and write full data types. You can use auto to shorten very long type names. For example, for iterators or chrono.
```cpp
// without auto
std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
// with auto
auto now = std::chrono::system_clock::now();
```

### Using namespaces
Place code in a namespace (possible exceptions). Namespaces should have unique names based on the project name, and possibly its path.
When definitions in a source file do not need to be referenced outside that file, give them internal linkage by placing them in an unnamed namespace. Do not use **static** for internal linkage. Do not use internal linkage in header files.
Do not use a using-directive to make all names from a namespace available.
```cpp
// Forbidden
using namespace foo;
```

### using vs typedef
Prefer **using** over **typedef**.

### Statements and curly braces
Always put curly braces for a single-line controll statements (if, while, foreach etc.).
Use [Allman style](https://en.wikipedia.org/wiki/Indentation_style#Allman_style) for curly braces.
```cpp
while (!isDone())
{
    doSomething();
}
```

### Lambda expressions
Do not overuse lambda expressions. Use them where appropriate. Prefer explicit captures if the lambda extends beyond the current scope.

### Functions length
Prefer small and focused functions. Long functions are sometimes appropriate, so no hard limit is placed on functions length. Use common sense.

### Line length
There is no hard limit is placed on line length. Use common sense.

### Number of function arguments
Avoid a large number of function arguments. If there are more than 5 arguments, you should think about combining them into a structure if possible. In some cases, a large number of arguments may be appropriate.

### Documenting source code
Use the following doxygen style to comment your code:
```
/**
 * @brief
 * @param arg_a
 * @param arg_b
 * @return
 */
float func(int arg_a, int arg_b)
```

## Naming conventions
Prefer readability over length. Don't use short, cryptic names.

### Files
Use snake_case for files names

### Namespaces, classes, functions etc.
```cpp
// .hpp
// use UPPER_CASE for macro definitions
// prefix macro definitions with NAU
#define NAU_SOME_DEF 1

// use snake_case for namespaces
namespace common_utils
{
    // use the prefix "g_" for global varables
    int g_globalVar = 0;

    // use camelCase for functions
    void globalFunc()
    {
    }

    // put "&" and "*" as part of a type, not a name.
    void globalFunc(const std::string& someString,  SomeClass* someClassPtr)
    {
    }
}

//use PascalCase for class names
class SomeClass
{
// constants and enums go first
public:
    // use UPPER_CASE for constants
    // prefer "constexpr" over "const" for constants 
    constexpr int SOME_CONST = 1;

public:
    // prefer "enum class" over "enum"
    // use PascalCase for enumerations
    enum class Enumeration
    {
    // use camelCase for enum's cases
        enumCase1 = 0,
        enumCase2
    };

public:
    //Use rule of five
    SomeClass();
    SomeClass(const SomeClass&) = delete;
    SomeClass(SomeClass&&) = delete;
    SomeClass& operator=(const SomeClass&) = delete;
    SomeClass& operator=(SomeClass&&) = delete;

    virtual ~SomeClass() = default;

public:
    // use camelCase for methods
    void publicFunc();

// protected functions follow public ones
protected:
    void protectedFunc();

// private functions follow protected ones
private:
    void privateFunc();

private:
    // use camelCase for variables
    // use the prefix "s_" for static varables 
    static int s_staticVar;

// member variables follow static ones
private:
    // use the prefix "m_" for memebr varables
    int m_memberVarX = 0;
    int m_memberVarY = 0;
    int m_memberVarZ = 0;
};

// use the prefix "I" for interfaces (contain pure virtual fucntions only)
class ISomeInterface
{
public:
    virtual void publicFunc() = 0;
};

// use struct for POD type
struct PodStruct
{
    // don't use prefix for POD type's variables
    int var = 0;
};

// as an exception, you can omit the prefix for public variables of some types
struct Point
{
    void publicFunc();

    int x = 0;
    int y = 0;
};

// use prefix "T" for template's params types
template<typename TParamType>
class TemplateClass
{
};
```
```cpp
// .cpp
// In the general case, put the implementation in .cpp files
// Use the following initializer list style
SomeClass::SomeClass(int x, int y, int z) :
    m_memberVarX(x),
    m_memberVarY(y),
    m_memberVarZ(z)
{
}

```

## Final notes
- Cover everything with tests.
- Reduce template usage to minimum.
- Follow [const-correctness](https://isocpp.org/wiki/faq/const-correctness).
- Prefer aggregation/composition over inheritance.
- Use **#pragma once** instead of include guards.
