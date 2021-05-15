#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#define eps 0.001

extern double f1 (double x);
extern double f2 (double x);
extern double f3 (double x);
extern double df1 (double x);
extern double df2 (double x);
extern double df3 (double x);

enum {DEFAULT, TEST_ROOT, TEST_INTEGRAL, TEST_DEFAULT};

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
    -verbose (-v)        show information about mission of the program\n\
    -points (-p)         display intersection points\n\
    -iters  (-i)         display number of iterations in root()\n\
\n\
Commands:\n\
    test_root            test function root()\n\
    test_integral        test function integral()\n\
    test_default         run default tests\
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
\n\
";

const char *verbose_text =
"Calculating area between functions f1 (x) = 0.35x^2 -0.95x + 2.7, f2 (x) = 3x + 1, f3 (x) = 1 / (x + 2),\n\
intersections are found by trapezoid method, integrating is done using Newton's method\n\
On the segment [a, b], the function f(x) - g(x) has one isolated root. \n\
Such a segment should be chosen based on the analysis of the functions f(x) and g(x) and their derivatives.\n\
The first and second derivatives of the function F (x) on the segment [a, b] are continuous and do not equal to zero,\n\
that is, they have a constant sign (> 0 or <0).\n\
\n\
";

//first test example
double g1 (double x) { return exp(2 * x - 15);}
double g2 (double x) { return 5 * sin(x);}
double g3 (double x) { return 2 * atan(x);}
double dg1 (double x) { return 2 * exp(2 * x - 15);}
double dg2 (double x) { return 5 * cos(x);}
double dg3 (double x) { return 2 / (1 + x * x);}

//second test example
double h1 (double x) { return 10 * log(x);}
double h2 (double x) { return x * x * x;}
double h3 (double x) { return x * x * x * x - x - 20;}
double dh1 (double x) { return 10 / x;}
double dh2 (double x) { return 3 * x * x;}
double dh3 (double x) { return 4 * x * x * x - 1;}

void test (const char *mode, const char* func, double left, double right, double expect, double res, double eps1) {
    printf("%s of %s in [%f; %f], expected %f, calculated %f\n", mode, func, left, right, expect, res);
    if (fabs(res - expect) < eps1)
        printf("GOOD! Calculating answer differs from expected less than %f\n\n", eps1);
    else
        printf("BAD! Calculating answer differs from expected more than %f\n\n", eps1);
}

int main (int argc, char **argv) {
    double epss = eps / 10, a = -1.99, b = 1;
    const char *mode_root = "root", *mode_int = "integral";
    double (*foo1)(double) = f1;
    double (*foo2)(double) = f2;
    double (*dfoo1)(double) = df1;
    double (*dfoo2)(double) = df2;
    if (argc > 1)
        for (int i = 1; i < argc; i++) {
            if (!strcmp(argv[i], "-help"))
                printf("%s", help_text);
            if ((!strcmp(argv[i], "-verbose")) || (!strcmp(argv[1], "-v"))) 
                printf("%s", verbose_text);
            if (!strcmp(argv[i], "-points") || !strcmp(argv[i], "-p"))
                PRINT_POINTS = 1;
            else if (!strcmp(argv[i], "-iters") || !strcmp(argv[i], "-i"))
                PRINT_ITERS = 1;
            else if (!strcmp(argv[i], "test_root"))
                MODE = TEST_ROOT;
            else if (!strcmp(argv[i], "test_integral"))
                MODE = TEST_INTEGRAL;
            else if (!strcmp(argv[i], "test_default"))
                MODE = TEST_DEFAULT;
            else if (!strcmp(argv[i], "-a")) {
                a = strtod(argv[++i], NULL);    // next param - left bound of segment - from string to double
                if (errno == ERANGE) {
                    printf("incorrect a\n");
                    return 0;
                }
            }
            else if (!strcmp(argv[i], "-b")) {
                b = strtod(argv[++i], NULL);    // next param - right bound of segment - from string to double
                if (errno == ERANGE) {
                    printf("incorrect b!\n");
                    return 0;
                }
            }
            else if (!strcmp(argv[i], "-eps")) {
                epss = strtod(argv[++i], NULL);  // next param - precision - from string to double
                if ((errno == ERANGE) || (epss <= 0.0000001) || (epss >= 1)) {
                    printf("incorrect eps!\n");
                    return 0;
                }
            }
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
                        break;
                    default:
                        printf("incorrect number of foo1!\n");
                        return 0;
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
                        break;
                    default:
                        printf("incorrect number of foo2!\n");
                        return 0;
                }
        }
    if (a > b) {
        printf("incorrect input: a > b!\n");
        return 0;
    }
    if (MODE == TEST_ROOT) {
        if (((f1 == foo1) && (f2 == foo2)) || ((f2 == foo1) && (f1 == foo2)))
            b = -1.3;
        printf("a = %f, b = %f, eps = %f\n\n", a, b, epss);
        double x = root(foo1, foo2, a, b, epss, dfoo1, dfoo2);
        if (!PRINT_POINTS)
            printf("Intersection point = (%f, %f)\n", x, foo1(x));
    }
    else if (MODE == TEST_INTEGRAL) {
        printf("a = %f, b = %f, eps = %f\n\n", a, b, epss);
        printf("Integral = %f\n", integral(foo1, a, b, epss));
    }
    else if (MODE == TEST_DEFAULT) {
        double root1 = root(g1, g2, 6, 8.1, epss, dg1, dg2), root2 = root(g2, g3, 6, 8.1, epss, dg2, dg3),
        root3 = root(g3, g1, 6, 8.1, epss, dg3, dg1),
        int1 = integral(g1, root1 , root3, epss), int2 = integral(g2, root1, root2, epss),
        int3 = integral(g3, root2, root3, epss);
        test(mode_root, "5sinx - e^(2x-15)", 6, 8.1, 6.301380, root1, epss);
        test(mode_root, "2arctgx - 5sinx", 6, 8.1, 6.890506, root2, epss);
        test(mode_root, "2arctgx - e^(2x-15)", 6, 8.1, 8.031292, root3, epss);
        test(mode_int, "e^(2x - 15)", root1, root3, 1.40144, int1, epss);
        test(mode_int, "5sinx", root1, root2, 0.893273, int2, epss);
        test(mode_int, "2arctgx", root2, root3, 3.27932, int3, epss);

        printf("another test!\n");
        root1 = root(h1, h2, 1, 1.5, epss, dh1, dh2),
        root2 = root(h2, h3, 2, 3, epss, dh2, dh3),
        root3 = root(h3, h1, 2, 3, epss, dh3, dh1),
        test(mode_root,"10lnx - x^3", 1, 1.5, 1.177194, root1, epss);
        test(mode_root, "x^4 - x - 20 - x^3", 2, 3, 2.477724, root2, epss);
        test(mode_root, "x^4 - x - 20 - 10lnx", 2, 3, 2.35844, root3, epss);
        test(mode_int,"lnx", 1, 2, 3.862943, integral(h1, 1, 2, epss), epss);
        test(mode_int,"x^4 - x - 20", 3, 5, 528.40000, integral(h3, 3, 5, epss), epss);
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