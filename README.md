# curve-triangle's-square
This program counts the square of area bounded by 3 functions.  

Поиск корня функции F(x) на отрезке [a, b] с заданной точностью eps.
Уточним, что F(x) – это функция разности f(x) – g(x))на отрезке [a, b], то есть
нули функции F(x) –это точки пересечения функций f(x) и g(x). На отрезке [a,
b] функция F(x) имеет один изолированный корень. Такой отрезок
выбирается на основе анализа функций f(x) и g(x) и их производных.
Существование такого отрезка гарантировано для семейства функций из
вариантов задания. Первая и вторая производные функции F(x) на отрезке
[a, b] непрерывны и не обращаются в 0, то есть имеют постоянный знак (>0
либо <0).

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
