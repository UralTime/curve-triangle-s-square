#include <stdio.h>
#define eps 0.001

// тестовый пример из условия
double f1 (double x) {
    return (0.35 * x * x - 0.95 * x + 2.7);
}
double f3 (double x) {
    return (3 * x + 1);
}
double f2 (double x) {
    return (1 / (x + 2));
}
double f1diff (double x) {
    return (0.7 * x - 0.95);
}
double f3diff (double x) {
    return 3;
}
double f2diff (double x) {
    return (-1 / ((x + 2) * (x + 2)));
}

double abs (double x) {
    return ((x < 0)? -x: x);
}

double root (double (*f)(double), double (*g)(double), double a, double b, double eps1,
             double (*fdiff)(double), double (*gdiff)(double)) {
    double mid = (a + b) / 2, x = b;
    int F1diff = 1, F2diff = 1, cnt = 0; //знаки первой и второй производной у функции F(x)=f(x)-g(x)
    if (2 * (f(mid) - g(mid)) > f(a) - g(a) + f(b) - g(b))
        F2diff = -1;
    if (fdiff(mid) < gdiff(mid))
        F1diff = -1;
    if (F1diff * F2diff < 0) {
        x = a;
        eps1 = -eps1; // знаки будем сравнивать у x и x+eps, приближение к корню слева
    }
    do {
        x -= (f(x) - g(x)) / (fdiff(x) - gdiff(x));
        cnt++;
    } while ((f(x) - g(x)) * (f(x - eps1) - g(x - eps1)) > 0);
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
        absdiff = abs(delta * (2 * sum1 - sum2));
        sum1 = sum2;
    } while (absdiff >= 3 * eps2); //правило Рунге
    return sum2 * delta;
}

double f12 (double x) {
    return abs(f1(x) - f2(x));
}
double f13 (double x) {
    return abs(f1(x) - f3(x));
}

int main () {
    double root1 = root(f1, f2, -1.99, -1.3, eps / 2, f1diff, f2diff),
        root2 = root(f2, f3, -1.99, 1, eps / 2, f2diff, f3diff),
        root3 = root(f3, f1, -1.99, 1, eps / 2, f3diff, f1diff),
        int1 = integral(f12, root1, root2, eps / 6),
        int2 = integral(f13, root2, root3, eps / 6);
    printf("root1 = %f\n", root1);
    printf("root2 = %f\n", root2);
    printf("root3 = %f\n", root3);
    printf("int1 = %f\n", int1);
    printf("int2 = %f\n", int2);
    printf("ans = %f\n", int1 + int2);
    return 0;
}
