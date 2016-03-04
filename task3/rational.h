#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

class rational {
    int num, denom;
    int gcd(int a, int b);
public:
    rational(int n);

    rational(int num, int denom);

    int getNum() const;

    int getDenom() const;

    rational operator+(const rational &frac) const;

    rational operator-(const rational &frac) const;

    rational operator*(const rational &frac) const;

    rational operator/(const rational &frac) const;
};

rational::rational(int n) {
    num = n;
    denom = 1;
}

rational::rational(int nm, int dnm) {
    int d = gcd(abs(nm), abs(dnm));
    num = nm / d;
    denom = dnm / d;
}

int rational::getNum() const {
    return num;
}

int rational::getDenom() const {
    return denom;
}

rational rational::operator+(const rational &frac) const {
    return rational(num * frac.denom + denom * frac.num, frac.denom * denom);
}

rational rational::operator-(const rational &frac) const {
    return rational(num * frac.denom - denom * frac.num, frac.denom * denom);
}

rational rational::operator*(const rational &frac) const {
    return rational(num * frac.num, denom * frac.denom);
}

rational rational::operator/(const rational &frac) const {
    return rational(num * frac.denom, denom * frac.num);
}

int rational::gcd(int a, int b) {
    return a ? gcd(b % a, a) : b;
}
