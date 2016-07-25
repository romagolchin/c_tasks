#include "format.h"
const int buf_sz = 32;
typedef long long ll;
typedef unsigned long long ull;

template <class From, class To>
typename enable_if<is_convertible<From, To>::value, To>::type
cast(From value) {
	return value;
}

template <class From, class To>
typename enable_if<!is_convertible<From, To>::value, To>::type
cast(From value) {
	throw invalid_argument("");
}

template <class... Args>
string format(const string &fmt, Args... args) {
	if(!fmt.length())
		return "";
	string res;
	ull spec_start = string::npos;
	ull i = 0;
	while(i < fmt.length() && (fmt[i] != '%' || i < fmt.length() - 1 && fmt[i + 1] == '%' ||
	i && fmt[i - 1] == '%')) {
		if(fmt[i] != '%') {
			res += fmt[i];
		}
		else if(fmt[i] == '%' && fmt[i + 1] == '%') {
			res += '%';
			++i;
		}
		++i;
	}
	if(i < fmt.length() - 1 && fmt[i] == '%' && fmt[i + 1] != '%')
		spec_start = i;

	if (spec_start != string::npos) {
		format_properties props;
		// res += format_impl(fmt.substr(0, spec_start));
		cerr << "substring=" << fmt.substr(0, spec_start) << endl;
		if (spec_start + 1 < fmt.length()) {
			// res += format_impl(fmt.substr(0, spec_start));
			string new_fmt = fmt.substr(spec_start);
			spec_start = 0;
			string specs = "csdioxXufFeEaAgGnp";
			long long spec_ind = new_fmt.find_first_of(specs);
			cerr << new_fmt[spec_ind] << endl;
			cerr << "spec_ind=" << spec_ind << endl;
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
			int n_stars = 0;
			for(cur_ind += i - 1; cur_ind < new_fmt.length(); ++cur_ind) {
				if(new_fmt[cur_ind] == '*') {
					props.width = Width::arg;
					++n_stars;
				}
				else if(isdigit(new_fmt[cur_ind]))
					number += new_fmt[cur_ind];
				else
					break;
			}
			if(number.length() && number[0] != 0)
				props.width_num = atoi(number.c_str());
			//precision
			for(number = ""; cur_ind < new_fmt.length(); ++cur_ind) {
				if(new_fmt[cur_ind] == '.' && cur_ind < new_fmt.length() - 1 && 
					new_fmt[cur_ind] == '*') {
					props.precision = Precision::arg;
					++n_stars;
				}
				else if(isdigit(new_fmt[cur_ind]))
					number += new_fmt[cur_ind];
				else
					break;
			}
			if(number.length() && number[0] != 0)
				props.prec_num = atoi(number.c_str());
			props.len = new_fmt.substr(cur_ind, spec_ind - cur_ind);
			props.spec = new_fmt[spec_ind];
			string tail = new_fmt.substr(spec_ind + 1);
			if(n_stars == 0)
				res += format_impl(props, tail, args...);
			else if(n_stars == 1 && sizeof...(args) >= 2)
				res += format_impl_1(props, tail, args...);
			else if(n_stars == 2 && sizeof...(args) >= 3)
				res += format_impl_2(props, tail, args...);
			return res;
		}
	}
	else
		return format_impl(fmt);
}

string format_impl(string const& fmt) {
	string res;
	for (ull i = 0; i < fmt.length(); ++i) {
		if (fmt[i] == '%' && (i == fmt.length() - 1 || fmt[i + 1] != '%'))
			throw invalid_argument("not enough arguments 5 " + to_string(i));
		if(i < fmt.length() - 1 && fmt[i] == '%' && fmt[i++] == '%') {
			res += '%';
		}
		else
			res += fmt[i];
	}
	return res;
}

template <class Arg, class... Args>
string format_impl(format_properties& props, string const &tail, Arg arg, Args... args) {
	string res;
	normalize_flags(props);
	string fmt = "%";
	if(props.alt) fmt += '#';
	if(props.sign) fmt += '+';
	if(props.space) fmt += ' ';
	fmt += props.len + props.spec;
	switch (tolower(props.spec)) {
		case 'd': case 'i': case 'x': case 'a': case 'u': case 'c': {
			ll new_arg = cast<Arg, ll>(arg);
			if(arg == 0 && props.prec_num == 0) {
				res = "";
				break;
			}
			char* buf = new char[buf_sz];
			size_t num_chars = snprintf(buf, buf_sz, fmt.c_str(), new_arg);
			char to_fill = props.zero ? '0' : ' ';
			string fill = string(max(0ull, props.width_num - num_chars), to_fill);
			res = props.left ? string(buf) + fill : fill + string(buf);
			delete [] buf;
			break; 
		}
		case 'f': case 'g': case 'e': {
			long double new_arg = cast<Arg, long double>(arg);
			break; 
		}
		case 'p': {
			if(!is_pointer<Arg>::value)
				throw invalid_argument("expected pointer argument");
			
			break;
		}
		case 's': {
			char* new_arg = cast<Arg, char*>(arg);
			if(new_arg == nullptr)
				res = "<null>";
			break;
		}
	}
	return res + format(tail, args...);
}

void normalize_flags(format_properties &props) {
	if(props.left)
		props.zero = 0;
	if(props.sign)
		props.space = 0;
}