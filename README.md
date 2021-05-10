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

Functions:
    f1 = 0.35x^2 - 0.95x + 2.7
    f2 = 1 / (x + 2)
    f3 = 3x + 1

General options:
    -help                show help message
    -points   (-p)       display intersection points
    -iters    (-i)       display number of iterations in root()

Commands:
    test_root            test function root()
    test_integral        test function integral()

    Usage: program <command> [options]

Command options:
    -a <value>           set a(left)
    -b <value>           set b(right)
    -eps <value>         set eps(precision)

    only with test_root:
        -func1 <[1-3]>   specify first function
        -func2 <[1-3]>   specify second function

    only with test_integral:
        -func <[1-3]>    specify function

Examples:
    program -points -iters
    program test_integral -a -0.1 -b 0.5 -eps 0.02 -func 3
    program test_root -a -1 -b 1.01 -eps 0.1 -func1 3 -func2 1
