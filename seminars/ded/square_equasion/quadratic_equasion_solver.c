#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>

///Defines accuracy of calculations
const double EPS = 1e-6;

enum roots_count {
    ZERO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INFINITE_ROOTS = -1,
};

char is_zero(double number);
int solve_quadratic_equation(double a, double b, double c, double *x1, double *x2);
void solver_interface();
void solver_test();

/**
    \brief Main function
*/
int main() {

    solver_test();

    return 0;
}


/**
    \brief Interface for quadratic equation solver

    Reads coefs 'a', 'b', 'c' in ax^2 + bx + c = 0 equation, prints solutions for it if possible, else prints 'No solution' or 'Any x suits'.
*/
void solver_interface() {
    double a = 0;
    double b = 0;
    double c = 0;
    scanf("%lg %lg %lg", &a, &b, &c);


    double x1 = 0;
    double x2 = 0;
    int roots_cnt = solve_quadratic_equation(a, b, c, &x1, &x2);

    switch (roots_cnt) {
    case ZERO_ROOTS:
        {
            printf("No solutions!\n");
            break;
        }
    case ONE_ROOT:
        {
            printf("x = %lg\n", x1);
            break;
        }
    case TWO_ROOTS:
        {
            printf("x1 = %lg\n", x1);
            printf("x2 = %lg\n", x2);
            break;
        }
    case INFINITE_ROOTS:
        {
            printf("Any x suits!\n");
            break;
        }
    }
}

/**
    \brief Checks if given number is zero with set epsilon

    Checks if given number is closer to 0 then epsilon.

    \param[in] number Number to check
    \return Result of |number| - eps < 0 statement
*/
char is_zero(double number) {
    return fabs(number) - EPS < 0.0;
}

/**
    \brief Solves quadratic equation

    If roots are found, puts them into given memory.
    \param[in] a,b,c Coefs of ax^2 + bx + c = 0 needed to be solved
    \param[out] x1,x2 Pointers on variables to store roots into
    \return Number of roots or -1 if any x solves the equation
*/
int solve_quadratic_equation(double a, double b, double c, double *x1, double *x2) {
    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(x1 != x2);

    if (is_zero(a)) {
        if (is_zero(b)) {
            if (is_zero(c)) {
                return INFINITE_ROOTS;
            } else {
                return 0;
            }
        } else {
            *x1 = *x2 = -c / b;
            return 1;
        }
    }

    double discriminant = b*b - 4*a*c;
    if (is_zero(discriminant)) {
        *x1 = -b / (2*a);
        *x2 = -b / (2*a);
        return 1;
    } else {
        if (discriminant < 0) {
            return 0;
        } else {
            double sq = sqrt(discriminant);
            *x1 = (-b - sq) / (2*a);
            *x2 = (-b + sq) / (2*a);
            return 2;
        }
    }
}

/**
    \brief Runs 1 test for quadratic equation solver

    Prints 'OK' if the test is passed successfully and 'ER' with breaking arguments if not.
    \param[in] a,b,c Coefs of equation to solve for the test
    \param[in] test_n Number of current test. Will be increased by 1

*/
void a_test(double a, double b, double c, int *test_n) {
    double x1 = 0, x2 = 0;
    double roots_cnt = solve_quadratic_equation(a, b, c, &x1, &x2);
    printf("Test %3d | ", *test_n);
    *test_n += 1;
    if (roots_cnt == 0) {
        if ((is_zero(a) && is_zero(b) && !is_zero(c)) || (!is_zero(a) && c > EPS) || (b * b - 4 * a * c < 0)) {
            printf("OK\n");
        } else {
            printf("ER | solver(%g, %g, %g) -> 0\n", a, b, c);
        }
    } else {
        if (is_zero(a*x1*x1 + b*x1 + c) && is_zero(a*x2*x2 + b*x2 + c)) {
            printf("OK\n");
        } else {
            printf("ER | solver(%g, %g, %g) -> [%g, %g]\n", a, b, c, x1, x2);
        }
    }
}

/**
    \brief Unit test function for quadratic equation solver

    Runs set of tests to check if function solve_quadratic_equation works properly. You shouldn't see any ER results if everything is OK.
*/
void solver_test() {
    srand(time(NULL));
    int test_n = 1;

    printf("99 random tests\n---\n");
    for (int i = 0; i < 99; ++i) { // generating 100 random tests
        a_test(rand(), rand(), rand(), &test_n);
    }

    printf("---\n99 extreme tests\n---\n");
    for (int i = 0; i < 99; ++i) {
        a_test(rand() % 3 - 1, rand() % 3 - 1, rand() % 3 - 1, &test_n);
    }
}
