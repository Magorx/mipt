#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from random import randint

def f(a, l, r):
	s = 0
	for x in a:
		if l <= x and x <= r:
			s += x
	return s

a = []
A = 20
MX = 100
file = open('test.t', 'w')
print(A, file=file)
for i in range(A - 1):
	if randint(0, 1):
		x = randint(1, MX)
		print("+ {}".format(x), file=file)
		a.append(x)
	else:
		l = randint(1, MX)
		r = randint(l + 1, MX)
		print("? {} {}".format(l, r), file=file)
l = randint(1, MX)
r = randint(l + 1, MX)
print("? {} {}".format(l, r), file=file)
print(f(set(a), l, r))
file.close()