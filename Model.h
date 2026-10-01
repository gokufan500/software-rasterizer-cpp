#include <vector>
#include "Geometry.h"

// Model 
//
//
class Model {
    std::vector<vec3> vertices = {}; // array of vertices 
    std::vector<int> facet_vrt = {}; // per triangle index in above array 
  public:
    Model(const std::string filename);
    int nverts() const; 
    int nfaces() const; 
    vec3 vert(const int i) const; // 0 <= i < nverts()
    vec3 vert(const int iface, const int nthvert) const; // 0<= iface <= nfaces(), 0 <= nthvert < 3  
};
