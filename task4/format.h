#include <limits>
#include <cstdint>
#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <exception>

using namespace std;

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

template <class To, class From>
typename enable_if<is_convertible<From, To>::value, To>::type
cast(From value);

template <class To, class From>
typename enable_if<!is_convertible<From, To>::value, To>::type
cast(From value);


string format(const string &fmt);

template <class Arg, class... Args>
string format(const string &fmt, Arg arg, Args... args);

template <class Arg, class Arg1, class... Args>
string format(const string &fmt, Arg arg, Arg1 arg1, Args... args);

template <class Arg, class Arg1, class Arg2, class... Args>
string format(const string &fmt, Arg arg, Arg1 arg1, Arg2 arg2, Args... args);

template <class Arg, class... Args>
string format_impl(format_properties&, string const &fmt, Arg arg, Args... args);

template <class W, class Head, class... Tail>
string format_impl_1(format_properties&, string const&, W width, Head, Tail...);

template <class W, class P, class Head, class... Tail>
string format_impl_2(format_properties&, string const&, W width, P precision, Head h, Tail... t);

void normalize_props(format_properties& props);

bool parse_format(const string &fmt, string &result, int &num_stars, format_properties &properties, string &fmt_tail) {
	if(!fmt.length()) {
		result = "";
		return 0;
	}
	string res, tail;
	format_properties props;
	int n_stars;
	ull spec_start = string::npos;
	ull i = 0;
	while(i < fmt.length() && (fmt[i] != '%' || i < fmt.length() - 1 && fmt[i + 1] == '%' ||
	i && fmt[i - 1] == '%')) {
		if(fmt[i] != '%') {
			res += fmt[i];
		}
		else if(fmt[i] == '%' && fmt[i++] == '%') {
			res += '%';
		}
		++i;
	}
	if(i < fmt.length() - 1 && fmt[i] == '%' && fmt[i + 1] != '%')
		spec_start = i;
	bool ret = spec_start != string::npos;
	if (ret) {
		if (spec_start + 1 < fmt.length()) {
			string new_fmt = fmt.substr(spec_start);
			spec_start = 0;
			string specs = "csdioxXufFeEaAgGnp";
			long long spec_ind = new_fmt.find_first_of(specs);
			if (spec_ind == string::npos)
				throw "specifier expected";
			ll cur_ind = spec_start + 1;
			bool f = 1;
			//flags
			int i;
			for (i = 0; (i < 5) && f; ++i) {
				switch (new_fmt[cur_ind + i]) {
					case ' ': props.space = 1; break;
					case '+': props.sign = 1; break;
					case '#': props.alt = 1; break;
					case '-': props.left = 1; break;
					case '0': {
						props.zero = 1; break;
					}
					default: f = 0; 
				}
			}
			//width
			string number;
			n_stars = 0;
			for(cur_ind += i - 1; cur_ind < new_fmt.length(); ++cur_ind) {
				if(new_fmt[cur_ind] == '*') {
					props.width = Width::star;
					++n_stars;
				}
				else if(isdigit(new_fmt[cur_ind]))
					number += new_fmt[cur_ind];
				else
					break;
			}
			if(number.length() && number[0] != 0) {
				props.width = Width::number;
				props.width_num = atoi(number.c_str());
			}
			//precision
			for(number = ""; cur_ind < new_fmt.length(); ++cur_ind) {
				if(new_fmt[cur_ind] == '.' && cur_ind < new_fmt.length() - 1 && 
					new_fmt[cur_ind] == '*') {
					props.precision = Precision::star;
					++n_stars;
				}
				else if(isdigit(new_fmt[cur_ind]))
					number += new_fmt[cur_ind];
				else
					break;
			}
			if(number.length() && number[0] != 0) {
				props.precision = Precision::number;
				props.prec_num = atoi(number.c_str());
			}
			props.len = new_fmt.substr(cur_ind, spec_ind - cur_ind);
			props.spec = new_fmt[spec_ind];
			tail = new_fmt.substr(spec_ind + 1);
			
		}
	}
	result = res;
	num_stars = n_stars;
	properties = props;
	fmt_tail = tail;
	return ret;
}

template <class T>
T to_c_str(T t);

const char *to_c_str(string);