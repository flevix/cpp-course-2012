/* 
 * File:   main.cpp
 * Author: flevix
 *
 * Created on 10 Март 2013 г., 14:54
 */
#include <vector>
#include <chrono>
#include <future>
#include <algorithm>
#include <iostream>


using namespace std;

/*
 * 
 */

void quick_sort_tr(vector<int>::iterator begin, vector<int>::iterator end, int depth) {
    auto const size = end - begin;
    if (size <= 1) return;
    auto pivot = begin + size / 2;
    auto const pivot_v = *pivot;
    
    swap(*pivot, *(end - 1));
    auto p = partition(begin, end, [&](const int& a) {
        return a < pivot_v;
    });
    swap(*p, *(end - 1));
    if (depth < 3 && size > 16384) {
        depth++;
        auto const size1 = p - begin;
        auto const size2 = end - (p + 1);
        if (size1 <= size2) {
            auto start = chrono::high_resolution_clock::now();
            cout << "start: " << depth << endl;
            auto temp = async(launch::async, [&]() {
                cout << size1 << endl;
                quick_sort_tr(begin, p, depth);
                });
            quick_sort_tr(p + 1, end, depth);
            auto stop = chrono::high_resolution_clock::now();
            cout << "stop: " << depth << " duration: " << chrono::duration<double>(stop - start).count() << endl;
        } else {
            auto start = chrono::high_resolution_clock::now();
            cout << "start: " << depth << endl;
            auto temp = async(launch::async, [&]() {
                cout << size2 << endl;
                quick_sort_tr(p + 1, end, depth);
                });
            quick_sort_tr(begin, p, depth);
            auto stop = chrono::high_resolution_clock::now();
            cout << "stop: " << depth << " duration: " << chrono::duration<double>(stop - start).count() << endl;
        }
        
    } else {
        quick_sort_tr(begin, p, depth);
        quick_sort_tr(p + 1, end, depth);
    }
}

void quick_sort_ntr(vector<int>::iterator begin, vector<int>::iterator end) {
    auto const size = end - begin;
    if (size <= 1) return;
    auto pivot = begin + size / 2;
    auto const pivot_v = *pivot;
    
    swap(*pivot, *(end - 1));
    auto p = partition(begin, end, [&](const int& a) {
        return a < pivot_v;
    });   
    swap(*p, *(end - 1));
    quick_sort_ntr(begin, p);
    quick_sort_ntr(p + 1, end);
}

void quick_sort_treads (vector<int>& arr) {
    quick_sort_tr(arr.begin(), arr.end(), 0);
}
void quick_sort_utreads (vector<int>& arr) {
    quick_sort_ntr(arr.begin(), arr.end());
}

void f()
{
    for (;;);
}

int main() {
//    thread th1(&f);
//    thread th2(&f);
//    thread th3(&f);
//    for (;;);
    int n = 1000 * 1000 * 50;
    vector<int> array;
    vector<int> a1, a2, a3;
    for (int i = 0; i < n; i++) {
        array.push_back(i);
    }
    random_shuffle(array.begin(), array.end());;
    a1 = array;
    a2 = array;
    a3 = array;
    
    auto start = chrono::high_resolution_clock::now();
    sort(a1.begin(), a1.end());
    auto stop = chrono::high_resolution_clock::now();
    cout << "duration std::sort: " << chrono::duration<double>(stop - start).count() << endl;
    
    auto start2 = chrono::high_resolution_clock::now();
    quick_sort_treads(a2);
    auto stop2 = chrono::high_resolution_clock::now();
    cout << "duration threads: " << chrono::duration<double>(stop2 - start2).count() << endl;
    
    auto start3 = chrono::high_resolution_clock::now();
    quick_sort_utreads(a3);
    auto stop3 = chrono::high_resolution_clock::now();
    cout << "duration: " << chrono::duration<double>(stop3 - start3).count() << endl;
    if (a1 == a2 && a2 == a3) {
        cout << "ok: " << endl;
    } else {
        cout << "fail" << endl;
    }
    return 0;
}

