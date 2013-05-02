#include <algorithm>
#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;

typedef pair<int, int> point;

bool rotate(const point &a, const point &b, const point &c) //проверка поворота
{
    return (b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first) < 0;
}

vector<point> convexHull(vector<point> p, int n)    //построение выпуклой оболочки
{
    if (n <= 1) //бессмысленно, что-то делать, если всего одна точка
        return p;
    int k = 0;  //счётчик точек
    sort(p.begin(), p.end());   //сортируем стабильной сортировкой
    vector<point> temp(n * 2);  //временный вектор, для построения оболочки

    for (int i = 0; i < n; temp[k++] = p[i++])  //добавляем i-ую точку
        while (k >= 2 && !rotate(temp[k - 2], temp[k - 1], p[i]))
            --k;    //и проверяем сколько точек можно исключить, взяв следующую
    for (int i = n - 2, t = k; i >= 0; temp[k++] = p[i--])
        while (k > t && !rotate(temp[k - 2], temp[k - 1], p[i]))
            --k;    //аналогично для верхней границы оболочки
    temp.resize(k - 1 - (temp[0] == temp[1]));  //(temp[0] == temp[1]) уместно, если у нас только две одинаковых точки
    return temp;
}

char * file_name_build (char * p, int x, const char * src)  //функция для перебора названий файлов
{
    char * t = p;   //указатель на начало массива символов
    if (x > 9) {    //так число тестов == 40, можно воспользоваться таким костылём ->
        *p++ = x / 10 + '0';    // -> чтобы не переворачивать число
    }
    *p++ = x % 10 + '0';
    while ((*p++ = *src++));
    return t;
}


int main()
{
    for (int z = 1; z <= 40; z++)   //всего есть 40 тестов
    {
        char _in[8];    //максимум может быть 5 + нулевой символ, но если степень двойки, то работает быстрее
        FILE * input =  fopen(file_name_build(_in, z, ".in"), "r"); //z.in -- входные данные
        int n;  //считывает количество точек
        fscanf (input, "%d", &n);
        vector<point> points(n);
        for (int i = 0; i < n; i++) {   //считываем сами точки
            fscanf (input, "%d%d", &points[i].first, &points[i].second);
        }
        vector<point> hull = convexHull(points, n); //строим выпулую оболочку
        fclose(input);  //закрываем файл с входными данными
        char _out[8];
        input = fopen(file_name_build(_out, z, ".out"), "r");   //z.out -- правильная выпуклая оболочка
        fscanf (input, "%d", &n);   //n == количество точек в правильной выпуклой оболочке
        if ((size_t) n != hull.size())
        {
            cout << "In " << z << " test, size correct convex_hull != size my convex_hull" << endl;
            return 0;   //если размер не совпал выходим
        }
        for (int i = 0; i < n; i++)
        {
            int x, y;   //объявляем переменные, как можно локальнее
            fscanf (input, "%d%d", &x, &y); //x и y -- i-ая точка правильной выпуклой оболочки
            if (hull[i].first != x || hull[i].second != y)
            {
                cout << z << " convex_hull was built unsuccessful." << endl;
                return 0;   //если i-ая точка не совпала, то выходим
            }
        }
        cout << z << " test check was successful." << endl;
        fclose(input);  //закрываем файл проверки
    }
}
