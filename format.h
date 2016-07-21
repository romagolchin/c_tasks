#include <cstdint>
#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <exception>

using namespace std;

template <class... Args>
string format(string const &fmt, Args... args);

//pre: fmt has form %[length modifier]<specifier>
template <class Head, class... Tail>
string format_call_1(string const& fmt, Head h, Tail... t);
//pre: %*[lm]<spec>
template <class W, class Head, class... Tail>
string format_call_2(string const&, size_t, W, Head, Tail...);
//%.*[lm]<spec>
template <class Head, class... Tail>
string format_call_3(string const& fmt, P precision, Head h, Tail... t);
//%*.*[lm]<spec>
template <class Head, class... Tail>
string format_call_4(string const& fmt, W width, P precision, Head h, Tail... t);

template<class T>
bool correct_type(const char c, const T t);

// class bad_format: public std::exception {}; 
