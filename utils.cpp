#include "utils.hpp"

template <class T>
istream & operator >> (istream & is, vector<T> & v ){
    int n; is >> n;
    v.resize(n);
    for(int i=0; i<n; i++)
        is >> v[i];
    return is;
}

template <class T>
ostream & operator << (ostream & os, const vector<T> & v){
    int n=v.size();
    for(int i=0; i<n;++i)
        os<<v[i]<<" ";
    return os;
}

template <class T>
ostream & operator << (ostream & os, const vector<vector<T>> & m){
    int n=m.size();
    for(int i=0; i<n;++i)
        os<<m[i]<<'\n';
    return os;
}
