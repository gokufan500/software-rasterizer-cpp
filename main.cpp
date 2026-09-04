#include <GLFW/glfw3.h>
#include <iostream>
#include <GL/gl.h>
#include <cmath>
#include <string>
#include "utils.h"
#include <sstream>
int width = 800;
int height = 600;
unsigned char* pixels = new unsigned char[width * height * 4];

void draw();

struct Vec3 {
  float x,y,z;
};
struct Camera {
  Vec3 position = Vec3{0,0,0};
  Vec3 target = Vec3{0,0,0};
};

Camera cam;

struct color {
  unsigned char r = 0;
  unsigned char g = 0;
  unsigned char b = 0;
  unsigned char a = 255;
};

color red{255,0,0};
color blue{0,0,255};
color green{0,255,0};
color yellow{0,200,255};
color dark_gray{15,15,15}; 
color white{255,255,255};

struct object {
  std::vector<float> vertices;
  std::vector<int>   faces;
  float scale = 1;
};


void setPixel(int x, int y, color c)
{
  if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
  // pixel position is x + width * y * 4
  int pos = (x + width * y) * 4;
  pixels[pos] = c.r;
  pixels[pos + 1] = c.g;
  pixels[pos + 2] = c.b;
  pixels[pos + 3] = c.a;
}

void setPixel3D(int x, int y, int z, color c)
{
  float dx = x - cam.position.x;
  float dy = y - cam.position.y;
  float dz = z - cam.position.z;

  if (z == 0) return;
   
  float screenX = (dx / dz + 1.0f) * width / 2.0f;
  float screenY = (dy / dz + 1.0f) * height / 2.0f;

  setPixel(static_cast<int>(screenX), static_cast<int>(screenY), c);
}

void drawLine(int ax, int ay, int bx, int by, color c)
{
  bool steep = std::abs(ax-bx) < std::abs(ay-by);
  if (steep)
  {
    std::swap(ax,ay);
    std::swap(bx,by);
  }
  if (ax > bx)
  {
    std::swap(ax,bx);
    std::swap(ay,by);
  }
  float y = ay;
  for (int x = ax; x <= bx; x++)
  {
    if (steep)
    {
      setPixel(y,x,c);
    }
    else 
    {
      setPixel(x,y,c);
    }
    y += (by-ay) / static_cast<float>(bx-ax);
  }
}

void drawLine3D(int ax, int ay, int az, int bx, int by, int bz, color c)
{

  // 3d to 2d
  // stop division by 0 
  
  float dx = ax - cam.position.x;
  float dy = ay - cam.position.y;
  float dz = az - cam.position.z;
  float ex = bx - cam.position.x;
  float ey = by - cam.position.y;
  float ez = bz - cam.position.z;
  if (dz == 0 || ez == 0) return;
  if (dz <= 0 && ez <= 0) return;
  float dscreenX = (dx / dz + 1.0f) * width / 2.0f;
  float dscreenY = (dy / dz + 1.0f) * height / 2.0f;

  float escreenX = (ex / ez + 1.0f) * width / 2.0f;
  float escreenY = (ey / ez + 1.0f) * height / 2.0f;


  drawLine(static_cast<int>(dscreenX),
      static_cast<int>(dscreenY),
      static_cast<int>(escreenX),
      static_cast<int>(escreenY),c);
}

void clear(color c)
{

  for (int i = 0; i < width * height * 4; i += 4) {
            pixels[i] = c.r;     // Red
            pixels[i+1] = c.g;     // Green
            pixels[i+2] = c.b;     // Blue
            pixels[i+3] = c.a;   // Alpha
        }
}


object loadObject(std::string filepath)
{
  object o;

  //std::string file = readTextFile(filepath);
  std::ifstream file(filepath);
  if (!file.is_open()){
    return o;
  }
  std::string line;

  
  while(std::getline(file,line))
  {
    std::string type;
    float vx,vy,vz;
    std::stringstream ss(line);
    ss >> type;
    if (type == "v")
    {
      ss >> vx >> vy >> vz;
      o.vertices.emplace_back(vx);
      o.vertices.emplace_back(vy);
      o.vertices.emplace_back(vz);
    } else if (type == "f")
    {
      std::string v1, v2, v3;
      ss >> v1 >> v2 >> v3;
            
      // Extract just the vertex index (before any '/' character)
      int idx1 = std::stoi(v1.substr(0, v1.find('/')));
      int idx2 = std::stoi(v2.substr(0, v2.find('/')));
      int idx3 = std::stoi(v3.substr(0, v3.find('/')));
      
      // OBJ indices are 1-based, convert to 0-based
      o.faces.emplace_back(idx1 - 1);
      o.faces.emplace_back(idx2 - 1);
      o.faces.emplace_back(idx3 - 1);
    }
  }
  file.close();
  std::cout << "Loaded " << o.vertices.size()/3 << " vertices and " 
              << o.faces.size()/3 << " faces" << std::endl;
  return o;
}

void drawWireframe(object o)
{
  for (int i = 0; i < (o.faces.size() / 3); i+=3)
  {
    // triangle 
    int va = o.faces[i];
    int vb = o.faces[i+1];
    int vc = o.faces[i+2];

    float vax = o.scale * o.vertices[va * 3];
    float vay = o.scale * o.vertices[va * 3 +1];
    float vaz = o.scale * o.vertices[va * 3 +2];

    float vbx = o.scale * o.vertices[vb * 3];
    float vby = o.scale * o.vertices[vb * 3+1];
    float vbz = o.scale * o.vertices[vb*3+2];

    float vcx = o.scale * o.vertices[vc*3];
    float vcy = o.scale * o.vertices[vc*3+1];
    float vcz = o.scale * o.vertices[vc*3+2];
    drawLine3D(static_cast<int>(vax), static_cast<int>(vay), static_cast<int>(vaz),static_cast<int>(vbx), static_cast<int>(vby), static_cast<int>(vbz), red);
    drawLine3D(static_cast<int>(vbx), static_cast<int>(vby), static_cast<int>(vbz),static_cast<int>(vcx), static_cast<int>(vcy), static_cast<int>(vcz), red);
    drawLine3D(static_cast<int>(vax), static_cast<int>(vay), static_cast<int>(vaz),static_cast<int>(vcx), static_cast<int>(vcy), static_cast<int>(vcz), red);
  }

  for (int i = 0; i < (o.vertices.size() / 3); i+=3)
  {
    //std::cout << element << " ";
    float x = o.vertices[i];
    float y = o.vertices[i+1];
    float z = o.vertices[i+2];

    // values are below 1 
    x *= o.scale;
    y *= o.scale;
    z *= o.scale;
    setPixel3D(static_cast<int>(x),static_cast<int>(y),static_cast<int>(z),white);
  }
  
    
}


int main() 
{
  if (!glfwInit()) 
  {
    return -1;
  }

  GLFWwindow* window = glfwCreateWindow(width, height, "sr", nullptr,nullptr);
  if(!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  object diablo = loadObject("./objects/diablo3_pose.obj"); 
  diablo.scale = 200;
  
  object cube = loadObject("./objects/cube.obj");
  cube.scale = 200;
  while (!glfwWindowShouldClose(window)) {
    
    /// draw 

    clear(dark_gray);
  
    cam.position = Vec3{0, 0, -300};

    //drawWireframe(diablo);
    drawWireframe(cube);


    ////////////
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(-1.0f,-1.0f);
    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glPixelZoom(1.0f, 1.0f);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  delete[] pixels;
  glfwTerminate();
  return 0;
}

