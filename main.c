#include <stdio.h>
#include <math.h>
#define eps 0.001

// example from my task
double ff1 (double x) {
    return (0.35 * x * x - 0.95 * x + 2.7);
}
double ff3 (double x) {
    return (3 * x + 1);
}
double ff2 (double x) {
    return (1 / (x + 2));
}
double ff1diff (double x) {
    return (0.7 * x - 0.95);
}
double ff3diff (double x) {
    return 3;
}
double ff2diff (double x) {
    return (-1 / ((x + 2) * (x + 2)));
}

// test examples
double f1 (double x) {
    return exp(2 * x - 15);
}
double f2 (double x) {
    return 5 * sin(x);
}
double f3 (double x) {
    return 2 * atan(x);
}
double f1diff (double x) {
    return 2 * f1(x);
}
double f2diff (double x) {
    return 5 * cos(x);
}
double f3diff (double x) {
    return 2 / (1 + x * x);
}

//final test example
double h1 (double x) {
    return 10 * log(x);
}
double h2 (double x) {
    return x * x * x;
}
double h3 (double x) {
    return x * x * x * x - x - 20;
}
double h1diff (double x) {
    return 10 / x;
}
double h2diff (double x) {
    return 3 * x * x;
}
double h3diff (double x) {
    return 4 * x * x * x - 1;
}

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
    int cnt = 0;
    do {
        x -= (f(x) - g(x)) / (fdiff(x) - gdiff(x));
        cnt++;
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
    } while (absdiff >= 3 * eps2); // Runge's rule
    return sum2 * delta;
}

double f12 (double x) {
    return fabs(f1(x) - f2(x));
}
double f13 (double x) {
    return fabs(f1(x) - f3(x));
}
double ff12 (double x) {
    return fabs(ff1(x) - ff2(x));
}
double ff13 (double x) {
    return fabs(ff1(x) - ff3(x));
}

int main () {
    double root1 = root(f1, f2, 6, 8.1, eps / 6, f1diff, f2diff),
        root2 = root(f2, f3, 6, 8.1, eps / 6, f2diff, f3diff),
        root3 = root(f3, f1, 6, 8.1, eps / 6, f3diff, f1diff),
        int1 = integral(f12, root1 , root2, eps / 6),
        int2 = integral(f13, root2, root3, eps / 6);
    printf("Calculating root for 5sinx - exp(2x-15) in [6;8.1], expected result = 6.301380, calculated result x1= %f\n", root1);
    printf("Calculating root for 2arctgx - 5sinx in [6;8.1], expected result = 6.890506, calculated result x2= %f\n", root2);
    printf("Calculating root for 2arctgx - exp(2x-15) in [6;8.1], expected result = 8.031292, calculated result x3= %f\n", root3);
    printf("Calculating integral for 5sinx - exp(2x-15) in [x1;x2], expected result = 0.790995, calculated result = %f\n", int1);
    printf("Calculating integral for 2arctgx - exp(2x-15) in [x2;x3], expected result = 1.980160, calculated result = %f\n", int2);
    printf("Calculating curve triangle's square, expected result = 2.771155, calculating result = %f\n", int1 + int2);
    if (fabs(int1 + int2 - 2.771155) < eps) printf("calculating answer differs from expected answer less then eps = 0.001, good!\n\n");
    else printf("bad\n\n");
    printf("new test!\n");
    double root11 = root(ff1, ff2, -1.99, -1.3, eps / 6, ff1diff, ff2diff),
            root22 = root(ff2, ff3, -1.99, 1, eps / 6, ff2diff, ff3diff),
            root33 = root(ff3, ff1, -1.99, 1, eps / 6, ff3diff, ff1diff),
            int11 = integral(ff12, root11, root22, eps / 6),
            int22 = integral(ff13, root22, root33, eps / 6);
    printf("Calculating root for 0.35x^2 - 0.95x + 2.7 - 1 / (x+2) in [-1.99;-1.3], expected result = -1.821140, calculated result x1= %f\n", root11);
    printf("Calculating root for 3x + 1 - 1 / (x+2) in [-1.99;1], expected result = -0.152872, calculated result x2= %f\n", root22);
    printf("Calculating root for 0.35x^2 - 0.95x + 2.7 - (3x + 1) in [-1.99;1], expected result = 0.448178, calculated result x3= %f\n", root33);
    printf("Calculating integral for 0.35x^2 - 0.95x + 2.7 - 1 / (x+2) in [x1;x2], expected result = 4.438040, calculated result = %f\n", int11);
    printf("Calculating integral for 0.35x^2 - 0.95x + 2.7 - (3x + 1) in [x2;x3], expected result = 0.682157, calculated result = %f\n", int22);
    printf("Calculating curve triangle's square, expected result = 5.120197, calculating result = %f\n", int11 + int22);
    if (fabs(int11 + int22 - 5.120197) < eps) printf("calculating answer differs from expected answer less then eps = 0.001, good!\n\n");
    else printf("bad\n\n");
    printf("new test, not triangle's square, just roots and integrals!\n");
    root1 = root(h1, h2, 1, 1.5, eps / 6, h1diff, h2diff),
    root2 = root(h2, h3, 2, 3, eps / 6, h2diff, h3diff),
    root3 = root(h3, h1, 2, 3, eps / 6, h3diff, h1diff),
    int1 = integral(h1, 1, 2, eps / 6),
    int2 = integral(h3, 3, 5, eps / 6);
    printf("Calculating root for 10lnx - x^3 in [1;1.5], expected result = 1.177194, calculated result x1= %f\n", root1);
    if (fabs(root1 - 1.177194) < eps) printf("calculating answer differs from expected answer less then eps = 0.001, good!\n");
    else printf("bad\n");
    printf("Calculating root for x^3 - x^4 + x + 20 in [2;3], expected result = 2.4777, calculated result x2= %f\n", root2);
    if (fabs(root2 - 2.4777) < eps) printf("calculating answer differs from expected answer less then eps = 0.001, good!\n");
    else printf("bad\n");
    printf("Calculating root for 10lnx - x^4 + x + 20 in [2;3], expected result = 2.35844, calculated result x3= %f\n", root3);
    if (fabs(root3 - 2.35844) < eps) printf("calculating answer differs from expected answer less then eps = 0.001, good!\n");
    printf("Calculating integral for lnx in [1;2], expected result = 3.8629, calculated result = %f\n", int1);
    if (fabs(int1 - 3.8629) < eps) printf("calculating answer differs from expected answer less then eps = 0.001, good!\n");
    else printf("bad\n");
    printf("Calculating integral for x^4 - x - 20 in [3;5], expected result = 528.4, calculated result = %f\n", int2);
    if (int1 - 528.4 < eps) printf("calculating answer differs from expected answer less then eps = 0.001, good!\n");
    else printf("bad\n");
    return 0;
}
