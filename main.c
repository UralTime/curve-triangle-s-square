#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define eps 0.001

extern double f1 (double x);
extern double f2 (double x);
extern double f3 (double x);
extern double df1 (double x);
extern double df2 (double x);
extern double df3 (double x);

enum {DEFAULT, TEST_ROOT, TEST_INTEGRAL};

int PRINT_POINTS = 0,
        PRINT_ITERS = 0,
        MODE = DEFAULT;

double root (double (*f)(double), double (*g)(double), double a, double b, double eps1,
             double (*fdiff)(double), double (*gdiff)(double)) {
    double mid = (a + b) / 2, x = b;
    int F1diff = 1, F2diff = 1; // sign of F' and F'', where F(x) = f(x) - g(x)
    if (2 * (f(mid) - g(mid)) > f(a) - g(a) + f(b) - g(b))
        F2diff = -1;
    if (fdiff(mid) < gdiff(mid))
        F1diff = -1;
    if (F1diff * F2diff < 0) {
        x = a;
        eps1 = -eps1; // compare signs of x and x+eps, approximation from the left instead of right
    }
    int iters = 0;
    do {
        x -= (f(x) - g(x)) / (fdiff(x) - gdiff(x));
        iters++;
    } while (((f(x) - g(x)) * (f(x - eps1) - g(x - eps1)) > 0) || (fabs(f(x - eps1) - g(x - eps1)) > 1));
    if (PRINT_ITERS)
        printf("root is found by %d iterations\n", iters);
    if (PRINT_POINTS)
        printf("intersection point: (%f, %f)\n", x, f(x));
    return x;
}

double integral (double (*f)(double), double a, double b, double eps2) {
    int n = 1; // numbers of splitting segments in [a,b]
    double prev_sum = (f(a) + f(b) / 2), cur_sum, absdiff, delta;  //integral = cur_sum * delta
                                    // cur_sum = f(a)/2 + prev_sumf(x_i) for i=1,...,n-1 + f(b)/2
    do {
        n *= 2;
        cur_sum = prev_sum;  // cur_sum = prev_sum + sum( f(middles of new splitting segments) )
        delta = (b - a) / n;
        double x = a + delta;
        for (int k = 1; k <= n / 2; k++) {
            cur_sum += f(x);
            x += 2 * delta;
        }
        absdiff = fabs(delta * (2 * prev_sum - cur_sum));
        prev_sum = cur_sum;
    } while (absdiff >= eps2 / 3); // Runge's rule
    return cur_sum * delta;
}

const char *help_text =         // if user types "help", he'll see this message
"\
Functions:\n\
    f1 = 0.35x^2 - 0.95x + 2.7\n\
    f2 = 1 / (x + 2)\n\
    f3 = 3x + 1\n\
\n\
General options:\n\
    -help                show help message\n\
    -points              display intersection points\n\
    -iters               display number of iterations in root()\n\
\n\
Commands:\n\
    test_root            test function root()\n\
    test_integral        test function integral()\n\
\n\
    Usage: program <command> [options]\n\
\n\
Command options:\n\
    -a <value>           set a(left)\n\
    -b <value>           set b(right)\n\
    -eps <value>         set eps(precision)\n\
\n\
    only with test_root:\n\
        -func1 <[1-3]>   specify first function\n\
        -func2 <[1-3]>   specify second function\n\
\n\
    only with test_integral:\n\
        -func <[1-3]>    specify function\n\
\n\
Examples:\n\
    program -points -iters\n\
    program test_integral -a -0.1 -b 0.5 -eps 0.02 -func 3\n\
    program test_root -a -1 -b 1.01 -eps 0.1 -func1 3 -func2 1\n\
";

int main (int argc, char **argv) {
    double epss = eps / 10, a = -1.99, b = 1;
    double (*foo1)(double) = f1;
    double (*foo2)(double) = f2;
    double (*dfoo1)(double) = df1;
    double (*dfoo2)(double) = df2;
    if (argc > 1) {
        if (!strcmp(argv[1], "-help")) {
            printf("%s", help_text);
            return 0;
        }
        for (int i = 1; i < argc; i++) {
            if (!strcmp(argv[i], "-points") || !strcmp(argv[i], "-p"))
                PRINT_POINTS = 1;
            else if (!strcmp(argv[i], "-iters") || !strcmp(argv[i], "-i"))
                PRINT_ITERS = 1;
            else if (!strcmp(argv[i], "test_root"))
                MODE = TEST_ROOT;
            else if (!strcmp(argv[i], "test_integral"))
                MODE = TEST_INTEGRAL;
            else if (!strcmp(argv[i], "-a"))
                a = strtod(argv[++i], NULL);    // next param - left bound of segment - from string to double
            else if (!strcmp(argv[i], "-b"))
                b = strtod(argv[++i], NULL);    // next param - right bound of segment - from string to double
            else if (!strcmp(argv[i], "-eps"))
                epss = strtod(argv[++i], NULL);  // next param - precision - from string to double
            else if (!strcmp(argv[i], "-func1") || !strcmp(argv[i], "-func")) // choice func for test root or integral
                switch (atoi(argv[++i])) {
                    case 1:
                        foo1 = f1;
                        dfoo1 = df1;
                        break;
                    case 2:
                        foo1 = f2;
                        dfoo1 = df2;
                        break;
                    case 3:
                        foo1 = f3;
                        dfoo1 = df3;
                }
            else if (!strcmp(argv[i], "-func2"))   //choice func for test root or integral
                switch (atoi(argv[++i])) {
                    case 1:
                        foo2 = f1;
                        dfoo2 = df1;
                        break;
                    case 2:
                        foo2 = f2;
                        dfoo2 = df2;
                        break;
                    case 3:
                        foo2 = f3;
                        dfoo2 = df3;
                }
        }
    }

    if (MODE == TEST_ROOT) {
        printf("a = %f, b = %f, eps = %f\n\n", a, b, epss);
        double x = root(foo1, foo2, a, b, epss, dfoo1, dfoo2);
        if (!PRINT_POINTS)
            printf("Intersection point = (%f, %f)\n", x, foo1(x));
    }

    else if (MODE == TEST_INTEGRAL) {
        printf("a = %f, b = %f, eps = %f\n\n", a, b, epss);
        printf("Integral = %f\n", integral(foo1, a, b, epss));
    }
    else { // MODE == DEFAULT
        printf("a = %f, b = %f, eps1 = eps2 = %f\n", a, b, epss);
        double root12, root13, root23;
        if (PRINT_POINTS || PRINT_ITERS)
            printf("\nf1 & f2\n");
        root12 = root(f1, f2, a, -1.3, epss, df1, df2);
        if (PRINT_POINTS || PRINT_ITERS)
            printf("\nf1 & f3\n");
        root13 = root(f1, f3, a, b, epss, df1, df3);
        if (PRINT_POINTS || PRINT_ITERS)
            printf("\nf2 & f3\n");
        root23 = root(f2, f3, a, b, epss, df2, df3);
        double int1 = integral(f1, root12, root13, epss), int2 = integral(f2, root12, root23, epss),
        int3 = integral(f3, root23, root13, epss), ans = int1 - int2 - int3;
        printf("\n int1 = %f \n int2 = %f \n int3 = %f \n", int1, int2, int3);
        printf("\n Result integral = %f \n", ans);
    }
    return 0;
}