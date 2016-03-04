#include <stdlib.h>

class rational {
    int num, denom;
    int gcd(int a, int b);
public:
    rational(int n);

    rational(int num, int denom);

    int getNum() const;

    int getDenom() const;

    rational operator+(const rational &frac);

    rational operator-(const rational &frac);

    rational operator*(const rational &frac);

    rational operator/(const rational &frac);
};

rational::rational(int n) {
    num = n;
    denom = 1;
}

rational::rational(int num, int denom) {
    int d = gcd(num, denom);
    num = num / d;
    denom = denom / d;
}

int rational::getNum() const {
    return num;
}

int rational::getDenom() const {
    return denom;
}

rational rational::operator+(const rational &frac) {
    return rational(num * frac.denom + denom * frac.num, frac.num * num);
}

rational rational::operator-(const rational &frac) {
    return rational(num * frac.denom - denom * frac.num, frac.num * num);
}

rational rational::operator*(const rational &frac) {
    return rational(num * frac.num, denom * frac.denom);
}

rational rational::operator/(const rational &frac) {
    return rational(num * frac.denom, denom * frac.num);
}

int rational::gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    return a ? gcd(a, b % a) : b;
}
