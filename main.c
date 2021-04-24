#include <stdio.h>
#include <math.h>
#define eps 0.001

// пример треугольника из функций 5sinx, exp(2x-15), 2arctgx в районе отрезка [6;8.1]
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

double root (double (*f)(double), double (*g)(double), double a, double b, double eps1,
             double (*fdiff)(double), double (*gdiff)(double)) {
    double mid = (a + b) / 2, x = b;
    int F1diff = 1, F2diff = 1; //знаки первой и второй производной у функции F(x)=f(x)-g(x)
    if (2 * (f(mid) - g(mid)) > f(a) - g(a) + f(b) - g(b))
        F2diff = -1;
    if (fdiff(mid) < gdiff(mid))
        F1diff = -1;
    if (F1diff * F2diff < 0) {
        x = a;
        eps1 = -eps1; // знаки будем сравнивать у x и x+eps, приближение к корню слева
    }
    //int cnt = 0;
    do {
        x -= (f(x) - g(x)) / (fdiff(x) - gdiff(x));
        //cnt++;
    } while ((f(x) - g(x)) * (f(x - eps1) - g(x - eps1)) > 0);
    //printf("we find root by %d steps ", cnt);
    return x;
}

double integral (double (*f)(double), double a, double b, double eps2) {
    int n = 1; //кол-во отрезков разбиения
    double sum1 = (f(a) + f(b) / 2), sum2, absdiff, delta;  //integral = sum * delta
                                    // sum = f(a)/2 + sumf(x_i) for i=1,...,n-1 + f(b)/2
    do {
        n *= 2;
        sum2 = sum1;  //sum2 = sum1 + sum( f(середина разбиения отрезка, на концах которого уже вычислены функции) )
        delta = (b - a) / n;
        double x = a + delta;
        for (int k = 1; k <= n / 2; k++) {
            sum2 += f(x);
            x += 2 * delta;
        }
        absdiff = fabs(delta * (2 * sum1 - sum2));
        sum1 = sum2;
    } while (absdiff >= 3 * eps2); //правило Рунге
    return sum2 * delta;
}

double f12 (double x) {
    return fabs(f1(x) - f2(x));
}
double f13 (double x) {
    return fabs(f1(x) - f3(x));
}

int main () {
    double root1 = root(f1, f2, 6, 8.1, eps / 2, f1diff, f2diff),
        root2 = root(f2, f3, 6, 8.1, eps / 2, f2diff, f3diff),
        root3 = root(f3, f1, 6, 8.1, eps / 2, f3diff, f1diff),
        int1 = integral(f12, root1 , root2, eps / 6),
        int2 = integral(f13, root2, root3, eps / 6);
    printf("root1 = %f\n", root1);
    printf("root2 = %f\n", root2);
    printf("root3 = %f\n", root3);
    printf("int1 = %f\n", int1);
    printf("int2 = %f\n", int2);
    printf("ans = %f\n", int1 + int2);
    return 0;
}
