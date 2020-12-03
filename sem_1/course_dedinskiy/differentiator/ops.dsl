#define L_COPY *(L->deep_copy())
#define R_COPY *(R->deep_copy())

#define DL *(L->differentiate(file))
#define DR *(R->differentiate(file))

#define COS(N) *NEW(OPERATION, (double) 'c', PRIOR_COS, (N))
#define SIN(N) *NEW(OPERATION, (double) 's', PRIOR_SIN, (N))
#define LOG(N) *NEW(OPERATION, (double) 'L', PRIOR_LOG, (N))

OPDEF(add, +, 2, 5, L_RES + R_RES, DL + DR, "", "+", "")
OPDEF(sub, -, 2, 5, L_RES - R_RES, DL + DR, "", "-", "")

OPDEF(mul, *, 2, 7, L_RES * R_RES, DL * R_COPY + DR * L_COPY, "", "\\cdot ", "")
OPDEF(div, /, 2, 7, L_RES / R_RES, (DL * R_COPY - DR * L_COPY) / R_COPY / R_COPY, "\\frac{", "}{", "}")

OPDEF(pow, ^, 2, 8,   pow(L_RES, R_RES), (L_COPY ^ R_COPY) * ((DR * LOG(L_COPY) + (L_COPY ^ (-1)) * R_COPY)), "", "^{", "}")

OPDEF(sin, sin, 1, 9, sin(R_RES), COS(R_COPY) * DR, "\\sin {", " ", "}")
OPDEF(cos, cos, 1, 9, cos(R_RES), SIN(R_COPY) * DR  * (-1), "\\cos {", " ", "}")

OPDEF(Ln, Ln, 1, 9, log(R_RES), (R_COPY ^ (-1)) * DR, "\\ln {", "", "}")

OPDEF(log, log, 2, 8, log(R_RES) / log(L_RES), ((R_COPY ^ -1) * DR * LOG(L_COPY) - LOG(R_COPY) * DL * (L_COPY ^ -1)) / LOG(L_COPY) / LOG(L_COPY), "\\log _{", "} {", "}")