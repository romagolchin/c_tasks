#ifndef ITMO_CPP_FORMAT_H
#define ITMO_CPP_FORMAT_H

#include <limits>
#include <cstdint>
#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <exception>
#include <stdexcept>
#include <stdio.h>
#include <cxxabi.h>

#define DEBUG 1
#undef DEBUG
using namespace std;

const int buf_sz = 512;

typedef long long ll;
typedef unsigned long long ull;

// enum class Specifier {none};
// enum class Length {none};
enum class Width {number, star, none};
// enum class Kind {integer, floating, pointer};
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
		alt = left = sign = zero = space = width_num = 0;
		prec_num = 1;
		width = Width::none;
		precision = Precision::none;
		spec = '\0';
		len = "";
	}
};

template <class T>
T to_c_str(T t) {
	return t;
}

void normalize_props(format_properties& props);

bool parse_format(const string &fmt, string &result, int &num_stars, format_properties &properties, string &fmt_tail);
const char *to_c_str(string);


template <class To, class From>
typename enable_if<is_convertible<From, To>::value, To>::type
cast(From value) {
	return value;
}

template <class To, class From>
typename enable_if<!is_convertible<From, To>::value, To>::type
cast(From value) {
	throw invalid_argument("implicit conversion is not possible");
}

string format(const string &fmt);


template <class Arg, class... Args>
string format(const string &fmt, Arg arg, Args... args) {
	string res, tail;
	int n_stars;
	format_properties props;
	bool arg_expected = parse_format(fmt, res, n_stars, props, tail);
	if(arg_expected) {
		if(n_stars == 0)
			res += format_impl(props, tail, arg, args...);
		else
			throw out_of_range("not enough arguments");
	} 
	return res;
}

template <class Arg, class Arg1, class... Args>
string format(const string &fmt, Arg arg, Arg1 arg1, Args... args) {
	string res, tail;
	int n_stars;
	format_properties props;
	if(parse_format(fmt, res, n_stars, props, tail)) { 
		if(n_stars == 1)
			res += format_impl_1(props, tail, arg, arg1, args...);
		else if(n_stars == 0)
			res += format_impl(props, tail, arg, arg1, args...);
		else
			throw out_of_range("not enough arguments");
	}
	return res;
}

template <class Arg, class Arg1, class Arg2, class... Args>
string format(const string &fmt, Arg arg, Arg1 arg1, Arg2 arg2, Args... args) {
	string res, tail;
	int n_stars;
	format_properties props;
	bool arg_expected = parse_format(fmt, res, n_stars, props, tail);
	if(arg_expected)  {
		if(n_stars == 2)
			res += format_impl_2(props, tail, arg, arg1, arg2, args...);
		else if(n_stars == 1)
			res += format_impl_1(props, tail, arg, arg1, arg2, args...);
		else if(n_stars == 0)
			res += format_impl(props, tail, arg, arg1, arg2, args...);
	}
	return res;
}



template <class Arg, class... Args>
string format_impl(format_properties& props, string const &tail, Arg arg, Args... args) {
	string res;
	normalize_props(props);
	string fmt = "%";
	if(props.alt) fmt += '#';
	if(props.sign) fmt += '+';
	if(props.space) fmt += ' ';
	fmt += props.len + props.spec;
	char* buf = new char[buf_sz];
	size_t num_chars;
	switch (tolower(props.spec)) {
		case 'd': case 'i': case 'c': {
			ll new_arg = cast<ll>(arg);
			if(new_arg == 0 && props.prec_num == 0) {
				res = "";
				break;
			}
			num_chars = snprintf(buf, buf_sz, fmt.c_str(), new_arg);
			res = string(buf);
			break; 
		}
		case 'x': case 'o': case 'u': {
			ull new_arg = cast<ull>(arg);
			num_chars = snprintf(buf, buf_sz, fmt.c_str(), new_arg);
			res = string(buf);
			break;
		}
		case 'f': case 'g': case 'e': case 'a': {
			double new_arg = cast<double>(arg);
			#ifdef DEBUG
			//cerr << "new_arg = " << new_arg << endl;
			#endif
			num_chars = snprintf(buf, buf_sz, fmt.c_str(), new_arg);
			res = string(buf);
			int actual_prec = std::numeric_limits<long double>::digits10;
			if(props.precision != Precision::none && props.prec_num > actual_prec) {
				size_t exp_ind = res.find_first_of("eEpP");
				string zeros = string(props.prec_num - actual_prec, '0');
				if(exp_ind == string::npos) {
					res += zeros;
					break;
				}
				string old_res = res;
				//cerr << old_res << endl;
				res = old_res.substr(0, exp_ind) + zeros + old_res.substr(exp_ind);
			}
			break; 
		}
		case 'p': {
			if(!is_pointer<Arg>::value && !is_same<Arg, nullptr_t>::value)
				throw invalid_argument("expected pointer argument");
			num_chars = snprintf(buf, buf_sz, fmt.c_str(), arg);
			res = string(buf);
			break;
		}
		case 's': {
			int status;
			const std::type_info  &ti = typeid(to_c_str(arg));
			char *realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
			//cout << realname << endl;
			const char* new_arg = cast<const char*>(to_c_str(arg));
			if(new_arg == nullptr) {
				res = "<null>";
				break;
			}
			if(new_arg)
				res = string(new_arg);
			if(props.precision != Precision::none) {
				//cerr << props.prec_num << ' ' << res.length() << endl;
				res = res.substr(0, min(props.prec_num, (int) res.length()));
			}
			num_chars = res.length();
		}
	}
	char to_fill = props.zero ? '0' : ' ';
	string fill = string(max(0ll, (ll) props.width_num - (ll) num_chars), to_fill);
	res = props.left ? res + fill : fill + res;
	delete [] buf;
	return res + format(tail, args...);
}

template <class Num, class Head, class... Tail>
typename enable_if<is_same<Num, int>::value, string>::type format_impl_1(format_properties &props, string const &fmt, Num num, Head h, Tail... t) {
	if(props.width == Width::star) {
		props.width_num = num;
	}
	if(props.precision == Precision::star) {
		props.prec_num = num;
	}
	return format_impl(props, fmt, h, t...);
}

template <class Num, class Head, class... Tail>
typename enable_if<!is_same<Num, int>::value, string>::type
format_impl_1(format_properties &props, string const &fmt, Num num, Head h, Tail... t) {
	if(props.width == Width::star) 
		throw invalid_argument("width is not integer");	
	if(props.precision == Precision::star)
		throw invalid_argument("precision is not integer");
}

template <class W, class P, class Head, class... Tail>
typename enable_if<is_same<W, int>::value && is_same<P, int>::value, string>::type
format_impl_2(format_properties &props, string const &fmt, W width, P precision, Head h, Tail... t) {		
	//cerr << __PRETTY_FUNCTION__ << endl;
	props.width_num = width;	
	props.prec_num = precision;
	return format_impl(props, fmt, h, t...);
}

template <class W, class P, class Head, class... Tail>
typename enable_if<!is_same<W, int>::value || !is_same<P, int>::value, string>::type
format_impl_2(format_properties &props, string const &fmt, W width, P precision, Head h, Tail... t) {
	throw invalid_argument("either width or precision is not integer");
}

#endif