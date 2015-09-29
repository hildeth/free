// Draw a sphere to an object file rendered by Irrlicht
#include <stdio.h>
#include <irrlicht.h>
#include <string>
#include <math.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

inline int phi_resolution(int i,int resolution){return ceil(resolution*sin(i*PI/(2*resolution)))+1;}

// Prints a unit sphere, with resolution points between the intersections with the unit cube.
path print_sphere(int resolution, double size)
{
  char buffer [L_tmpnam];
  mkstemp(buffer);
  FILE* sphereobj;
  std::string bluffer(buffer);
  std::string sluffer=bluffer+".obj";
  char snuffer [L_tmpnam+4];
  strcpy(snuffer,sluffer.c_str());
  sphereobj = fopen(snuffer,"w");
  float x,y,z;
  int count=0;
  int depth[resolution+2]={0};
  /*
    Dynamic Sphere Generation Methods
  */
  /*

  //Method - The Terrible Symmetry of Spheres (BH)

  for(int i=0;i<resolution;i++){
    for(int j=0;j<resolution/sqrt(2)-sqrt(i)*sqrt(resolution);j++){
      x=i/resolution;
      y=j/resolution;
      ++depth[i];
      z=sqrt(1-x*x-y*y);
      fprintf(sphereobj,"v %f %f %f\n",x,y,z);
      fprintf(sphereobj,"v %f %f %f\n",y,z,x);
      fprintf(sphereobj,"v %f %f %f\n",z,x,y);
    }
  }
  for(int i=1; i<resolution;i++){
    for(int j=0; j<depth[i-1];j++){
    }
  }
  */
  /*

  //Method - Spheres are just pretty much cubes, almost (TH)

  for(int i=0;i<=resolution;i++){
    float angle=PI*i/(resolution*2);
    z=cos(angle);
    for(int j=0;j<=phi_resolution(i,resolution);j++){
      float fangle=PI*j/(resolution*2);
      x=sin(fangle)*sin(angle);
      y=cos(fangle)*sin(angle);
      fprintf(sphereobj,"v %f %f %f\n",x,y,z);
    }
  }

  */

   //Method - Quantization Party! (BH)

  for(int i=0;i<=resolution;i++){
    z=cos(i*PI/(2*resolution))*size;
    for(int j=0;j<=4*(i+1);j++){
      float angle=(j*PI)/(2*i+2);
      ++count;
      x=sin(angle)*sin(i*PI/(2*resolution))*size;
      y=cos(angle)*sin(i*PI/(2*resolution))*size;
      fprintf(sphereobj,"v %f %f %f\n",x,y,z);
    }
    depth[i+1]=count;
  }
  for(int i=1;i<=resolution;i++){
    for(int j=1;j<=depth[i];j++){
      fprintf(sphereobj,"f %i %i %i\n",depth[i-1]+j,depth[i]+j,depth[i-1]+j%depth[i]+1);
      fprintf(sphereobj,"f %i %i %i\n",depth[i]+j,depth[i-1]+j,depth[i]+j%depth[i]+1);
    }
  }

  fclose(sphereobj);
  return buffer;
}

int main()
{
  IrrlichtDevice *device = createDevice(video::EDT_SOFTWARE, dimension2d<u32>(640,480),16,false,false,false,0);
  if(!device)
    return 1;
  device->setWindowCaption(L"Irrlicht Sphere Renderer");
  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();
  guienv->addStaticText(L"Have a sphere. No choices here.",rect<s32>(10,10,260,22),true);
  path Sphere=print_sphere(10,100);
  IMesh* mesh = smgr->getMesh(Sphere+".obj");
  if (!mesh)
  {
    device->drop();
    return 1;
  }
  IMeshSceneNode* node = smgr->addMeshSceneNode( mesh );
  if (node)
  {
    node->setMaterialFlag(EMF_LIGHTING, false);
  }
  //smgr->addCameraSceneNode(0,vector3df(2.5,5,-5),vector3df(0,0,0));
  smgr->addCameraSceneNodeFPS();
  device->getCursorControl()->setVisible(false);
  while(device->run())
  {
    if(device->isWindowActive())
    {
      driver->beginScene(true,true,SColor(255,100,101,140));
      smgr->drawAll();
      guienv->drawAll();
      driver->endScene();
    }
    else{
      device->yield();
    }
  }
  device->drop();
  return 0;
}
