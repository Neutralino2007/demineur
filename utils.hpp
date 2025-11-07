#pragma once
#include "types.hpp"
#include <iostream>
#include <vector>

template <class T>
istream & operator >> (istream & is, vector<T> & v );

template <class T>
ostream & operator << (ostream & os, const vector<T> & v);

template <class T>
ostream & operator << (ostream & os, const vector<vector<T>> & m);
