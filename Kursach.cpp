
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#define _USE_MATH_DEFINES
using namespace std;



void max_el(int a, vector<vector <double>>& mass, vector<double>& values)
{
    for (int i = a; i < mass.size(); i++)
    {
        if (abs(mass[a][a]) < abs(mass[i][a]))
        {
            swap(mass[a], mass[i]);
            swap(values[a], values[i]);
        }
    }
}

vector<double> Gaus(vector<vector <double>> mass, vector<double> values)
{
    vector<double> ans(mass[0].size());
    double per;
    double del;
    double sum = 0;
    for (int i = 0; i < mass.size() - 1; i++)
    {
        max_el(i, mass, values);
        per = mass[i][i];
        for (int j = i + 1; j < mass.size(); j++)
        {
            del = -(mass[j][i] / per);
            mass[j][i] = 0;
            values[j] += values[i] * del;
            for (int k = i + 1; k < mass[0].size(); k++)
            {
                mass[j][k] += mass[i][k] * del;
            }
        }
    }
    for (int i = mass.size() - 1; i >= 0; i--)
    {
        for (int j = i + 1; j < mass.size(); j++)
        {
            sum += mass[i][j] * ans[j];
        }
        ans[i] = (values[i] - sum) / mass[i][i];
        sum = 0;
    }
    return ans;
}





double eq1(double x1, double x2)
{
    return  x1*x1+x2*x2-5;
}

double eq2(double x1, double x2)
{
    return tan(x1*x2)-x1-0.1;
}


double eq_P1(double x1, double x2)
{
    return 2 *x1;
}

double eq_P2(double x1, double x2)
{
    return 2* x2;
}

double eq_P3(double x1, double x2)
{
    return x2/(cos(x1*x2)*cos(x1 * x2))-1;
}

double eq_P4(double x1, double x2)
{
    return x1 / (cos(x1 * x2) * cos(x1 * x2));
}

vector<double> Newton(double (*f1)(double, double), double (*f2)(double, double), double (*f3)(double, double), double (*f4)(double, double), double (*f5)(double, double), double (*f6)(double, double), double a, double b, double acc)
{
    vector<double> ans{ 0,0 };
    vector<double> lastans{ a,b };
    bool accuracy = false;
    vector<vector<double>> W;
    vector<double> F;
    vector<double> soul;
    double temp;
    while (accuracy != true)
    {
        W = { {f1(lastans[0],lastans[1]),f2(lastans[0],lastans[1])}, {f3(lastans[0],lastans[1]), f4(lastans[0],lastans[1])} };
        F = { -f5(lastans[0],lastans[1]),-f6(lastans[0],lastans[1]) };
        soul = Gaus(W, F);

        for (int i = 0; i < lastans.size(); i++)
        {
            temp = ans[i];
            ans[i] = lastans[i] + soul[i];
            lastans[i] = temp;
        }

        if ((abs(f5(ans[0], ans[1])) < acc) && (abs(f6(ans[0], ans[1])) < acc))
        {
            accuracy = true;
        }
        
    }
    return ans;
}

double K1(double h, double x, double y1, double y2, double (*f)(double, double, double))
{
    return h * f(x, y1,y2);
}

double K2(double h, double x, double y1, double y2, double (*f)(double, double, double))
{
    return h * f(x + 0.5 * h, y1 + 0.5 * K1(h, x, y1,y2, f),y2);
}

double K3(double h, double x, double y1, double y2, double (*f)(double, double, double))
{
    return h * f(x + 0.5 * h, y1 + 0.5 * K2(h, x, y1, y2, f),y2);
}

double K4(double h, double x, double y1, double y2, double (*f)(double, double, double))
{
    return h * f(x + h, y1 + K3(h, x, y1, y2, f),y2);
}

pair<vector<double>, vector<double>> Adams_for_Systems(double x1, double x2, double h, double y10, double y20, double (*f1)(double, double, double), double (*f2)(double, double, double))
{
    vector<double> dots1;
    vector<double> dots2;
    double x = x1;
    double y1i = y10;
    double y2i = y20;
    dots1.push_back(y1i);
    dots2.push_back(y2i);

    for (int i = 0; i < 3; i++)
    {
        y1i = y1i + (1.0 / 6.0) * (K1(h, x, y1i, y2i, f1) + 2 * K2(h, x, y1i, y2i, f1) + 2 * K3(h, x, y1i, y2i, f1) + K4(h, x, y1i, y2i, f1));
        dots1.push_back(y1i);
        y2i = y2i + (1.0 / 6.0) * (K1(h, x, y2i, y1i, f2) + 2 * K2(h, x, y2i, y1i, f2) + 2 * K3(h, x, y2i, y1i, f2) + K4(h, x, y2i, y1i, f2));
        dots2.push_back(y2i);
        x += h;
    }
    int i = 3;

    double c = double(h / 24);

    while (x <= x2)
    {
        y1i += c * (55 * f1(x, dots1[i], dots2[i]) - 59 * f1(x - h, dots1[i - 1], dots2[i - 1]) + 37 * f1(x - 2 * h, dots1[i - 2], dots2[i - 2]) - 9 * f1(x - 3 * h, dots1[i - 3], dots2[i - 3]));
        y2i += c * (55 * f2(x, dots2[i], dots1[i]) - 59 * f2(x - h, dots2[i - 1], dots1[i - 1]) + 37 * f2(x - 2 * h, dots2[i - 2], dots1[i - 2]) - 9 * f2(x - 3 * h, dots2[i - 3], dots1[i - 3]));
        dots2.push_back(y2i);
        dots1.push_back(y1i);
        x += h;
        i++;
    }
    return make_pair(dots1,dots2);
}

double Y1(double x, double y1, double y2)
{
    return  sin(y1*y2);
}

double Y2(double x, double y1, double y2)
{
    return  cos(x*y1 * y2);
}



int main()
{


    double a = 0;
    double b = 4;
    double h = 0.1;

    vector<double> ans;


    ans = Newton(eq_P1, eq_P2, eq_P3, eq_P4, eq1, eq2, 5, 0, 0.00001);
    cout << "Корни системы нелинейных уравнений, вычисленный методом Ньютона, равны:  " << '\n';
    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << '\n';
    }

    pair<vector<double>, vector<double>> y = Adams_for_Systems(a, b, h, ans[0], ans[1], Y1, Y2);
    pair<vector<double>, vector<double>> y2 = Adams_for_Systems(a, b, h/2, ans[0], ans[1], Y1, Y2);



    std::ofstream out;
    out.open("x1.txt");
    double x = a;
    if (out.is_open())
    {

        out << x << "\n";
        while (abs(x - b) > h / 10)
        {
            x += h;
            out << x << "\n";
        }
    }
    out.close();
    double e = -1000;

    out.open("y1_1.txt");
    if (out.is_open())
    {
        for (int i = 0; i < size(y.first); i++)
        {
            out << y.first[i] << "\n";
            e = max(e, abs((y.first[i] - y2.first[i]) / 15));
        }
    }
    out.close();
    cout << e << "\n";
    e = -1000;
    out.open("y1_2.txt");
    if (out.is_open())
    {
        for (int i = 0; i < size(y.second); i++)
        {
            out << y.second[i] << "\n";
            e = max(e, abs((y.second[i] - y2.second[i]) / 15));
        }
    }
    out.close();
    cout << e << "\n";
}

