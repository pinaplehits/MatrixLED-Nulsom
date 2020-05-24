#include <Bounce2.h>
#include <NS_Rainbow.h>

#define PIN 9
#define N_CELL 64

const int RGBColorLeds[][3] = {
  {0, 0, 0}, //turn cells off
  {255, 0, 0}, //red
  {0, 255, 0}, //green
  {0, 0, 255}, //blue
  {255, 0, 255}, //purple
  {0, 255, 255}, //green_blue
  {255, 255, 0}, //yellow
  {255, 255, 255}, //white
  {255, 100, 100}, //pink
  {255, 85, 0} //orange
};
const int colorPin[] = {5, 6, 7, 8};
const int brightnessPin[] = {10, 11};
const int brightnessIntensity[] = {10, 25, 45, 60, 75, 80, 110, 150, 200, 255};
const int sizeColorPin = sizeof(colorPin) / sizeof(colorPin[0]);
const int sizeBrightnessIntensity = sizeof(brightnessIntensity) / sizeof(brightnessIntensity[0]);
const int rowsRGBColorLeds = sizeof(RGBColorLeds) / sizeof(RGBColorLeds[0]);
const int delayBounce = 5;
const int delayBrightness = 150;


int currentColor;
int contador;

// Parameter 1 = number of cells (max: 512)
// Parameter 2 = Arduino pin number (default pin: 9)
//NS_Rainbow ns_matrix = NS_Rainbow(N_CELL);
NS_Rainbow ns_matrix = NS_Rainbow(N_CELL, PIN);

Bounce debouncerPin10 = Bounce();
Bounce debouncerPin11 = Bounce();

void setup()
{
  ns_matrix.begin();
  ns_matrix.setBrightness(brightnessIntensity[sizeBrightnessIntensity - 1]);  // range: 0 ~ 255

  for (byte i = 0 ; i < ns_matrix.numCells(); i++) {
    ns_matrix.setColor(i, 255, 255, 255);
    ns_matrix.show();
    delay(20);
  }

  ns_matrix.clear();

  // Pin 10 and Pin 11 prevents electrical pulse and multiple inputs
  pinMode(brightnessPin[0], INPUT_PULLUP);
  debouncerPin10.attach(brightnessPin[0]);
  debouncerPin10.interval(delayBounce);

  pinMode(brightnessPin[1], INPUT_PULLUP);
  debouncerPin11.attach(brightnessPin[1]);
  debouncerPin11.interval(delayBounce);

  for (int i = 0; i < sizeColorPin; i++) {
    pinMode(colorPin[i], INPUT);
  }

  ns_matrix.setBrightness(brightnessIntensity[0]);
}

void loop()
{
  debouncerPin10.update();
  debouncerPin11.update();

  int debouncerState10 = debouncerPin10.read();
  int debouncerState11 = debouncerPin11.read();

  //BRIGHTNESS SELECTOR BY PIN
  if (debouncerState10 == LOW) {
    if (contador <= 0) {
      contador = (sizeBrightnessIntensity - 1);
      ns_matrix.setBrightness(brightnessIntensity[contador]);
      delay(delayBrightness);
    }
    else
    {
      contador--;
      ns_matrix.setBrightness(brightnessIntensity[contador]);
      delay(delayBrightness);
    }
  }

  if (debouncerState11 == LOW) {
    if (contador + 1 < sizeBrightnessIntensity) {
      contador++;
      ns_matrix.setBrightness(brightnessIntensity[contador]);
      delay(delayBrightness);
    }
    else
    {
      contador = 0;
      ns_matrix.setBrightness(brightnessIntensity[contador]);
      delay(delayBrightness);
    }
  }

  //COLOR SELECTOR BY PIN
  if (digitalRead(colorPin[0]) == LOW)
  {
    int acumulador = pow(2, 0);
    currentColor += acumulador;
    acumulador = 0;
  }

  if (digitalRead(colorPin[1]) == LOW)
  {
    int acumulador = pow(2, 1);
    currentColor += acumulador;
    acumulador = 0;
  }

  if (digitalRead(colorPin[2]) == LOW)
  {
    int acumulador = pow(2, 2);
    currentColor += acumulador;
    acumulador = 0;
  }

  if (digitalRead(colorPin[3]) == LOW)
  {
    int acumulador = pow(2, 3);
    currentColor += acumulador;
    acumulador = 0;
  }

  if (currentColor <= 9)
  {
    for (int i = 0; i < ns_matrix.numCells(); i++)
    {
      ns_matrix.setColor(i, RGBColorLeds[currentColor][0], RGBColorLeds[currentColor][1], RGBColorLeds[currentColor][2]);
      ns_matrix.show();
    }
  }

  currentColor = 0;
}
