#include <stdio.h>
#include <math.h>
#define eps 0.001

// example from my task
double f1 (double x) { return (0.35 * x * x - 0.95 * x + 2.7);}
double f2 (double x) { return (1 / (x + 2));}
double f3 (double x) { return (3 * x + 1);}
double df1 (double x) { return (0.7 * x - 0.95);}
double df2 (double x) { return (-1 / ((x + 2) * (x + 2)));}
double df3 (double x) { return 3;}

// first test example
double g1 (double x) { return exp(2 * x - 15);}
double g2 (double x) { return 5 * sin(x);}
double g3 (double x) { return 2 * atan(x);}
double dg1 (double x) { return 2 * exp(2 * x - 15);}
double dg2 (double x) { return 5 * cos(x);}
double dg3 (double x) { return 2 / (1 + x * x);}

// second test example
double h1 (double x) { return 10 * log(x);}
double h2 (double x) { return x * x * x;}
double h3 (double x) { return x * x * x * x - x - 20;}
double dh1 (double x) { return 10 / x;}
double dh2 (double x) { return 3 * x * x;}
double dh3 (double x) { return 4 * x * x * x - 1;}

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
    do {
        x -= (f(x) - g(x)) / (fdiff(x) - gdiff(x));
    } while (((f(x) - g(x)) * (f(x - eps1) - g(x - eps1)) > 0) || (fabs(f(x - eps1) - g(x - eps1)) > 1));
    return x;
}

double integral (double (*f)(double), double a, double b, double eps2) {
    int n = 1; // numbers of splitting segments in [a,b]
    double sum1 = (f(a) + f(b) / 2), sum2, absdiff, delta;  //integral = sum * delta
                                    // sum = f(a)/2 + sumf(x_i) for i=1,...,n-1 + f(b)/2
    do {
        n *= 2;
        sum2 = sum1;  //sum2 = sum1 + sum( f(middles of new splitting segments) )
        delta = (b - a) / n;
        double x = a + delta;
        for (int k = 1; k <= n / 2; k++) {
            sum2 += f(x);
            x += 2 * delta;
        }
        absdiff = fabs(delta * (2 * sum1 - sum2));
        sum1 = sum2;
    } while (absdiff >= eps2 / 3); // Runge's rule
    return sum2 * delta;
}

void test (const char *mode, const char* func, double left, double right, double expect, double res, double eps1) {
    printf("%s of %s in [%f; %f], expected %f, calculated %f\n", mode, func, left, right, expect, res);
    if (fabs(res - expect) < eps1)
        printf("GOOD! Calculating answer differs from expected less than %f\n\n", eps1);
    else
        printf("BAD! Calculating answer differs from expected more than %f\n\n", eps1);
}

int main () {
    const char *mode_root = "root", *mode_int = "integral";
    double epss = eps / 10,
    root1 = root(f1, f2, -1.99, -1.3, epss, df1, df2), root2 = root(f2, f3, -1.99, 1, epss, df2, df3),
    root3 = root(f3, f1, -1.99, 1, epss, df3, df1),
    int1 = integral(f1, root1, root3, epss), int2 = integral(f2, root1, root2, epss),
    int3 = integral(f3, root2, root3, epss), ans = fabs(int1 - int2 - int3);
    test(mode_root, "0.35x^2 - 0.95x + 2.7 - 1 / (x+2)", -1.99, -1.3, -1.821140, root1, epss);
    test(mode_root, "3x + 1 - 1 / (x+2)", -1.99, 1, -0.152872, root2, epss);
    test(mode_root, "0.35x^2 - 0.95x + 2.7 - (3x + 1)", -1.99, 1, 0.448178, root3, epss);
    test(mode_int, "0.35x^2 - 0.95x + 2.7", root1, root3, 8.32227, int1, epss);
    test(mode_int, "1 / (x+2)", root1, root2, 2.33478, int2, epss);
    test(mode_int, "(3x + 1)", root2, root3, 0.867291, int3, epss);
    printf("Curve triangle's square, expected 5.120197, calculating %f\n", ans);
    if (fabs(ans - 5.120197) < eps) printf("GOOD! Calculating answer differs from expected less than 0.001\n\n");
    else printf("BAD! Calculating answer differs from expected more than 0.001\n\n");

    printf("new functions!\n\n");
    root1 = root(g1, g2, 6, 8.1, epss, dg1, dg2), root2 = root(g2, g3, 6, 8.1, epss, dg2, dg3),
    root3 = root(g3, g1, 6, 8.1, epss, dg3, dg1),
    int1 = integral(g1, root1 , root3, epss), int2 = integral(g2, root1, root2, epss),
    int3 = integral(g3, root2, root3, epss);
    test(mode_root, "5sinx - e^(2x-15)", 6, 8.1, 6.301380, root1, epss);
    test(mode_root, "2arctgx - 5sinx", 6, 8.1, 6.890506, root2, epss);
    test(mode_root, "2arctgx - e^(2x-15)", 6, 8.1, 8.031292, root3, epss);
    test(mode_int, "e^(2x - 15)", root1, root3, 1.40144, int1, epss);
    test(mode_int, "5sinx", root1, root2, 0.893273, int2, epss);
    test(mode_int, "2arctgx", root2, root3, 3.27932, int3, epss);

    printf("new functions!\n");
    root1 = root(h1, h2, 1, 1.5, epss, dh1, dh2),
    root2 = root(h2, h3, 2, 3, epss, dh2, dh3),
    root3 = root(h3, h1, 2, 3, epss, dh3, dh1),
    test(mode_root,"10lnx - x^3", 1, 1.5, 1.177194, root1, epss);
    test(mode_root, "x^4 - x - 20 - x^3", 2, 3, 2.477724, root2, epss);
    test(mode_root, "x^4 - x - 20 - 10lnx", 2, 3, 2.35844, root3, epss);
    test(mode_int,"lnx", 1, 2, 3.862943, integral(h1, 1, 2, epss), epss);
    test(mode_int,"x^4 - x - 20", 3, 5, 528.40000, integral(h3, 3, 5, epss), epss);
    return 0;
}
