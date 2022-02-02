#!/usr/bin/python3

import sys

args = sys.argv
if (len(args) < 2):
    print("Pass an argument plz")
    exit(0)

name = args[1]
with open(name, "r") as fin:
    lines = fin.readlines()

lines = list(map(lambda x: x.rstrip().replace('\t', '    '), lines))
maxlen = max(map(len, lines))
for i in range(len(lines)):
    lines[i] = f' {lines[i]: <{maxlen}} \\'
print("\"" + ' ' * (maxlen + 1) + "\\")
for l in lines:
    print(l)
print("\"")
