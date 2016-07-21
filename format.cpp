#include "format.h"
const int buf_sz = 32;

template <class... Args>
string format(string const &fmt, Args... args) {
	string res;
	size_t ind = fmt.find("%");
	if (ind != std::string::npos) {
		res = fmt.substr(0, ind);
		if (ind + 1 < fmt.length()) {
			if (fmt[ind + 1] == '%') {
				res += '%';
				string tail;
				if (ind + 2 < fmt.length())
					tail = format(fmt.substr(ind + 2), args...);
				return res + tail;
			}
			if (!sizeof...(args))
				throw std::out_of_range("argument expected");
			// check if call to format_call_i is possible: fmt and
			// sizeof...(args) >=
			//pass specifier as character
			// res += format_call_i(fmt.substr(ind), args...);
			return res;
		}
		throw "specifier expected after %";
	}
	else
		return fmt;
}


template <class W, class Head, class... Tail>
string format_call_2(string const& fmt, string const& fmt_tail, size_t spec_ind, W width, Head h, Tail... t) {
	char spec = fmt[spec_ind];
	if (typeid(W) != typeid(1) || !correct_type(spec, h))
		throw invalid_argument("");
	char buf[buf_sz];
	string res;
	while (snprintf(buf, buf_sz, fmt, t...))
		res += string(buf);
	tail = format(fmt_tail, t...);
	return res + tail;
}

template <class Head, class... Tail>
string format_call_4(string const& fmt, string const& fmt_tail, W width, P precision, Head h, Tail... t) {
	char spec = fmt.back();
	if (typeid(W) != typeid(1) || typeid(P) != typeid(1) || !correct_type(spec, h))
		throw invalid_argument("");
}


template<class T>
bool correct_type(const char c, const T t) {
	switch (c) {
	case 'd': case 'i': case 'x': case 'a': case 'u': case 'c':
		return std::is_integral<T>::value;
	case 'f': case 'g': case 'e': return std::is_floating_point<T>::value;
	case 'p': case 's': return std::is_pointer<T>::value;
	}
}