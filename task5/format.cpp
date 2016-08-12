#include "format.h"

size_t Counter::cnt = 0; 


string format(const string &fmt) {
	string res, tail;
	int n_stars;
	format_properties props;
	bool f = parse_format(fmt, res, n_stars, props, tail);
	Counter::cnt = 0;
	if(f)
		throw out_of_range("not enough arguments");
	return res;
}

void format_properties::normalize() {
	if(left)
		zero = 0;
	if(sign)
		space = 0;
	if(width_num < 0) {
		width_num = -width_num;
		left = 1;
	}
	if(prec_num < 0)
		prec_num = 1;
}

const char *to_c_str(string s) {
	return s.c_str();
}

bool parse_format(const string &fmt, string &result, int &num_stars, format_properties &properties, string &fmt_tail) {
	if(!fmt.length()) {
		result = "";
		return 0;
	}
	string res, tail;
	format_properties props;
	int n_stars;
	size_t spec_start = string::npos;
	size_t j = 0;
	while(j < fmt.length()) {
		if(fmt[j] != '%') {
			res += fmt[j];
		}
		else if(fmt[j] == '%' && j < fmt.length() - 1 && 
			fmt[j + 1] == '%') {
			++j;
			// cerr << "yes " << fmt[j - 1] << ' ' << fmt[j] << endl;
			res += '%';
		}
		else {
			break;
		}
		++j;
	}
	if(j < fmt.length() - 1 && fmt[j] == '%' && fmt[j + 1] != '%')
		spec_start = j;
	bool ret = spec_start != string::npos;
	if (ret) {
		if (spec_start + 1 < fmt.length()) {
			string new_fmt = fmt.substr(spec_start);
			spec_start = 0;
			string specs = "csdioxXufFeEaAgGnp@";
			size_t spec_ind = new_fmt.find_first_of(specs);
			if (spec_ind == string::npos)
				throw "specifier expected";
			size_t cur_ind = spec_start + 1;
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
			//cerr << new_fmt << ' ' << cur_ind << ' ' << i << endl;
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
			//cerr << "cur_ind=" << cur_ind << endl;
			for(number = ""; cur_ind < new_fmt.length(); ++cur_ind) {
				if(new_fmt[cur_ind] == '.' && cur_ind + 1 < new_fmt.length() - 1 && 
					new_fmt[++cur_ind] == '*') {
					props.precision = Precision::star;
					++n_stars;
					
				}
				else if(new_fmt[cur_ind] == '.') continue;
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
			if(spec_ind - cur_ind > 2) {
				res = "";
				return 0;
			}

			props.spec = new_fmt[spec_ind];
			tail = new_fmt.substr(spec_ind + 1);
			
		}
	}
	Counter::cnt += res.length();
	result = res;
	num_stars = n_stars;
	//cerr << "n_stars=" << n_stars << endl;
	properties = props;
	fmt_tail = tail;
	return ret;
}
