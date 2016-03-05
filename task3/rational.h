#include <stdlib.h>
#include <bits/stdc++.h>

#pragma once


class rational {
    int num, denom;
public:
    int gcd(int a, int b);

    rational(int n);

    rational(int num, int denom);

    int getNum() const;

    int getDenom() const;

    rational operator+(const rational &frac) const;

    rational operator-(const rational &frac) const;

    rational operator*(const rational &frac) const;

    rational operator/(const rational &frac) const;
};

