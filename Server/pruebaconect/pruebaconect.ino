//include libraries
#include <ESP32Video.h>
#include <Ressources/Font6x8.h>
//include model
#include "shapes.h"


Mesh<VGA14Bit> model(shapes::vertexCount, shapes::vertices, 0, 0, shapes::triangleCount, shapes::triangles, shapes::triangleNormals);


//pin configuration
const int redPins[] = {2, 4, 12, 13, 14};
const int greenPins[] = {15, 16, 17, 18, 19};
const int bluePins[] = {21, 22, 23, 27};
const int hsyncPin = 32;
const int vsyncPin = 33;

//VGA Device
VGA14Bit videodisplay;
//3D engine
Engine3D<VGA14Bit> engine(1337);

void setup() {
    Serial.begin(9600); // Buffer de recepción de 256 bytes

      //need double buffering
    videodisplay.setFrameBufferCount(2);
    //initializing i2s vga
    videodisplay.init(VGAMode::MODE200x150, redPins, greenPins, bluePins, hsyncPin, vsyncPin);
    //setting the font
    videodisplay.setFont(Font6x8);
}

///a colorful triangle shader actually calculated per triangle
VGA14Bit::Color myTriangleShader(int trinangleNo, short *v0, short *v1, short *v2, const signed char *normal, VGA14Bit::Color color) {
    // Hacer que todos los triángulos sean visibles, ignorando la orientación
    return color;
}



//render 3d model
void drawModel()
{
  //perspective transformation
  static Matrix perspective = Matrix::translation(videodisplay.xres / 2, videodisplay.yres / 2, 0) * Matrix::scaling(100 * videodisplay.pixelAspect(), 100, 100) * Matrix::perspective(90, 1, 10);
  static float u = 0;
  u += 0.02;
  //rotate model
  Matrix rotation = Matrix::rotation(u, 0, 1, 0);
  Matrix m0 = perspective * Matrix::translation(0, 1.7 * 0, 7) * rotation * Matrix::scaling(7);
  //transform the vertices and normals
  model.transform(m0, rotation);
  //begin adding triangles to render pipeline
  engine.begin();
  //add this model to the render pipeline. it will sort the triangles from back to front and remove backfaced. The tiangle shader will determine the color of the tirangle.
  //the RGB color gien in the second parameter is not used in this case but could be used for calculations in the triangle shader
  model.drawTriangles(engine, videodisplay.RGB(128, 70, 20), myTriangleShader);
  //render all triangles in the pipeline. if you render multiple models you want to do this once at the end
  engine.end(videodisplay);
}


void loop() {
  //if (Serial.available() > 0) {
    //char word = Serial.read(); 
    //Serial.println(word);
    // Lee la palabra enviada por el puerto serie hasta un salto de línea

      //while (word == 'k') {
          //clear the back buffer
          videodisplay.clear(0);
          //draw the figure
  
          drawModel();
  
          videodisplay.show();
             
      //}
      
    //}
    
    //Serial.println("Esperando respuesta");
    //delay(10000);

}
