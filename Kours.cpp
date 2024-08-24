// Kours.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <cmath>
#include <stack>
#include <locale.h>
using namespace std;

template <class T>
class Way
{
protected:
    vector<vector<T>> field;

    void resize(int length, int width)
    {
        field.resize(length);
        for (int i = 0; i < length; i++)
        {
            field[i].resize(width);
        }
    }

    void answer1(pair<int, int> start, pair<int, int> finish, int aswer)
    {
        cout << " От клетки " << start.first << '.' << start.second << " до клетки " << finish.first << '.' << finish.second << " можно добраться за " << aswer << " шагов." << '\n';
    }

    void answer2(pair<int, int> start, pair<int, int> finish, T aswer)
    {
        cout << " Стоимость пути от клетки " << start.first << '.' << start.second << " до клетки " << finish.first << '.' << finish.second << " равняется " << aswer << " единицам." << '\n';
    }

    int h_function(pair<int, int> start, pair<int, int> finish)
    {
        return(abs(start.first - finish.first) + abs(start.second - finish.second));
    }

public:
    Way(int length, int width)
    {
        field.assign(length, vector<T>(width));
       
    }

    Way(int length, int width, T walkprice)
    {
        field.assign(length, vector<T>(width));
        for (int i = 0; i < length; i++)
        {
            fill(field[i].begin(), field[i].end(), walkprice);
        }
    }

    
    int bfs(pair<int, int> start, pair<int, int> finish)
    {
        int s = start.first * field[0].size() + start.second;
        queue<int> q;
        q.push(s);
        vector<bool> used(field.size()* field[0].size());
        vector<int> d(field.size() * field[0].size());
        used[s] = true;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            if ((((v+1) % field[0].size()) !=0) &&!used[v+1])
            {
                used[v + 1] = true;
                q.push(v + 1);
                d[v + 1] = d[v] + 1;
            }
            if ((((v) % field[0].size())  != 0) && !used[v - 1])
            {
                used[v - 1] = true;
                q.push(v - 1);
                d[v - 1] = d[v] + 1;
            }
            if ((v /field[0].size() != (field.size()-1))&& !used[v + field[0].size()])
            {
                used[v + field[0].size()] = true;
                q.push(v + field[0].size());
                d[v + field[0].size()] = d[v] + 1;
            }
            if ((v / field[0].size() != 0) && !used[v - field[0].size()])
            {
                used[v - field[0].size()] = true;
                q.push(v - field[0].size());
                d[v - field[0].size()] = d[v] + 1;
            }
                
        }
        answer1(start, finish, d[finish.first * field[0].size() + finish.second]);
        if (field[0][0] != 0 && field[0][0] != 1)
            answer2(start, finish, d[finish.first * field[0].size() + finish.second]* field[0][0]);
        return d[finish.first * field[0].size() + finish.second];
    }

    

    int dfs(pair<int, int> start, pair<int, int> finish)
    {
        int f = start.first * field.size() + start.second;
        stack<int> s;
        vector<int> d(field.size() * field[0].size());
        vector<bool> used(field.size() * field[0].size());
        s.push(f);
        used[f] = true;
        while (!s.empty())
        {
            int now = s.top();
            s.pop();
            if ((((now + 1) % field[0].size()) != 0) && !used[now + 1])
            {
                used[now + 1] = true;
                s.push(now + 1);
                d[now + 1] = d[now] + 1;
            }
            if ((((now) % field[0].size()) != 0) && !used[now - 1])
            {
                used[now - 1] = true;
                s.push(now - 1);
                d[now - 1] = d[now] + 1;
            }
            if ((now / field[0].size() != (field.size() - 1)) && !used[now + field[0].size()])
            {
                used[now + field[0].size()] = true;
                s.push(now + field[0].size());
                d[now + field[0].size()] = d[now] + 1;
            }
            if ((now / field[0].size() != 0) && !used[now - field[0].size()])
            {
                used[now - field[0].size()] = true;
                s.push(now - field[0].size());
                d[now - field[0].size()] = d[now] + 1;
            }

        }
        
        answer1(start, finish, d[finish.first * field[0].size() + finish.second]);
        if (field[0][0] != 0 && field[0][0] != 1)
            answer2(start, finish, d[finish.first * field[0].size() + finish.second] * field[0][0]);
        return d[finish.first * field[0].size() + finish.second];
    }

    int lee(pair<int, int> start, pair<int, int> finish)
    {
        vector<vector<T>> LEEfield = field;
        for (int i = 0; i < LEEfield.size(); i++)
        {
            fill(LEEfield[i].begin(), LEEfield[i].end(), -1);
        }
        int d = 0;
        LEEfield[start.first][start.second] = d;
        int move1[4] = { 1,0,-1,0 };
        int move2[4] = { 0,1,0,-1 };
        while (LEEfield[finish.first][finish.second] == -1)
        {
            for (int y = 0; y < field.size(); ++y)
                for (int x = 0; x < field[0].size(); ++x)
                    if (LEEfield[y][x] == d)
                    {
                        for (int k = 0; k < 4; ++k)
                        {
                            int iy = y + move1[k], ix = x + move2[k];
                            if (iy > -1 && iy < field.size() && ix > -1 && ix < field[0].size() &&
                                LEEfield[iy][ix] == -1)
                            {
                                LEEfield[iy][ix] = d + 1;
                            }
                        }
                    }
            d++;
        }
        answer1(start, finish, LEEfield[finish.first][finish.second]);
        if (field[0][0] != 0 && field[0][0] != 1)
            answer2(start, finish, LEEfield[finish.first][finish.second] * field[0][0]);
        return LEEfield[finish.first][finish.second];
    }



    

    T A_star(pair<int, int> start, pair<int, int> finish)
    {
        vector<pair<int, int>> open;
        vector<vector<bool>> closed;
        closed.assign(field.size(), vector<bool>(field[0].size()));
        vector<vector<T>> Gfield = field;
        vector<vector<T>> Ffield = field;
        int move1[4] = { 1,0,-1,0 };
        int move2[4] = { 0,1,0,-1 };
        for (int i = 0; i < field.size(); i++)
        {
            fill(closed[i].begin(), closed[i].end(), false);
        }
        open.push_back(start);
        Gfield[start.first][start.second] = 0;
        Ffield[start.first][start.second] = Gfield[start.first][start.second] + h_function(start, finish);
        while (open.size() != 0)
        {
            bool tentative_is_better;
            auto curent = open.back();
            for (auto i : open)
            {
                if (Ffield[i.first][i.second] < Ffield[curent.first][curent.second])
                    curent = i;
            }
            if (curent == finish)
            {
                answer2(start, finish, Ffield[finish.first][finish.second]);
                return Ffield[finish.first][finish.second];
            }
            open.erase(remove(open.begin(), open.end(), curent), open.end());
            closed[curent.first][curent.second] = true;
            for (int k = 0; k < 4; ++k)
            {
                int iy = curent.first + move1[k], ix = curent.second + move2[k];
                if((iy>-1 && ix>-1)&& iy< closed.size() && ix< closed[0].size())
                {
                    if (closed[iy][ix] == true)
                        continue;
                    auto tentative_g_score = Gfield[curent.first][curent.second] + field[iy][ix];
                    if (find(open.begin(), open.end(), make_pair(iy, ix)) == open.end())
                    {
                        open.push_back(make_pair(iy, ix));
                        tentative_is_better = true;
                    }
                    else
                    {
                        if (tentative_g_score < Gfield[iy][ix])
                            tentative_is_better = true;
                        else
                            tentative_is_better = false;
                    }

                    if (tentative_is_better == true)
                    {
                        Gfield[iy][ix] = tentative_g_score;
                        Ffield[iy][ix] = Gfield[iy][ix] + h_function(make_pair(iy, ix), finish);
                    }
                }
                
            }
            

        }
        return -1;


    }

    
    template<class T1>
    friend istream& operator>>(istream& s, Way<T1>& A);

};

