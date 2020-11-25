
OPDEF(add, +, 2, 5, LEFT + RIGHT)
OPDEF(sub, -, 2, 5, LEFT - RIGHT)
OPDEF(mul, *, 2, 7, LEFT * RIGHT)
OPDEF(div, /, 2, 7, LEFT / RIGHT)

OPDEF(pow, ^, 2, 8,   pow(LEFT, RIGHT))
OPDEF(sin, sin, 1, 9, sin(RIGHT))
OPDEF(cos, cos, 1, 9, cos(RIGHT))
OPDEF(log, log, 2, 8, log(RIGHT) / log(LEFT))