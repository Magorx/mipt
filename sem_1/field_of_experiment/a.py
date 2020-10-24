#!/bin/python3

def F(a, b):
	ret = []
	for x in a:
		for y in b:
			ret.append(x + y)
	ret.sort()
	return ret

a = [1, 2, 3, 3, 3, 4, 7, 9]
b = [1, 4, 5, 9, 15, 16, 21, 100, 1001]

c = F(a, b)
k = 6
print('ans =', c[k])

r = len(b)
l = 0
while (l + 1 < r):
	m = (l + r) // 2

	much = 0
	for i in range(len(a)):
		if (m * i) == k - 1:
			print(b[m] + a[i], 'is an ans?')
		if m * i > k:
			much = 1
			break

	if much:
		r = m
	else:
		l = m
print(l)

