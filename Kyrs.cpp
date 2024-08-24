

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
using namespace std;

vector<vector<int>> number_generator(int n, int k)
{
    vector <vector<int>> answer;
    vector<int> temp;
    if (k == 1)
    {
        for (int i = 0; i < n; i++)
        {
            temp.push_back(i);
            answer.push_back(temp);
            temp.erase(temp.begin());
        }
    }
    else
    { 
        int z = 0;
        for (int i = 0; i < n; i++)
        {
            temp.push_back(i);
            for (int j = i; j < n; j++)
            {
                z = j + 1;
                while (size(temp) < k && z < n)
                {
                    temp.push_back(z);
                    z++;
                }
                if (size(temp) == k)
                {
                    answer.push_back(temp);
                }
                temp.erase(temp.begin() + 1, temp.end());
            }
            temp.erase(temp.begin());
        }
    }
    return(answer);
}

double multiplier(vector<double> x, vector<vector<int>> i)
{
    double answer = 0;
    double temp = 1;
    for (int j = 0; j < size(i); j++)
    {
        for (int k = 0; k < size(i[j]); k++)
        {
            temp = temp * -x[i[j][k]];
        }
        answer += temp;
        temp = 1;
    }
    return answer;
}

vector<double> inter_mg(vector<double> x, vector<double> y)
{
    vector<double> answer(size(x));
    vector<double> tx(size(x));
    vector<double> temp_answer(size(x));
    vector<vector<int>> index;
    double tempy = 1;
    for (int i = 0; i < size(x); i++)
    {
        tx = x;
        tx.erase(tx.begin() + i, tx.begin() + i+1);
        tempy = 1;
        for (int j = 0; j < size(tx); j++)
        {
            tempy = tempy*(x[i] - tx[j]);
        }
        for (int j = size(tx); j > 0; j--)
        {
            index = number_generator(size(tx), j);
            temp_answer[j] = multiplier(tx, index);
            
        }
        temp_answer[0] = 1;
        tempy = y[i] / tempy;
        for (int j = 0; j < size(x); j++)
        {
            temp_answer[j] = temp_answer[j] * tempy;
            answer[j] += temp_answer[j];
        }
    }
    return answer;
}

double ans_in_dote(vector<double> x, double c)
{
    double temp = 0;
    for (int i = 0; i < size(x); i++)
    {
        temp += x[i] * pow(c, size(x) - i - 1);
    }
    return temp;
}

vector<double> mg_multiplier(vector<double> coef, double a, double b)
{
    vector<double> answer(size(coef) + 1);
    answer[0] = coef[0] * a;
    for (int i = 1; i < size(coef); i++)
    {
        answer[i] += coef[i] * a + b * coef[i - 1];
    }
    answer[size(coef)] = coef[size(coef) - 1] * b;
    return answer;
}
double min_metod2(vector<double> coef, double a, double b, double error)
{
    double answer = std::numeric_limits<double>::infinity();
    while (a <= b)
    {
        answer = min(answer, ans_in_dote(coef, a));
        a += error;
    }
    return answer;
}

double min_metod1(vector<double> coef, vector<double> zero, double a, double b)
{
    double answer = std::numeric_limits<double>::infinity();
    for (int i = 0; i < size(zero); i++)
    {
        if (zero[i] >= a && zero[i] <= b)
        {
            answer = min(answer, ans_in_dote(coef, zero[i]));
        }
    }
    answer = min(answer, ans_in_dote(coef, a));
    answer = min(answer, ans_in_dote(coef, b));
    return answer;
}

vector<double> proiz(vector<double> coef)
{
    vector<double> answer(size(coef) -1);
    for (int i = 0; i < size(coef)-1; i++)
    {
        answer[i] += coef[i] * (size(coef)-1-i);
    }
    return answer;
}

inline double sq(const double& x) { //Функция sq принимает  double число, служит для быстрого вычисления квадрата числа
    return x * x;
}

inline int signum(const double& x) {//Функция signum, принимает double число, и возвращает его знак, то есть 1 если число положительно или -1 - если отрицательное
    return (0.0 < x) - (x < 0.0);
}

inline double newton1(const double a, const double b, const double c, double x) {//Функция newton1 вычисляет первый шаг итерации Ньютона, принимает два double числа
    double y, y1;
    y = a + x;
    y1 = 2 * y + x;
    y1 = x * y1 + b;
    y = (x * y + b) * x + c;
    if (y1 != 0.0) x -= y / y1;
    return x;
}

inline int eqn_quadratic(const double a, const double b, double*& x) {//Функция для нахождения  корней квадратного урванения, принимает вектор x, в него заносятся ответы, и 2 числа double, a это коэфициент при x, а b это свободный член
    double p = -0.5f * a,
        d = sq(p) - b;//Вычисление дискриминанта
    if (d >= 0.0) {//Вычисление вещественных корней
        d = sqrt(d);
        x[1] = p - d;
        x[2] = p + d;
        return 2;
    }
    x[1] = x[2] = std::numeric_limits<double>::infinity();
    return 0;//Если корни комплексные возвращаем 0
}

