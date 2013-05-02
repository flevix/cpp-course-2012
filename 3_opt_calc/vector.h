#ifndef VECTOR_H
#define VECTOR_H

struct myvector
{
    int length;
    bool in_vector;
    std::vector<unsigned> data;
    unsigned small_data;

    myvector();
    void push_back(unsigned);
    void pop_back();
    void clear();
    int size() const;
    void resize(int);
    bool empty();
    unsigned & back();
    unsigned & front();
    unsigned const & back() const;
    unsigned const & front() const;
    unsigned & operator [] (int);
    unsigned const & operator [] (int) const;
};

#endif // VECTOR_H
