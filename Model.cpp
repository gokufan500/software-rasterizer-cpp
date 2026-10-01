/*
class Model 
    std::vector<vec3> vertices = {}; // array of vertices 
    std::vector<int> facet_vrt = {}; // per triangle index in above array 
  public:
    Model(const std::string filename);
    int nverts() const; 
    int nfaces() const; 
    vec3 vert(const int i) const; // 0 <= i < nverts()
    vec3 vert(const int iface, const int nthvert) const; // 0<= iface <= nfaces(), 0 <= nthvert < 3  
*/
#include "Model.h"
#include <sstream>
#include <fstream>
Model::Model(const std::string filename) {

  std::ifstream file(filename);
  if (!file.is_open()){
    return;
  }
  std::string line;

  while(std::getline(file,line))
  {
    std::string type;
    std::stringstream ss(line);
    ss >> type;
    if (type == "v")
    {
      vec3 v; 
      ss >> v[0] >> v[1] >> v[2];
      vertices.push_back(v); 
    } else if (type == "f")
    {
      std::string v1, v2, v3;
      ss >> v1 >> v2 >> v3;
            
      // Extract just the vertex index (before any '/' character)
      int idx1 = std::stoi(v1.substr(0, v1.find('/')));
      int idx2 = std::stoi(v2.substr(0, v2.find('/')));
      int idx3 = std::stoi(v3.substr(0, v3.find('/')));
      
      // OBJ indices are 1-based, convert to 0-based
      facet_vrt.emplace_back(idx1 - 1);
      facet_vrt.emplace_back(idx2 - 1);
      facet_vrt.emplace_back(idx3 - 1);
    }
  }
  file.close();
  std::cout << "Loaded " << nverts() << " vertices and " 
              << nfaces() << " faces" << std::endl;
}

int Model::nverts() const  { return vertices.size();}
int Model::nfaces() const  { return facet_vrt.size()/3;} 

vec3 Model::vert(const int i) const {
  return vertices[i];
}

vec3 Model::vert(const int iface, const int nthvert) const {
  return vertices[facet_vrt[iface*3+nthvert]];
}
