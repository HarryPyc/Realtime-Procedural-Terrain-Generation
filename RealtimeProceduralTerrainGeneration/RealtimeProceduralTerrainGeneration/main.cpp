#include <windows.h>

#include <GL/glew.h>

#include <GL/freeglut.h>

#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "imgui_impl_glut.h"
#include "VideoMux.h"
#include "DebugCallback.h"
#include "GenerateTerrain.h"
#include "Camera.h"
#include "FPSController.h"

//names of the shader files to load
static const std::string vertex_shader("template_vs.glsl");
static const std::string fragment_shader("template_fs.glsl");

GLuint shader_program = -1;
static const std::string texGrass_name = "textures/T_Ground_Grass_1_BC_R.tga";
static const std::string texRock_name = "textures/T_Ground_Rock_2_BC_R.tga";
static const std::string texRock2_name = "textures/T_Ground_Rock_5_BC_R.tga";
static const std::string texSnow_name = "textures/T_Ground_Snow_1_BC_R.tga";
static const std::string texMud_name = "textures/T_Ground_Mud_1_BC_R.tga";

GLuint texGrass = -1;
GLuint texRock = -1;
GLuint texRock2 = -1; 
GLuint texSnow = -1;
GLuint texMud = -1;

float time_sec = 0.0f;
float slider = 0.0f;

GLuint vao = -1;
vec3 lightDir(-1, -1, -1);

bool useLight = true;

Camera* camera;
FPSController* controller;

Terrain* terrain;
float ratio = 0.667f;
bool bThermal = true;
int thermalTime = 10;
int hydraulicTime = 1000;
bool bHydraulic = true;
//Draw the user interface using ImGui
void draw_gui()
{
   ImGui_ImplGlut_NewFrame();

   //create a slider to change the angle variables
   ImGui::SliderFloat3("LightDir", &lightDir[0],-1.0f, 1.0f);
   
   ImGui::SliderFloat("1/f Noise:Voronoi", &ratio, 0.0f, 1.0f);
   //ImGui::Checkbox("Enable Turbulence", &enableTurb);
   //ImGui::SliderFloat("Turbulence", &c, 0.0f, 5.f);
   ImGui::Checkbox("Thermal Erosion", &bThermal);
   ImGui::SliderInt("Thermal Times", &thermalTime, 0, 100);
   if (ImGui::Button("GetThermalErosion")) {
       if (bThermal) {
           terrain->thermal(thermalTime);
           vao = create_terrain_vao(&terrain->v, terrain->N);
       }
   }
   ImGui::SameLine();
   std::string totalTimes = "Total Times:" + std::to_string(terrain->Tcount);
   ImGui::Text(totalTimes.c_str());
   ImGui::Checkbox("Hydraulic Erosion", &bHydraulic);
   ImGui::SliderInt("Hydraulic Times", &hydraulicTime, 0, 1000);
   if (ImGui::Button("GetHydraulicErosion")) {
       if (bHydraulic) {
           terrain->hydraulic(hydraulicTime);
           vao = create_terrain_vao(&terrain->v, terrain->N);
       }
   }
   ImGui::SameLine();
   std::string HtotalTimes = "Total Times:" + std::to_string(terrain->Hcount);
   ImGui::Text(HtotalTimes.c_str());
   ImGui::Spacing();
   if (ImGui::Button("Generate Terrain")) {
       terrain->update(ratio, bThermal, thermalTime, bHydraulic, hydraulicTime);
       vao = create_terrain_vao(&terrain->v, terrain->N);
   }
   if (ImGui::Button("Generate New Terrain")) {
       delete terrain;
       terrain = new Terrain(ratio, bThermal, thermalTime, bHydraulic, hydraulicTime);
       vao = create_terrain_vao(&terrain->v, terrain->N);
   }
   ImGui::Checkbox("UseTexture", &useLight);

   ImGui::Text("use wasd and qe to move");
   ImGui::Text("press m to release cursor");
   ImGui::Render();
 }

// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
   //clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glUseProgram(shader_program);


   glm::mat4 M = glm::rotate(slider, glm::vec3(0.0f, 1.0f, 0.0f));

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texGrass);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texRock);
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, texRock2);
   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE_2D, texSnow);
   glActiveTexture(GL_TEXTURE4);
   glBindTexture(GL_TEXTURE_2D, texMud);
   int texGrass_loc = glGetUniformLocation(shader_program, "grass");
   if (texGrass_loc != -1)
   {
	   glUniform1i(texGrass_loc, 0); // we bound our texture to texture unit 0
   }
   int texRock_loc = glGetUniformLocation(shader_program, "rock");
   if (texRock_loc != -1)
   {
	   glUniform1i(texRock_loc, 1); // we bound our texture to texture unit 0
   }
   int texRock2_loc = glGetUniformLocation(shader_program, "rock2");
   if (texRock2_loc != -1)
   {
	   glUniform1i(texRock2_loc, 2); // we bound our texture to texture unit 0
   }
   int texSnow_loc = glGetUniformLocation(shader_program, "snow");
   if (texSnow_loc != -1)
   {
	   glUniform1i(texSnow_loc, 3); // we bound our texture to texture unit 0
   }
   int texMud_loc = glGetUniformLocation(shader_program, "mud");
   if (texMud_loc != -1)
   {
       glUniform1i(texMud_loc, 4); // we bound our texture to texture unit 0
   }

   int light_loc = glGetUniformLocation(shader_program, "light");
   if (light_loc != -1) {
	   glUniform3fv(light_loc, 1, &lightDir[0]);
   }
   
   camera->upload(shader_program);
   int M_loc = glGetUniformLocation(shader_program, "M");
   if (M_loc != -1)
   {
	   glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
   }
   int useLight_loc = glGetUniformLocation(shader_program, "useLight");
   if (useLight_loc) {
	   glUniform1f(useLight_loc, useLight);
   }
   glUniform1f(glGetUniformLocation(shader_program, "hMin"), terrain->hMin);
   glUniform1f(glGetUniformLocation(shader_program, "hMax"), terrain->hMax);

   if(vao != -1)
     DrawTerrain(vao, terrain->N);
         
   draw_gui();
   controller->update();

   glutSwapBuffers();
}

// glut idle callback.
//This function gets called between frames
void idle()
{
	glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   time_sec = 0.001f*time_ms;
}

void reload_shader()
{
   GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());

   if(new_shader == -1) // loading failed
   {
      glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
   }
   else
   {
      glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

      if(shader_program != -1)
      {
         glDeleteProgram(shader_program);
      }
      shader_program = new_shader;

   }
}

// Display info about the OpenGL implementation provided by the graphics driver.
// Your version should be > 4.0 for CGT 521 
void printGlInfo()
{
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
}

void initOpenGl()
{
   //Initialize glew so that new OpenGL function names can be used

   glewInit();
   //RegisterCallback();
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_PRIMITIVE_RESTART);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   reload_shader();
   texGrass = LoadTexture(texGrass_name.c_str());
   texRock = LoadTexture(texRock_name.c_str());
   texRock2 = LoadTexture(texRock2_name.c_str());
   texSnow = LoadTexture(texSnow_name.c_str());
   texMud = LoadTexture(texMud_name.c_str());
   terrain = new Terrain(ratio, bThermal, thermalTime, bHydraulic, hydraulicTime);
   vao = create_terrain_vao(&terrain->v, terrain->N);

}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
    controller->keyboard(key, x, y);
}

void keyboard_up(unsigned char key, int x, int y)
{
    controller->keyboard_up(key, x, y);
}

void special_up(int key, int x, int y)
{
    controller->special_up(key, x, y);
}

void passive(int x, int y)
{
    controller->passive(x, y);
}

void special(int key, int x, int y)
{
    controller->special(key, x, y);
}

void motion(int x, int y)
{
    controller->motion(x, y);
}

void mouse(int button, int state, int x, int y)
{
    controller->mouse(button, state, x, y);
}


int main (int argc, char **argv)
{
#if _DEBUG
	glutInitContextFlags(GLUT_DEBUG);
#endif
	glutInitContextVersion(4, 3);
   //Configure initial window state using freeglut
   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (1280, 720);
   int win = glutCreateWindow ("Realtime Procedural Terrain Generation");

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutKeyboardUpFunc(keyboard_up);
   glutSpecialUpFunc(special_up);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(passive);

   glutIdleFunc(idle);

   initOpenGl();
   ImGui_ImplGlut_Init(); // initialize the imgui system
   camera = new Camera(glm::vec3(0,5,5));
   controller = new FPSController(camera, 1280, 720);

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}


