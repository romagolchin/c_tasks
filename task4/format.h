#include <cstdint>
#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <exception>

using namespace std;

enum class Specifier {none};
enum class Length {none};
enum class Width {number, arg, none};
enum class Kind {integer, floating, pointer};
typedef Width Precision;
struct format_properties {
	//flags
	bool alt;
	bool left;
	bool sign;
	bool zero;
	bool space;
	//width & precision
	Width width;
	int width_num;
	Precision precision;
	int prec_num;
	//length modifier, specifier
	char spec;
	string len;
	format_properties() {
		alt = left = sign = zero = space = width_num = prec_num = 0;
		width = Width::none;
		precision = Precision::none;
		spec = '\0';
		len = "";
	}
};

template <class From, class To>
typename enable_if<is_convertible<From, To>::value, To>::type
cast(From value);

template <class From, class To>
typename enable_if<!is_convertible<From, To>::value, To>::type
cast(From value);


template <class... Args>
string format(const string &fmt, Args... args);

string format_impl(string const& fmt);

template <class Arg, class... Args>
string format_impl(format_properties&, string const &fmt, Arg arg, Args... args);

template <class... Args>
string format_impl(format_properties&, string const &fmt, Args... args) {
	throw out_of_range("not enough arguments");
}

template <class W, class Head, class... Tail>
string format_impl_1(format_properties&, string const&, W width, Head, Tail...) {return "";}

template <class W, class P, class Head, class... Tail>
string format_impl_2(format_properties&, string const&, W width, P precision, Head h, Tail... t) {return "";}

template <class Head, class... Tail>
string format_impl_1(format_properties&, string const&, Head, Tail...) {
	throw out_of_range("not enough arguments 0");
}

template <class... Tail>
string format_impl_1(format_properties&, string const&, Tail...){
	throw out_of_range("not enough arguments 1");
}

template <class P, class Head, class... Tail>
string format_impl_2(format_properties&, string const&, P precision, Head h, Tail... t){
	throw out_of_range("not enough arguments 2");
}

template <class Head, class... Tail>
string format_impl_2(format_properties&, string const&, Head h, Tail... t){
	throw out_of_range("not enough arguments 3");
}

template <class... Tail>
string format_impl_2(format_properties&, string const&, Tail...){
	throw out_of_range("not enough arguments 4");
}

void normalize_flags(format_properties& props);

bool valid_format(const string &format);


// class bad_format: public std::exception {};
