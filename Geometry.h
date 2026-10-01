#pragma once 
#include <cmath> 
#include <cassert>
#include <iostream>

// int n means we can template vectors of integer size 
template<int n> struct vec {
  // array for storing 
  double data[n] = {0};
  // write overload 
  // v[0] = 5;
  double& operator[](const int i)       { assert(i>=0 && i<n); return data[i];};
  // read overload
  // double a = v[0];
  double  operator[](const int i) const { assert(i>=0 && i<n); return data[i];};
};

template<> struct vec<3> {
  double x = 0, y = 0, z = 0;
  // i ? (1==i ? y : z) : x 
  // if i non zero, then check if i = 1, if i = 1 return y, if not 1 but not 0, return z, else return 0.
  double& operator[](const int i)       {assert(i>=0 && i<3); return i ? (1==i ? y : z) : x;}
  double  operator[](const int i) const {assert(i>=0 && i<3); return i ? (1==i ? y : z) : x;}
};


// printer for vec 
template<int n> std::ostream& operator<<(std::ostream& out, const vec<n>& v){
  for (int i = 0; i < n; i++) out << v[i] << " ";
  return out;
}
typedef vec<3> vec3;