template<class T1>
istream& operator>>(istream& s, Way<T1>& A)
{
    int length, width;
    s >> length >> width;
    if (A.field.size() != length || A.field[0].size() != width)
        A.resize(length, width);
    T1 walkprice;
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++)
        {
            s >> walkprice;
            A.field[i][j] = walkprice;
        }
    }
	return s;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "\n";
    ifstream fin("D:\\test.txt");
    if (fin.is_open())
    {
        Way<int> T(1, 1);
        fin >> T;
        fin.close();
        T.A_star(make_pair(1, 1), make_pair(1, 3));
        T.A_star(make_pair(2, 2), make_pair(2, 4));
        T.A_star(make_pair(0, 0), make_pair(4, 5));
    }
    Way <int> A(10, 10);
    Way <int> С(39, 41);
    Way<int> D(10, 10, 13);
    Way<int> E(17, 5, 23);
    A.dfs(make_pair(2, 8), make_pair(9, 1));
    A.bfs(make_pair(2, 8), make_pair(9, 1));
    A.lee(make_pair(2, 8), make_pair(9, 1));
    cout << "\n";
    С.dfs(make_pair(1, 8), make_pair(37, 40));
    С.bfs(make_pair(1, 8), make_pair(37, 40));
    С.lee(make_pair(1, 8), make_pair(37, 40));
    cout << "\n";
    D.dfs(make_pair(1, 3), make_pair(7, 9));
    D.bfs(make_pair(1, 3), make_pair(7, 9));
    D.lee(make_pair(1, 3), make_pair(7, 9));
    cout << "\n";
    E.dfs(make_pair(3, 3), make_pair(16, 4));
    E.bfs(make_pair(3, 3), make_pair(16, 4));
    E.lee(make_pair(3, 3), make_pair(16, 4));
}


    



