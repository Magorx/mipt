#!/bin/python3

from math import sqrt
a, b, c = map(int, input().split())
def f(a, b, c):
    d = b * b - 4 * a * c
    dd = sqrt(d)
    print(dd)
    return [(-b - dd) / (2 * a), (-b + dd) / (2 * a)]
print(*f(a, b, c))