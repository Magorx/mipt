#define L_COPY *(L->deep_copy())
#define R_COPY *(R->deep_copy())

OPDEF(add, +, 2, 5, L_RES + R_RES, DL + DR)
OPDEF(sub, -, 2, 5, L_RES - R_RES, DL + DR)
OPDEF(mul, *, 2, 7, L_RES * R_RES, DL * R + L * DR)
OPDEF(div, /, 2, 7, L_RES / R_RES, (DL * R - DR * L))

OPDEF(pow, ^, 2, 8,   pow(L_RES, R_RES), 0)
OPDEF(sin, sin, 1, 9, sin(R_RES), (cos(R)) )
OPDEF(cos, cos, 1, 9, cos(R_RES), 0)
OPDEF(log, log, 2, 8, log(R_RES) / log(L_RES), 0)