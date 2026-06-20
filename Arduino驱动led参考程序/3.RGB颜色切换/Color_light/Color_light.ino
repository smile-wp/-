/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Color_Light
* @author       Cindy
* @version      V1.0
* @date         2019.07.29
* @brief        切换RGB灯的颜色
* @details
* @par History  
*
*/
#include "./Adafruit_NeoPixel.h"     //Library file
#define PIN 6                        // Define the pins of the RGB light
#define MAX_LED 14                   //14 RGB light

Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );


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
* @brief         main fuction
* @param[in]     void
* @retval        void
* @par History   no
*/
void RGB_Light(int R, int G, int B)
{
  uint8_t i = 0;
  R = map(R, 0, 255, 0, 100);  //这里可以通过映射改变亮度，如果觉得灯太亮了刺眼，就可以通过这里降低亮度
  G = map(G, 0, 255, 0, 100);
  B = map(B, 0, 255, 0, 100);
  uint32_t color = strip.Color(G, R, B);  //设置颜色
  for(i=0;i<MAX_LED;i++)   //设置i的值控制点亮灯的序号
  {
    strip.setPixelColor(i, color);  //点亮第一个RGB灯为红色
  }
  strip.show();
}
/**
* Function       loop
* @author        Cindy
* @date          2019.07.29
* @brief         main fuction
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{ 
   RGB_Light(255,0 ,0);   //红
   delay(1000);
   RGB_Light(0,255 ,0);    //绿
   delay(1000);
   RGB_Light(0,0 ,255);    //蓝
   delay(1000);
   RGB_Light(255,255 ,0);   //黄
   delay(1000);
   RGB_Light(255,0 ,255);   //紫
   delay(1000);
   RGB_Light(0,255,255);   //青
   delay(1000);
   RGB_Light(255,255,255);  //白
   delay(1000);
   RGB_Light(0,0 ,0);     //关闭
   delay(1000);

}

