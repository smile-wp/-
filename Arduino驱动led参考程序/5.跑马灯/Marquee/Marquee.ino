/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Marquee
* @author       Cindy
* @version      V1.0
* @date         2019.07.29
* @brief        
* @details
* @par History  
*
*/
#include<stdlib.h>
#include<time.h>
#include "./Adafruit_NeoPixel.h"     //Library file
#define PIN 6                        // Define the pins of the RGB light
#define MAX_LED 14                   //14 RGB light

Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );

int R =0; 
int G =0;
int B =0;

/**
* Function       setup
* @author        wusicaijuan
* @date          2019.07.29
* @brief         Initial configuration
* @param[in]     void
* @retval        void
* @par History   no
*/
void setup()
{
  // put your setup code here, it will run once:
  strip.begin();           
  strip.show();           
}

/**
* Function       RGB_Light
* @author        Cindy
* @date          2019.07.29
* @brief         点亮RGB灯
* @param[in]     void
* @retval        void
* @par History   no
*/
void RGB_Light(int i, int R, int G, int B)
{
  uint32_t color = strip.Color(G, R, B);
  strip.setPixelColor(i, color);
  strip.show();
}


/**
* Function       RGB_OFF
* @author        Cindy
* @date          2019.07.29
* @brief         关闭所有的RGB灯
* @param[in]     void
* @retval        void
* @par History   no
*/
void RGB_OFF()
{ 
  uint8_t i = 0;
  uint32_t color = strip.Color(0, 0, 0);
  for(i=0;i<MAX_LED;i++)   
  {
    strip.setPixelColor(i, color);  
  }
    strip.show();
}
/**
* Function       loop
* @author        Cindy
* @date          2019.07.29
* @brief         跑马灯
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{
 for(int i = 0 ; i < MAX_LED ;i++ )
 {
    R = rand()%255 + 1;
    G = rand()%255 + 1;
    B = rand()%255 + 1;
    RGB_Light(i, R, G, B );
    delay(100);
    RGB_OFF();
    
  }
  RGB_OFF();
  delay(300);
}

