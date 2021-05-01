#include <stdio.h>
#include <math.h>
#define eps 0.001

// example from my task
double ff1 (double x) { return (0.35 * x * x - 0.95 * x + 2.7);}
double ff3 (double x) { return (3 * x + 1);}
double ff2 (double x) { return (1 / (x + 2));}
double ff1diff (double x) { return (0.7 * x - 0.95);}
double ff3diff (double x) { return 3;}
double ff2diff (double x) { return (-1 / ((x + 2) * (x + 2)));}
double ff12 (double x) { return fabs(ff1(x) - ff2(x));}
double ff13 (double x) { return fabs(ff1(x) - ff3(x));}

// test examples
double f1 (double x) { return exp(2 * x - 15);}
double f2 (double x) { return 5 * sin(x);}
double f3 (double x) { return 2 * atan(x);}
double f1diff (double x) { return 2 * f1(x);}
double f2diff (double x) { return 5 * cos(x);}
double f3diff (double x) { return 2 / (1 + x * x);}
double f12 (double x) { return fabs(f1(x) - f2(x));}
double f13 (double x) { return fabs(f1(x) - f3(x));}

//final test example
double h1 (double x) { return 10 * log(x);}
double h2 (double x) { return x * x * x;}
double h3 (double x) { return x * x * x * x - x - 20;}
double h1diff (double x) { return 10 / x;}
double h2diff (double x) { return 3 * x * x;}
double h3diff (double x) { return 4 * x * x * x - 1;}

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
    //int cnt = 0;
    do {
        x -= (f(x) - g(x)) / (fdiff(x) - gdiff(x));
        //cnt++;
    } while (((f(x) - g(x)) * (f(x - eps1) - g(x - eps1)) > 0) || (fabs(f(x - eps1) - g(x - eps1)) > 1));
    //printf("we find root by %d steps ", cnt);
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

void test (const char* s1, const char* s2, const char* s3, double a, double b, double expect, double res, double eps3) {
    printf("Calculating %s for %s - %s in [%f; %f] with precision %f, expected result = %f, calculated result = %f\n", s3, s1, s2, a, b, eps3, expect, res);
    if (fabs(res - expect) < eps3)
        printf("calculating answer differs from expected answer less than %f, good!\n", eps3);
    else
        printf("calculating answer differs from expected answer more than %f, bad!\n", eps3);
}

int main () {
    double epss = eps / 10;
    double root1 = root(f1, f2, 6, 8.1, epss, f1diff, f2diff),
        root2 = root(f2, f3, 6, 8.1, epss, f2diff, f3diff),
        root3 = root(f3, f1, 6, 8.1, epss, f3diff, f1diff),
        int1 = integral(f12, root1 , root2, epss),
        int2 = integral(f13, root2, root3, epss);
    test("5sinx", "exp(2x-15)", "root", 6, 8.1, 6.301380, root1, epss);
    test("2arctgx", "5sinx", "root", 6, 8.1, 6.890506, root2, epss);
    test("2arctgx", "exp(2x-15)", "root", 6, 8.1, 8.031292, root3, epss);
    test("5sinx", "exp(2x-15)", "integral", root1, root2, 0.790995, int1, epss);
    test("2arctgx", "exp(2x-15)", "integral", root2, root3, 1.980160, int2, epss);
    printf("Calculating curve triangle's square, expected result = 2.771155, calculating result = %f\n", int1 + int2);
    if (fabs(int1 + int2 - 2.771155) < eps) printf("calculating answer differs from expected answer less then eps = 0.001, good!\n\n");
    else printf("calculating answer differs from expected answer more then eps = 0.001, bad!\n\n");
    printf("new test!\n");
    root1 = root(ff1, ff2, -1.99, -1.3, epss, ff1diff, ff2diff),
    root2 = root(ff2, ff3, -1.99, 1, epss, ff2diff, ff3diff),
    root3 = root(ff3, ff1, -1.99, 1, epss, ff3diff, ff1diff),
    int1 = integral(ff12, root1, root2, epss),
    int2 = integral(ff13, root2, root3, epss);
    test("0.35x^2 - 0.95x + 2.7", "1 / (x+2)", "root", -1.99, -1.3, -1.821140, root1, epss);
    test("3x + 1", "1 / (x+2)", "root", -1.99, 1, -0.152872, root2, epss);
    test("0.35x^2 - 0.95x + 2.7", "(3x + 1)", "root", -1.99, 1, 0.448178, root3, epss);
    test("0.35x^2 - 0.95x + 2.7", "1 / (x+2)", "integral", root1, root2, 4.438040, int1, epss);
    test("0.35x^2 - 0.95x + 2.7", "(3x + 1)", "integral", root2, root3, 0.682157, int2, epss);
    printf("Calculating curve triangle's square, expected result = 5.120197, calculating result = %f\n", int1 + int2);
    if (fabs(int1 + int2 - 5.120197) < eps) printf("calculating answer differs from expected answer less then eps = 0.001, good!\n\n");
    else printf("calculating answer differs from expected answer more then eps = 0.001, bad!\n\n");
    printf("new test, not triangle's square, just roots and integrals!\n");
    root1 = root(h1, h2, 1, 1.5, epss, h1diff, h2diff),
    root2 = root(h2, h3, 2, 3, epss, h2diff, h3diff),
    root3 = root(h3, h1, 2, 3, epss, h3diff, h1diff),
    test("10lnx", "x^3", "root", 1, 1.5, 1.177194, root1, epss);
    test("x^3", "(x^4 - x - 20)", "root", 2, 3, 2.477724, root2, epss);
    test("10lnx", "(x^4 - x - 20)", "root", 2, 3, 2.35844, root3, epss);
    test("lnx", "0", "integral", 1, 2, 3.862943, integral(h1, 1, 2, epss), epss);
    test("x^4 - x - 20", "0", "integral", 3, 5, 528.40000, integral(h3, 3, 5, epss), epss);
    return 0;
}
