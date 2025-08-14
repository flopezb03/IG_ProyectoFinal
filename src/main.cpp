#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

void configScene();
void renderScene();
void setLights (glm::mat4 P, glm::mat4 V);
void setDirLights();
void setPosLights();
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCinta(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCabina(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCabina3(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSalida(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawEntrada(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPrensa(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawGrua(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawEsferas(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, double y);
void drawRuedas(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, bool rot);
void drawChasis(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, double y, double z, float rot, bool painted);
void drawCoche(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, double y);
void drawFoco(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, double z);
void drawVentana(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos      (GLFWwindow* window, double xpos, double ypos);
void funAnimacionesTimer(double seconds, double &frames, double &timer1s, double &timer3s, bool &rest);
void timerEsferas(bool &bajar, double &x, double &y);
void timerRuedas(bool &rot, double &x);
void timerChasis(int &fase, double &x, double &y, double &z, float &rot, bool &painted);
void timerChasisRot(int fase, double &x, double &y, double &z, float &rot);
void timerCoche(bool &bajar, double &x, double &y);
void animacionEsferas(bool bajar, double &x, double &y);
void animacionRuedas(bool rot, double &x);
void animacionPuertas(double time, double &puertasY);
void animacionPrensa(double time, double &prensaY);
void animacionChasis(int &fase, double &x, double &y, double &z, bool &painted);
void animacionGrua1(float &angle);
void animacionGrua2(double time, float &angle);
void animacionChasisRot(int &chasisFase, float &chasisRot);
void animacionCoche(bool bajar, double &x, double &y);
void animacionTrampillaAbrir(double time, double &x);
void animacionTrampillaCerrar(double &x);
void funLucesTimer(double seconds, double &frames,double &timer0_5s,bool &on);



// Shaders
   Shaders shaders;

// Modelos
   Model sphere;
   Model plane;
   Model cube;
   Model cylinder;
   Model carChassis;
   Model wheel;

// Imagenes (texturas)
   Texture imgNoEmissive;
   Texture imgGold;
   Texture imgMetal;
   Texture imgMetal2;
   Texture imgCinta;
   Texture imgDoor;
   Texture imgGoma;
   Texture imgCaution;
   Texture imgConcrete;

// Luces y materiales
   #define   NLD 5
   #define   NLP 11
   #define   NLF 1
   Light     lightG;
   Light     lightD[NLD];
   Light     lightP[NLP];
   Light     lightF[NLF];
   Material  mluz;
   Material  ruby;
   Material glass;
   Material cooper;
   Material redPlastic;
   Material whitePlastic;
   Material blackPlastic;
   Textures  texGold;
   Textures texMetal;
   Textures texMetal2;
   Textures texCinta;
   Textures texDoor;
   Textures texGoma;
   Textures texCaution;
   Textures texConcrete;

// Viewport
   int w = 500;
   int h = 500;

// Animaciones
   double movSpeed = 1.5*1.0/60.0;

   double esferasX[] = {-5.0,-5.0};
   double esferasY[] = {10.15,1.15};
   bool esferasBajar[] = {true,false};

   double ruedasX[] = {-2.0,1.0};
   bool ruedasRot[] = {false,true};

   double puertasY = 0.9;

   double prensaY = 1.5;

   double chasisX[] = {-2.0,-2.0,1.0,3.0};
   double chasisY[] = {10.15,1.15,1.15,1.15};
   double chasisZ[] = {-1.5,-1.5,-1.5,-1.5};
   int chasisFase[] = {0,1,2,3};
   bool chasisPaint[] = {false,false,true,true};
   float chasisRot[] = {0.0,0.0,0.0,0.0};

   float gruaRot = 0.0;

   double cocheX[] = {4.0,7.0};
   double cocheY[] = {1.15,-1.15};
   bool cocheBajar[] = {false,true};

   double trampillaX = 0.0;

   double focoX = 0.0;
   double focoZ = 0.0;
   
   //Luces
   bool focalOn = true;
   
   float dirIntensity = 1.0f;
   bool dirOn = true;

   float posIntensity = 0.0f;

   bool emergencyMode = false;

   // Movimiento de camara
   float fovy   = 60.0;
   float alphaX =  0.0;
   float alphaY =  0.0;

int main() {

// Inicializamos GLFW
   if(!glfwInit()) return -1;
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

// Creamos la ventana
   GLFWwindow* window;
   window = glfwCreateWindow(w, h, "Proyecto Final", NULL, NULL);
   if(!window) {
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSwapInterval(0);

// Inicializamos GLEW
   glewExperimental = GL_TRUE;
   GLenum err = glewInit();
   if(GLEW_OK != err) {
      std::cout << "Error: " << glewGetErrorString(err) << std::endl;
      return -1;
   }
   std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
   const GLubyte *oglVersion = glGetString(GL_VERSION);
   std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

// Configuramos los CallBacks
   glfwSetFramebufferSizeCallback(window, funFramebufferSize);
   glfwSetKeyCallback      (window, funKey);
   glfwSetScrollCallback   (window, funScroll);
   glfwSetCursorPosCallback(window, funCursorPos);

// Entramos en el bucle de renderizado
   configScene();
   double frames1 = glfwGetTime();
   double timer1s = frames1;
   double timer3s = frames1;
   bool rest = true;
   double frames2 = glfwGetTime();
   double timer0_5 = frames2;
   bool on = true;
   while(!glfwWindowShouldClose(window)) {
      renderScene();
      glfwSwapBuffers(window);
      glfwPollEvents();

      funAnimacionesTimer(1.0/60.0,frames1,timer1s,timer3s,rest);
      funLucesTimer(1.0/5.0,frames2,timer0_5,on);
   }
   glfwDestroyWindow(window);
   glfwTerminate();

   return 0;
}
void funAnimacionesTimer(double seconds, double &frames, double &timer1s, double &timer3s, bool &rest){
   if(glfwGetTime()-frames > seconds){
      if(rest){
         double time = glfwGetTime()-timer1s;
         if(time>1.0){
            // Final Fase de Descanso
            timerChasisRot(chasisFase[0],chasisX[0],chasisY[0],chasisZ[0],chasisRot[0]);
            timerChasisRot(chasisFase[1],chasisX[1],chasisY[1],chasisZ[1],chasisRot[1]);
            timerChasisRot(chasisFase[2],chasisX[2],chasisY[2],chasisZ[2],chasisRot[2]);
            timerChasisRot(chasisFase[3],chasisX[3],chasisY[3],chasisZ[3],chasisRot[3]);

            rest = false;
            timer3s = glfwGetTime();
         }else{
            // Fase de Descanso
            animacionGrua1(gruaRot);
            animacionChasisRot(chasisFase[0],chasisRot[0]);
            animacionChasisRot(chasisFase[1],chasisRot[1]);
            animacionChasisRot(chasisFase[2],chasisRot[2]);
            animacionChasisRot(chasisFase[3],chasisRot[3]);
            animacionTrampillaAbrir(time,trampillaX);

         }
      }else{
         double time = glfwGetTime()-timer3s;
         if(time>3.0){
            // Final Fase de movimiento
            timerEsferas(esferasBajar[0],esferasX[0],esferasY[0]);
            timerEsferas(esferasBajar[1],esferasX[1],esferasY[1]);
            timerRuedas(ruedasRot[0],ruedasX[0]);
            timerRuedas(ruedasRot[1],ruedasX[1]);
            timerChasis(chasisFase[0],chasisX[0],chasisY[0],chasisZ[0],chasisRot[0],chasisPaint[0]);
            timerChasis(chasisFase[1],chasisX[1],chasisY[1],chasisZ[1],chasisRot[1],chasisPaint[1]);
            timerChasis(chasisFase[2],chasisX[2],chasisY[2],chasisZ[2],chasisRot[2],chasisPaint[2]);
            timerChasis(chasisFase[3],chasisX[3],chasisY[3],chasisZ[3],chasisRot[3],chasisPaint[3]);
            timerCoche(cocheBajar[0],cocheX[0],cocheY[0]);
            timerCoche(cocheBajar[1],cocheX[1],cocheY[1]);

            rest = true;
            timer1s = glfwGetTime();
         }else{
            // Fase de Movimientos
            animacionEsferas(esferasBajar[0],esferasX[0],esferasY[0]);
            animacionEsferas(esferasBajar[1],esferasX[1],esferasY[1]);
            animacionRuedas(ruedasRot[0],ruedasX[0]);
            animacionRuedas(ruedasRot[1],ruedasX[1]);
            animacionPuertas(time,puertasY);
            animacionPrensa(time,prensaY);
            animacionChasis(chasisFase[0],chasisX[0],chasisY[0],chasisZ[0],chasisPaint[0]);
            animacionChasis(chasisFase[1],chasisX[1],chasisY[1],chasisZ[1],chasisPaint[1]);
            animacionChasis(chasisFase[2],chasisX[2],chasisY[2],chasisZ[2],chasisPaint[2]);
            animacionChasis(chasisFase[3],chasisX[3],chasisY[3],chasisZ[3],chasisPaint[3]);
            animacionGrua2(time,gruaRot);
            animacionCoche(cocheBajar[0],cocheX[0],cocheY[0]);
            animacionCoche(cocheBajar[1],cocheX[1],cocheY[1]);
            animacionTrampillaCerrar(trampillaX);
         }
      }
      frames = glfwGetTime();
   }
}
void timerEsferas(bool &bajar, double &x, double &y){
   if(bajar)
      bajar = false;
   else{
      bajar = true;
      x = -5.0;
      y = 10.15;
   }
}
void timerRuedas(bool &rot, double &x){
   if(rot){
      rot = false;
      x = -2.0;
   }else
      rot = true;
}
void timerChasis(int &fase, double &x, double &y, double &z, float &rot, bool &painted){
   
   if(fase==0){
      fase = 1;
   }else if(fase==1){
      fase = 2;
      painted = true;
   }else if(fase==2){
      fase = 3;
   }else if(fase==3){
      fase=0;
      x = -2.0;
      y = 10.15;
      z = -1.5;
      rot = 0.0;
      painted = false;
   }
}
void timerChasisRot(int fase, double &x, double &y, double &z, float &rot){
   if(fase==3){
      x = 4.0;
      y = 1.15;
      z = -0.5;
      rot = 90.0;
   }
}
void timerCoche(bool &bajar, double &x, double &y){
   if(bajar){
      bajar = false;
      x = 4.0;
      y = 1.15;
   }else
      bajar = true;
}
void animacionEsferas(bool bajar, double &x, double &y){
   if(bajar){
      if(y > 1.15)
         y -= 5*movSpeed;
      else
         y = 1.15;
   }else{
      if(x < -2.0)
         x += movSpeed;
      else{
         x = -2.0;
      }
   }
}
void animacionRuedas(bool rot, double &x){
   if(rot){
      if(x < 4.0)
         x += movSpeed;
      else
         x = 4.0;
   }else{
      if(x < 1.0)
         x += movSpeed;
      else
         x = 1.0;
   }
}
void animacionPuertas(double time, double &puertasY){
   if(time>2.5){
      if(puertasY>0.45)
         puertasY -= 1.25*movSpeed;
      else
         puertasY = 0.45;
   }else if(time<0.5){
      if(puertasY<1.15)
         puertasY += 1.25*movSpeed;
      else
         puertasY = 1.15;
   }
}
void animacionPrensa(double time, double &prensaY){
   if(time>2.75){
      if(prensaY>0.0)
         prensaY -= 10*movSpeed;
      else
         prensaY = 0.0;
   }else if(time<1.0){
      if(prensaY<1.5)
         prensaY += 2.5*movSpeed;
      else
         prensaY = 1.5;
   }
}
void animacionChasis(int &fase, double &x, double &y, double &z, bool &painted){
   if(fase==0){
      if(y>1.15)
         y -= 5*movSpeed;
      else
         y = 1.15;
   }else if(fase==1){
      if(x < 1.0)
         x += movSpeed;
      else
         x = 1.0;
   }else if(fase==2){
      if(x<3.0)
         x += (2.0/3.0)*movSpeed;
      else
         x = 3.0;
   }else if(fase==3){
      if(z<1.5)
         z += movSpeed;
      else
         z = 1.5;
   }
}
void animacionGrua1(float &angle){
   if(angle < 90.0)
      angle += 1.75;
   else
      angle = 90.0;
}
void animacionGrua2(double time, float &angle){
   if(time < 1.25 and time > 0.25){
      if(angle > 0.0)
         angle -= 1.75;
      else
         angle = 0.0;
   }
}
void animacionChasisRot(int &chasisFase, float &chasisRot){
   if(chasisFase==3){
      if(chasisRot <89.9)
         chasisRot += 1.75;
      else
         chasisRot = 89.9;
   }
}
void animacionCoche(bool bajar, double &x, double &y){
   if(bajar){
      y -= 5*movSpeed;
   }else{
      if(x < 7.0)
         x += movSpeed;
      else{
         x = 7.0;
      }
   }
}
void animacionTrampillaAbrir(double time, double &x){
   if(time > 0.8){
      if(x > -0.9)
         x -= 5*movSpeed;
      else
         x = -0.9;
   }
}
void animacionTrampillaCerrar(double &x){
   if(x<0.0)
      x += 0.5*movSpeed;
   else
      x = 0.0;
}
void funLucesTimer(double seconds, double &frames,double &timer0_5s,bool &on){
   if(glfwGetTime()-frames>seconds){
      if(on){
         if(glfwGetTime()-timer0_5s>0.5){
            posIntensity = 0.0f;
            setPosLights();
            mluz.emissive = glm::vec4(0.3,0.0,0.0,1.0);

            timer0_5s = glfwGetTime();
            on = false;
         }
      }else{
         if(glfwGetTime()-timer0_5s>0.5){
            posIntensity = 1.0f;
            setPosLights();
            if(emergencyMode)
               mluz.emissive = glm::vec4(0.8,0.0,0.0,1.0);

            timer0_5s = glfwGetTime();
            on = true;
         }
      }
      frames = glfwGetTime();
   }
}

void configScene() {

   // Test de profundidad
   glEnable(GL_DEPTH_TEST);

   // Transparencias
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Shaders  
   shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

   // Modelos
   sphere.initModel("resources/models/sphere.obj");
   plane.initModel("resources/models/plane.obj");
   cube.initModel("resources/models/cube.obj");
   cylinder.initModel("resources/models/cylinder.obj");
   carChassis.initModel("resources/models/carChassis.obj");
   wheel.initModel("resources/models/wheel.obj");

   // Imagenes (texturas)
   imgNoEmissive.initTexture("resources/textures/imgNoEmissive.png");
   imgGold.initTexture("resources/textures/imgGold.png");
   imgMetal.initTexture("resources/textures/imgMetal.jpg");
   imgMetal2.initTexture("resources/textures/imgMetal2.jpg");
   imgCinta.initTexture("resources/textures/imgCinta.jpg");
   imgDoor.initTexture("resources/textures/imgDoor.jpg");
   imgGoma.initTexture("resources/textures/imgGoma.jpg");
   imgCaution.initTexture("resources/textures/imgCaution.jpg");
   imgConcrete.initTexture("resources/textures/imgConcrete.jpg");

   // Luz ambiental global
   lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

   // Luces direccionales
   setDirLights();

   // Luces posicionales
   setPosLights();
   
   // Luces focales
   lightF[0].position    = glm::vec3(0.0,5.5,0.0);
   lightF[0].direction   = glm::vec3(0.0, -1.0, 0.0);
   lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
   lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
   lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
   lightF[0].innerCutOff = 10.0;
   lightF[0].outerCutOff = lightF[0].innerCutOff + 1.0;
   lightF[0].c0          = 1.000;
   lightF[0].c1          = 0.090;
   lightF[0].c2          = 0.032;
   

   // Materiales
   mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
   mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
   mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
   mluz.emissive  = glm::vec4(0.3, 0.0, 0.0, 1.0);
   mluz.shininess = 1.0;

   ruby.ambient   = glm::vec4(0.174500, 0.011750, 0.011750, 0.55);
   ruby.diffuse   = glm::vec4(0.614240, 0.041360, 0.041360, 0.55);
   ruby.specular  = glm::vec4(0.727811, 0.626959, 0.626959, 0.55);
   ruby.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
   ruby.shininess = 76.8;

   glass.ambient   = glm::vec4(0.011750, 0.174500, 0.011750, 0.2);
   glass.diffuse   = glm::vec4(0.041360, 0.614240, 0.041360, 0.2);
   glass.specular  = glm::vec4(0.626959, 0.727811, 0.626959, 0.2);
   glass.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
   glass.shininess = 76.8;

   cooper.ambient   = glm::vec4( 0.19125, 0.0735, 0.0225, 1.0 );
   cooper.diffuse   = glm::vec4(0.7038, 0.27048, 0.0828, 1.0 );
   cooper.specular  = glm::vec4(0.256777, 0.137622, 0.086014, 1.0);
   cooper.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
   cooper.shininess = 12.8;

   redPlastic.ambient   = glm::vec4(0.0,0.0,0.0,1.0);
   redPlastic.diffuse   = glm::vec4(0.5,0.0,0.0,1.0);
   redPlastic.specular  = glm::vec4(0.7,0.6,0.6,1.0);
   redPlastic.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
   redPlastic.shininess = 32.0;

   whitePlastic.ambient   = glm::vec4(0.0,0.0,0.0,1.0);
   whitePlastic.diffuse   = glm::vec4( 0.55,0.55,0.55,1.0);
   whitePlastic.specular  = glm::vec4( 0.55,0.55,0.55,1.0);
   whitePlastic.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
   whitePlastic.shininess = 32.0;

   blackPlastic.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
   blackPlastic.diffuse   = glm::vec4(0.01, 0.01, 0.01, 1.0);
   blackPlastic.specular  = glm::vec4(0.50, 0.50, 0.50, 1.0);
   blackPlastic.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
   blackPlastic.shininess = 32.0;

   texGold.diffuse   = imgGold.getTexture();
   texGold.specular  = imgGold.getTexture();
   texGold.emissive  = imgNoEmissive.getTexture();
   texGold.normal    = 0;
   texGold.shininess = 51.2;

   texMetal.diffuse   = imgMetal.getTexture();
   texMetal.specular  = imgMetal.getTexture();
   texMetal.emissive  = imgNoEmissive.getTexture();
   texMetal.normal    = 0;
   texMetal.shininess = 50.0;

   texMetal2.diffuse   = imgMetal2.getTexture();
   texMetal2.specular  = imgMetal2.getTexture();
   texMetal2.emissive  = imgNoEmissive.getTexture();
   texMetal2.normal    = 0;
   texMetal2.shininess = 50.0;

   texCinta.diffuse   = imgCinta.getTexture();
   texCinta.specular  = imgCinta.getTexture();
   texCinta.emissive  = imgNoEmissive.getTexture();
   texCinta.normal    = 0;
   texCinta.shininess = 1.0;

   texDoor.diffuse   = imgDoor.getTexture();
   texDoor.specular  = imgDoor.getTexture();
   texDoor.emissive  = imgNoEmissive.getTexture();
   texDoor.normal    = 0;
   texDoor.shininess = 10.0;

   texGoma.diffuse   = imgGoma.getTexture();
   texGoma.specular  = imgGoma.getTexture();
   texGoma.emissive  = imgNoEmissive.getTexture();
   texGoma.normal    = 0;
   texGoma.shininess = 1.0;

   texCaution.diffuse   = imgCaution.getTexture();
   texCaution.specular  = imgCaution.getTexture();
   texCaution.emissive  = imgNoEmissive.getTexture();
   texCaution.normal    = 0;
   texCaution.shininess = 10.0;

   texConcrete.diffuse   = imgConcrete.getTexture();
   texConcrete.specular  = imgConcrete.getTexture();
   texConcrete.emissive  = imgNoEmissive.getTexture();
   texConcrete.normal    = 0;
   texConcrete.shininess = 10.0;
}
void setDirLights(){
   float dirAuxIntensity1 = 0.5f;
   float dirAuxIntensity2 = 0.1f;

   lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);
   lightD[0].ambient   = glm::vec3( 0.1, 0.1, 0.1)*dirIntensity;
   lightD[0].diffuse   = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity;
   lightD[0].specular  = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity;
   lightD[1].direction = glm::vec3(1.0, 0.0, 0.0);
   lightD[1].ambient   = glm::vec3( 0.1, 0.1, 0.1)*dirIntensity*dirAuxIntensity1;
   lightD[1].diffuse   = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity*dirAuxIntensity1;
   lightD[1].specular  = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity*dirAuxIntensity1;
   lightD[2].direction = glm::vec3(-1.0, 0.0, 0.0);
   lightD[2].ambient   = glm::vec3( 0.1, 0.1, 0.1)*dirIntensity*dirAuxIntensity2;
   lightD[2].diffuse   = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity*dirAuxIntensity2;
   lightD[2].specular  = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity*dirAuxIntensity2;
   lightD[3].direction = glm::vec3(0.0, 0.0, 1.0);
   lightD[3].ambient   = glm::vec3( 0.1, 0.1, 0.1)*dirIntensity*dirAuxIntensity2;
   lightD[3].diffuse   = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity*dirAuxIntensity2;
   lightD[3].specular  = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity*dirAuxIntensity2;
   lightD[4].direction = glm::vec3(0.0, 0.0, -1.0);
   lightD[4].ambient   = glm::vec3( 0.1, 0.1, 0.1)*dirIntensity*dirAuxIntensity1;
   lightD[4].diffuse   = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity*dirAuxIntensity1;
   lightD[4].specular  = glm::vec3( 0.7, 0.7, 0.7)*dirIntensity*dirAuxIntensity1;
}
void setPosLights(){
   glm::vec3 pos[]={
      glm::vec3(0.0,6.0,-3.0),
      glm::vec3(0.0,6.0,3.0),
      glm::vec3(-3.5,6.0,-3.0),
      glm::vec3(-3.5,6.0,3.0),
      glm::vec3(3.5,6.0,-3.0),
      glm::vec3(3.5,6.0,3.0),
      glm::vec3(-6.0,6.0,1.5),
      glm::vec3(6.0,6.0,1.5),
      glm::vec3(-6.0,6.0,-1.5),
      glm::vec3(6.0,6.0,-1.5)
   };

   for(int i = 0;i<NLP;i++){
      lightP[i].position    = pos[i];
      lightP[i].ambient     = glm::vec3(0.2, 0.0, 0.0)*posIntensity;
      lightP[i].diffuse     = glm::vec3(1.0, 0.0, 0.0)*posIntensity;
      lightP[i].specular    = glm::vec3(1.0, 0.0, 0.0)*posIntensity;
      lightP[i].c0          = 1.00;
      lightP[i].c1          = 0.22;
      lightP[i].c2          = 0.20;
   }
}
void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 10.0f*glm::sin(glm::radians(alphaY));
    float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 eye   (  x,   y,   z);
    glm::vec3 center(0.0, 0.0,  0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

 // Fijamos las luces
    setLights(P,V);

 // Dibujamos la escena
   glm::mat4 S = I;
   glm::mat4 R = I;
   glm::mat4 T = I;

   // Suelo
   S = glm::scale(I, glm::vec3(6.25, 1.0, 3.0));
   T = glm::translate(I,glm::vec3(-0.2,-0.0,0.0));
   drawObjectTex(plane, texConcrete, P, V, T*S);
   S = glm::scale(I, glm::vec3(6.25, -1.0, 3.0));
   T = glm::translate(I,glm::vec3(-0.2,-0.01,0.0));
   drawObjectTex(plane, texConcrete, P, V, T*S);
   // Cadena de Montaje
   T = glm::translate(I,glm::vec3(-1.0,0.0,-1.5));
   drawCinta(P,V,T);
   T = glm::translate(I,glm::vec3(2.0,0.0,-1.5));
   drawCinta(P,V,T);
   T = glm::translate(I,glm::vec3(-1.0,0.0,1.5));
   drawCinta(P,V,T);
   T = glm::translate(I,glm::vec3(2.0,0.0,1.5));
   drawCinta(P,V,T);
   T = glm::translate(I,glm::vec3(-4.0,0.0,1.5));
   drawCinta(P,V,T);
   T = glm::translate(I,glm::vec3(5.0,0.0,1.5));
   drawCinta(P,V,T);
   R = glm::rotate(I,glm::radians(90.0f),glm::vec3(0,1,0));
   T = glm::translate(I,glm::vec3(3.5,0.0,0.0));
   drawCinta(P,V,T*R);
   T = glm::translate(I,glm::vec3(-5.5,0.0,1.5));
   drawEntrada(P,V,T);
   T = glm::translate(I,glm::vec3(-2.5,0.0,-1.5));
   drawEntrada(P,V,T);
   T = glm::translate(I,glm::vec3(-2.5,0.0,1.5));
   drawPrensa(P,V,T);
   T = glm::translate(I,glm::vec3(0.5,0.0,-1.5));
   drawCabina(P,V,T);
   T = glm::translate(I,glm::vec3(0.5,0.0,1.5));
   drawCabina(P,V,T);
   T = glm::translate(I,glm::vec3(3.5,0.0,-1.5));
   drawGrua(P,V,T);
   T = glm::translate(I,glm::vec3(3.5,0.0,1.5));
   drawCabina3(P,V,T);
   T = glm::translate(I,glm::vec3(6.5,0.0,1.5));
   drawSalida(P,V,T);
   // Objetos en produccion
   drawEsferas(P,V,I,esferasX[0],esferasY[0]);
   drawEsferas(P,V,I,esferasX[1],esferasY[1]);

   drawRuedas(P,V,I,ruedasX[0],ruedasRot[0]);
   drawRuedas(P,V,I,ruedasX[1],ruedasRot[1]);

   drawChasis(P,V,I,chasisX[0],chasisY[0],chasisZ[0],chasisRot[0],chasisPaint[0]);
   drawChasis(P,V,I,chasisX[1],chasisY[1],chasisZ[1],chasisRot[1],chasisPaint[1]);
   drawChasis(P,V,I,chasisX[2],chasisY[2],chasisZ[2],chasisRot[2],chasisPaint[2]);
   drawChasis(P,V,I,chasisX[3],chasisY[3],chasisZ[3],chasisRot[3],chasisPaint[3]);

   drawCoche(P,V,I,cocheX[0],cocheY[0]);
   drawCoche(P,V,I,cocheX[1],cocheY[1]);
   // Foco
   drawFoco(P,V,I,focoX,focoZ);
   // Ventana
   drawVentana(P,V,I);  // Lo ultimo porque tiene transparencias

}

void setLights(glm::mat4 P, glm::mat4 V) {

   shaders.setBool("uFocalOn",focalOn);
   shaders.setBool("uDirOn",dirOn);
   shaders.setBool("uEmergencyMode",emergencyMode);

   shaders.setLight("ulightG",lightG);
   for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
   for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
   for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

   for(int i=0; i<NLP; i++) {
      glm::mat4 M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.1));
      drawObjectMat(sphere, mluz, P, V, M);
   }


}

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

   shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
   shaders.setMat4("uM"  ,M);
   shaders.setMat4("uPVM",P*V*M);
   shaders.setBool("uWithMaterials",true);
   shaders.setMaterial("umaterial",material);
   model.renderModel(GL_FILL);

}
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0) shaders.setBool("uWithNormals",true);
    else                   shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);

}
void drawEsferas(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, double y){
   glm::mat4 S = I;
   glm::mat4 R = I;
   glm::mat4 T = I;

   S = glm::scale(I,glm::vec3(0.05,0.05,0.05));
   T = glm::translate(I,glm::vec3(-0.25+x,y,1.25));
   drawObjectTex(sphere,texGoma,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.75+x,y,1.25));
   drawObjectTex(sphere,texGoma,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.25+x,y,1.75));
   drawObjectTex(sphere,texGoma,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.75+x,y,1.75));
   drawObjectTex(sphere,texGoma,P,V,M*T*S);
}
void drawRuedas(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, bool rot){
   glm::mat4 S = I;
   glm::mat4 R = I;
   glm::mat4 T = I;
   double y = 1.05;

   if(rot){
      R = glm::rotate(I,glm::radians(90.0f),glm::vec3(1,0,0));
      y = 1.15;
   }
      
   S = glm::scale(I,glm::vec3(0.3,0.3,0.3));
   T = glm::translate(I,glm::vec3(-0.25+x,y,1.25));
   drawObjectTex(wheel,texGoma,P,V,M*T*R*S);
   T = glm::translate(I,glm::vec3(-0.75+x,y,1.25));
   drawObjectTex(wheel,texGoma,P,V,M*T*R*S);
   T = glm::translate(I,glm::vec3(-0.25+x,y,1.75));
   drawObjectTex(wheel,texGoma,P,V,M*T*R*S);
   T = glm::translate(I,glm::vec3(-0.75+x,y,1.75));
   drawObjectTex(wheel,texGoma,P,V,M*T*R*S);
}
void drawChasis(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, double y, double z, float rot, bool painted){
   glm::mat4 S = I;
   glm::mat4 R = I;
   glm::mat4 R90 = glm::rotate(I,glm::radians(90.0f),glm::vec3(0,1,0));
   glm::mat4 R180 = glm::rotate(I,glm::radians(180.0f),glm::vec3(0,1,0));
   glm::mat4 T = I;

   Material mat;
   if(painted)
      mat = redPlastic;
   else
      mat = whitePlastic;

   glm::mat4 Taux = glm::translate(I,glm::vec3(-1.0,0.0,0.0));
   glm::mat4 Tinit = glm::translate(I,glm::vec3(1.0,0.0,0.0));

   S = glm::scale(I,glm::vec3(0.2,0.2,0.2));
   T = glm::translate(I,glm::vec3(x-0.5,y,z));
   R = glm::rotate(I,glm::radians(rot),glm::vec3(0,1,0));
   
   if(rot == 0.0)
      drawObjectMat(carChassis,mat,P,V,M*T*R90*S);
   else if(rot==90.0)
      drawObjectMat(carChassis,mat,P,V,M*T*R180*S);
   else
      drawObjectMat(carChassis,mat,P,V,M*T*Tinit*R*Taux*R90*S);
}
void drawCoche(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, double y){
   glm::mat4 S = I;
   glm::mat4 R = I;
   glm::mat4 T = I;
   glm::mat4 R180 = glm::rotate(I,glm::radians(180.0f),glm::vec3(0,1,0));

   S = glm::scale(I,glm::vec3(0.2,0.2,0.2));
   T = glm::translate(I,glm::vec3(x-0.5,y+0.1,1.5));
   drawObjectMat(carChassis,redPlastic,P,V,M*T*R180*S);

   R = glm::rotate(I,glm::radians(90.0f),glm::vec3(1,0,0));
   S = glm::scale(I,glm::vec3(0.3,0.3,0.3));
   T = glm::translate(I,glm::vec3(-0.25+x,y+0.05,1.3));
   drawObjectTex(wheel,texGoma,P,V,M*T*R*S);
   T = glm::translate(I,glm::vec3(-0.75+x,y+0.05,1.3));
   drawObjectTex(wheel,texGoma,P,V,M*T*R*S);
   T = glm::translate(I,glm::vec3(-0.25+x,y+0.05,1.7));
   drawObjectTex(wheel,texGoma,P,V,M*T*R*S);
   T = glm::translate(I,glm::vec3(-0.75+x,y+0.05,1.7));
   drawObjectTex(wheel,texGoma,P,V,M*T*R*S);
}
void drawCinta(glm::mat4 P, glm::mat4 V, glm::mat4 M){
   glm::mat4 S = I;
   glm::mat4 T = I;
   M = glm::translate(M,glm::vec3(0.0,0.95,0.0));

   // Cinta
   S = glm::scale(I,glm::vec3(0.5,1.0,0.5));
   T = glm::translate(I,glm::vec3(0.5,0.1,0.0));
   drawObjectTex(plane,texCinta,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.5,0.1,0.0));
   drawObjectTex(plane,texCinta,P,V,M*T*S);
   // Soporte Cinta
   S = glm::scale(I,glm::vec3(1.0,0.05,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.0,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Laterales Cinta
   S = glm::scale(I,glm::vec3(1.0,0.05,0.05));
   T = glm::translate(I,glm::vec3(0.0,0.1,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,0.1,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Patas Cinta
   S = glm::scale(I,glm::vec3(0.05,0.5,0.05));
   T = glm::translate(I,glm::vec3(0.7,-0.45,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.7,-0.45,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.7,-0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.7,-0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
}
void drawCabina(glm::mat4 P, glm::mat4 V, glm::mat4 M){
   glm::mat4 S = I;
   glm::mat4 T = I;
   M = glm::translate(M,glm::vec3(0.0,0.95,0.0));

   // Cinta
   S = glm::scale(I,glm::vec3(0.5,1.0,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.1,0.0));
   drawObjectTex(plane,texCinta,P,V,M*T*S);
   // Soporte Cinta
   S = glm::scale(I,glm::vec3(0.5,0.05,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.0,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Laterales Cabina
   S = glm::scale(I,glm::vec3(0.5,0.4,0.05));
   T = glm::translate(I,glm::vec3(0.0,0.45,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Techo Cabina
   S = glm::scale(I,glm::vec3(0.4,0.05,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.8,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Patas Cabina
   S = glm::scale(I,glm::vec3(0.05,0.5,0.05));
   T = glm::translate(I,glm::vec3(0.0,-0.45,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,-0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Puertas Cabina  (Cerradas)
   S = glm::scale(I,glm::vec3(0.05,0.4,0.5));
   T = glm::translate(I,glm::vec3(0.45,puertasY,0.0));
   drawObjectTex(cube,texDoor,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.45,puertasY,0.0));
   drawObjectTex(cube,texDoor,P,V,M*T*S);
}
void drawCabina3(glm::mat4 P, glm::mat4 V, glm::mat4 M){
   glm::mat4 S = I;
   glm::mat4 T = I;
   glm::mat4 R = I;
   M = glm::translate(M,glm::vec3(0.0,0.95,0.0));

   // Cinta
   S = glm::scale(I,glm::vec3(0.5,1.0,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.1,0.0));
   drawObjectTex(plane,texCinta,P,V,M*T*S);
   // Soporte Cinta
   S = glm::scale(I,glm::vec3(0.5,0.05,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.0,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Laterales Cabina
   S = glm::scale(I,glm::vec3(0.5,0.4,0.05));
   T = glm::translate(I,glm::vec3(0.0,0.45,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   S = glm::scale(I,glm::vec3(0.05,0.4,0.05));
   T = glm::translate(I,glm::vec3(0.45,0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.45,0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Techo Cabina
   S = glm::scale(I,glm::vec3(0.4,0.05,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.8,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Patas Cabina
   S = glm::scale(I,glm::vec3(0.05,0.5,0.05));
   T = glm::translate(I,glm::vec3(0.0,-0.45,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,-0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Puertas Cabina  (Cerradas)
   S = glm::scale(I,glm::vec3(0.05,0.4,0.5));
   T = glm::translate(I,glm::vec3(0.45,puertasY,0.0));
   drawObjectTex(cube,texDoor,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.45,puertasY,0.0));
   drawObjectTex(cube,texDoor,P,V,M*T*S);
   S = glm::scale(I,glm::vec3(0.4,0.4,0.05));
   T = glm::translate(I,glm::vec3(0.0,puertasY,-0.55));
   drawObjectTex(cube,texDoor,P,V,M*T*S);
}
void drawSalida(glm::mat4 P, glm::mat4 V, glm::mat4 M){
   glm::mat4 S = I;
   glm::mat4 T = I;
   M = glm::translate(M,glm::vec3(0.0,1.0,0.0));

   // Marco Puerta
   S = glm::scale(I,glm::vec3(0.6,0.05,0.05));
   T = glm::translate(I,glm::vec3(0.0,0.0,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,0.0,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   S = glm::scale(I,glm::vec3(0.05,0.05,0.5));
   T = glm::translate(I,glm::vec3(0.55,0.0,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-0.55,0.0,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Puerta
   S = glm::scale(I,glm::vec3(0.5,0.02,0.5));
   T = glm::translate(I,glm::vec3(0.0+trampillaX,0.0,0.0));
   drawObjectTex(cube,texCaution,P,V,M*T*S);
}
void drawEntrada(glm::mat4 P, glm::mat4 V, glm::mat4 M){
   glm::mat4 S = I;
   glm::mat4 T = I;
   M = glm::translate(M,glm::vec3(0.0,0.95,0.0));

   // Cinta
   S = glm::scale(I,glm::vec3(0.5,1.0,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.1,0.0));
   drawObjectTex(plane,texCinta,P,V,M*T*S);
   // Soporte Cinta
   S = glm::scale(I,glm::vec3(0.5,0.05,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.0,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Laterales Entrada
   S = glm::scale(I,glm::vec3(0.5,0.4,0.05));
   T = glm::translate(I,glm::vec3(0.0,0.45,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   S = glm::scale(I,glm::vec3(0.05,0.4,0.5));
   T = glm::translate(I,glm::vec3(-0.45,0.45,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Patas Entrada
   S = glm::scale(I,glm::vec3(0.05,0.5,0.05));
   T = glm::translate(I,glm::vec3(0.0,-0.45,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,-0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Puerta Entrada  (Cerrada)
   S = glm::scale(I,glm::vec3(0.05,0.4,0.5));
   T = glm::translate(I,glm::vec3(0.45,puertasY,0.0));
   drawObjectTex(cube,texDoor,P,V,M*T*S);
}
void drawPrensa(glm::mat4 P, glm::mat4 V, glm::mat4 M){
   glm::mat4 S = I;
   glm::mat4 T = I;
   M = glm::translate(M,glm::vec3(0.0,0.95,0.0));

   // Cinta
   S = glm::scale(I,glm::vec3(0.5,1.0,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.1,0.0));
   drawObjectTex(plane,texCinta,P,V,M*T*S);
   // Soporte Cinta
   S = glm::scale(I,glm::vec3(0.5,0.05,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.0,0.0));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Laterales Cinta
   S = glm::scale(I,glm::vec3(0.5,0.05,0.05));
   T = glm::translate(I,glm::vec3(0.0,0.1,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,0.1,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Patas Prensa
   S = glm::scale(I,glm::vec3(0.05,0.5,0.05));
   T = glm::translate(I,glm::vec3(0.0,-0.45,0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,-0.45,-0.55));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Prensa
   S = glm::scale(I,glm::vec3(0.5,0.15,0.5));
   T = glm::translate(I,glm::vec3(0.0,0.25+prensaY,0.0));
   drawObjectTex(cube,texCaution,P,V,M*T*S);
   S = glm::scale(I,glm::vec3(0.3,0.75,0.3));
   T= glm::translate(I,glm::vec3(0.0,1.15+prensaY,0.0));
   drawObjectTex(cube,texMetal2,P,V,M*T*S);
}
void drawGrua(glm::mat4 P, glm::mat4 V, glm::mat4 M){
   glm::mat4 S = I;
   glm::mat4 T = I;
   glm::mat4 R = glm::rotate(I,glm::radians(gruaRot),glm::vec3(0,1,0));
   M = glm::translate(M,glm::vec3(0.0,0.6,0.0));

   // Soporte Grua
   S = glm::scale(I,glm::vec3(0.5,0.6,0.5));
   drawObjectTex(cube,texMetal,P,V,M*T*S);
   // Base giratoria
   S = glm::scale(I,glm::vec3(0.2,0.05,0.2));
   T = glm::translate(I,glm::vec3(0.0,0.7,0.0));
   drawObjectTex(cylinder,texMetal2,P,V,M*T*R*S);
   // Grua
   S = glm::scale(I,glm::vec3(0.1,0.5,0.1));
   T = glm::translate(I,glm::vec3(0.0,1.3,0.0));
   drawObjectTex(cube,texGold,P,V,M*R*T*S);
   S = glm::scale(I,glm::vec3(0.8,0.1,0.1));
   T = glm::translate(I,glm::vec3(-0.5,1.9,0.0));
   drawObjectTex(cube,texGold,P,V,M*R*T*S);
   S = glm::scale(I,glm::vec3(0.01,0.2,0.01));
   T = glm::translate(I,glm::vec3(-1.0,1.35,0.0));
   drawObjectMat(cylinder,blackPlastic,P,V,M*R*T*S);
   S = glm::scale (I,glm::vec3(0.1,0.02,0.1));
   T = glm::translate(I,glm::vec3(-1.0,0.8,0.0));
   drawObjectTex(cylinder,texMetal2,P,V,M*R*T*S);
}
void drawFoco(glm::mat4 P, glm::mat4 V, glm::mat4 M, double x, double z){
   glm::mat4 S = I;
   glm::mat4 T = I;

   // Malla
   S = glm::scale(I,glm::vec3(6.0,0.1,0.1));
   T = glm::translate(I,glm::vec3(0.0,6.0,-1.5));
   drawObjectMat(cube,cooper,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,6.0,1.5));
   drawObjectMat(cube,cooper,P,V,M*T*S);
   S = glm::scale(I,glm::vec3(0.1,0.1,3.0));
   T = glm::translate(I,glm::vec3(-3.5,6.0,0.0));
   drawObjectMat(cube,cooper,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(3.5,6.0,0.0));
   drawObjectMat(cube,cooper,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(0.0,6.0,0.0));
   drawObjectMat(cube,cooper,P,V,M*T*S);
   //Foco
   S = glm::scale(I,glm::vec3(0.3,0.15,0.3));
   T = glm::translate(I,glm::vec3(0.0+x,5.55,0.0+z));
   drawObjectMat(cylinder,blackPlastic,P,V,M*T*S);
   lightF[0].position.x = focoX;
   lightF[0].position.z = focoZ;

}
void drawVentana(glm::mat4 P, glm::mat4 V, glm::mat4 M){
   glm::mat4 T = I;
   glm::mat4 S = I;

   // Marco
   S = glm::scale(I,glm::vec3(0.05,2.0,0.05));
   T = glm::translate(I,glm::vec3(-6.5,2.0,2.95));
   drawObjectTex(cube,texCaution,P,V,M*T*S);
   T = glm::translate(I,glm::vec3(-6.5,2.0,-2.95));
   drawObjectTex(cube,texCaution,P,V,M*T*S);
   S = glm::scale(I,glm::vec3(0.05,0.05,3.0));
   T = glm::translate(I,glm::vec3(-6.5,4.05,0.0));
   drawObjectTex(cube,texCaution,P,V,M*T*S);
   // Cristales
   
   S = glm::scale(I,glm::vec3(0.05,2.0,2.9));
   T = glm::translate(I,glm::vec3(-6.5,2.0,0.0));
   glDepthMask(GL_FALSE);
   drawObjectMat(cube,glass,P,V,M*T*S);
   glDepthMask(GL_TRUE);
   S = glm::scale(I,glm::vec3(0.05,1.0,1.0));
   T = glm::translate(I,glm::vec3(-6.2,1.0,1.5));
   glDepthMask(GL_FALSE);
   drawObjectMat(cube,ruby,P,V,M*T*S);
   glDepthMask(GL_TRUE);
}
void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

   bool focoMovZ = (focoX >= -3.65 and focoX <= -3.35) or
                   (focoX >= -0.15 and focoX <= 0.15) or
                   (focoX >= 3.35 and focoX <= 3.65);
   bool focoMovX = (focoZ > -1.65 and focoZ <= -1.35) or
                   (focoZ >= 1.35 and focoZ <= 1.65);          

   switch(key) {
      case GLFW_KEY_W:
         if(focoMovZ and focoZ>-3.0)
            focoZ -= 0.075;
         break;
      case GLFW_KEY_A:
         if(focoMovX and focoX>-6.0)
            focoX -= 0.075;
         break;
      case GLFW_KEY_S:
         if(focoMovZ and focoZ<3.0)
            focoZ += 0.075;
         break;
      case GLFW_KEY_D:
         if(focoMovX and focoX<6.0)
            focoX += 0.075;
         break;
      case GLFW_KEY_F: 
         if(action==GLFW_PRESS)
            focalOn = not(focalOn); 
         break;
      case GLFW_KEY_R: 
         if(action==GLFW_PRESS)
            dirOn = not(dirOn); 
         break;
      case GLFW_KEY_Q:
         if(dirIntensity>0.0){
            dirIntensity -= 0.05;
            setDirLights();
         }
         break;
      case GLFW_KEY_E:
         if(dirIntensity<1.0){
            dirIntensity += 0.05;
            setDirLights();
         }
         break;
      case GLFW_KEY_1: 
         if(action==GLFW_PRESS)
            emergencyMode = false; 
         break;
      case GLFW_KEY_2: 
         if(action==GLFW_PRESS)
            emergencyMode = true; 
         break;
      default:
         break;
   }
    

}

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {

    if(yoffset>0) fovy -= fovy>10.0f ? 5.0f : 0.0f;
    if(yoffset<0) fovy += fovy<90.0f ? 5.0f : 0.0f;

}

void funCursorPos(GLFWwindow* window, double xpos, double ypos) {

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) return;

    float limY = 89.0;
    alphaX = 90.0*(2.0*xpos/(float)w - 1.0);
    alphaY = 90.0*(1.0 - 2.0*ypos/(float)h);
    if(alphaY<-limY) alphaY = -limY;
    if(alphaY> limY) alphaY =  limY;

}