inline int eqn_quadratic(const double a, const double b, const double c, const double e, const double f, double*& x) {//Функция для нахождения корней квадратного уравнения,  принимает вектор x, в него заносятся ответы
    //2 числа double, a это коэфициент при x, а b это свободный член
    //3 числа double,с , e, f, это коэфициенты многочлена третей степени, идущие по убыванию, они необходимы для функции  newton1
    double p = -0.5f * a,
        d = sq(p) - b;
    if (d >= 0.0) {//Вычисляем дискриминат
        d = sqrt(d);
        if (p < 0.0) {
            x[1] = newton1(c, e, f, p - d);//Вычисляем один из корней, с помощью метода Ньютона
            x[2] = p + d;
            return 2;
        }
        else {
            x[1] = p - d;
            x[2] = newton1(c, e, f, p + d);//Вычисляем один из корней, с помощью метода Ньютона
            return 2;
        }
    }
    x[1] = x[2] = std::numeric_limits<double>::infinity();
    return 0;//Если корни комплексные возвращаем 0
}

int eqn_cubic(const double a, const double b, const double e, double*& x) { // Remark #2
    int i_slope, i_loc;
    double w, xh, y, y1, y2, dx, c[2], d;
    double  prec = 1.0e-4f; // termination criterion, Remark #3

    w = (double)1.0f;
    if (e == 0.0) { // Отсутствует ли свободный член
        x[0] = 0.0;
        if (eqn_quadratic(a, b, x) == 0)
            return 1;
        else {

            return 3;
        }
    }

    xh = -1.0f / 3.0f * a; // точка перегиба
    y = e + xh * (b + xh * (a + xh));
    if (y == 0.0) { // Является ли точка перегиба корнем 
        x[0] = x[1] = xh;
        c[1] = xh + a; // Понижение порядка уравнения
        c[0] = c[1] * xh + b;
        return 1 + eqn_quadratic(c[1], c[0], x);
    }
    i_loc = (y >= 0.0f);
    d = sq(a) - 3 * b;
    if ((i_slope = signum(d)) == 1) // Laguerre-Nair-Samuelson bounds
    {
        if (d < 0)
            return 0;
        else
            xh += ((i_loc) ? -2.0f / 3.0f : 2.0f / 3.0f) * sqrt(d);
    }
    else if (i_slope == 0) { // Седловая точка?
        x[0] = xh - cbrt(y);//Вещественный корень всего один
        return 1;
    }
    do { // Итерации ( Сам метод Галлея)
        y = a + xh;
        y1 = 2 * y + xh;
        y2 = y1 + 3 * xh;
        y1 = xh * y1 + b;
        y = (xh * y + b) * xh + e;
        dx = y * y1 / (sq(y1) - 0.5f * y * y2);
        if (isinf(dx) || isnan(dx))
            return 0;
        xh -= dx;
    } while (fabs(dx) > prec * fabs(xh));
    x[0] = x[2] = xh;
    if (i_slope == 1) {
        c[1] = xh + a; // Понижение порядка уравнения
        c[0] = c[1] * xh + b;
        return 1 + eqn_quadratic(c[1], c[0], a, b, e, x);
    }

    return 1;
}

int main()
{
    vector<double> x = { 1.05,	1.15, 1.25, 1.35 };
    vector<double> y = { 2.30,	2.74,	3.46,	4.60 };
    vector<double> coef = inter_mg(x, y);
    /*for (int i = 0; i < size(coef); i++)
    {
        cout << coef[i] << "\n";
    }
    cout << ans_in_dote(coef, 1.05) << "\n";
    cout << ans_in_dote(coef, 1.15) << "\n";
    cout << ans_in_dote(coef, 1.25) << "\n";
    cout << ans_in_dote(coef, 1.35) << "\n";*/
    double a = 1.05;
    double b = 1.35;
    double c = 1.10;
    double error = 0.001;
    double k = ans_in_dote(coef, c);
    /*cout << k << "\n";*/
    coef = mg_multiplier(coef, a * k, b * k);
    double min2 = min_metod2(coef, a, b, error);
    /*for (int i = 0; i < size(coef); i++)
    {
        cout << coef[i] << "\n";
    }*/
    vector<double> proizv = proiz(coef);
    for (int i = 0; i < size(proizv); i++)
    {
        cout << proizv[i] << "\n";
    }
    double* temp = new double [3];
    vector<double> dot_0(3);
    eqn_cubic(proizv[1] / proizv[0], proizv[2] / proizv[0], proizv[3] / proizv[0], temp);
    for (int i = 0; i < 3; i++)
    {
        dot_0[i] =temp[i];
        cout << temp[i] << "\n";
    }
    double min = min_metod1(coef, dot_0, a, b);
    cout << min << "\n";
    cout << min2 << "\n";
}

