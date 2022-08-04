//include files
#include "ShiftRegisterPWM.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "light.h"
#include"Arduino.h"

//Determin how to light
//A*cos(x/T)+B
#define T 3 //period
#define pi 3 //PI
#define A 40 //amplitude
#define B 55 //Intercept
#define maxDarkenValue 150

//Total amount of LED
#define led_count 8

//Total amount of pin
#define total_pin 24

//cycle of color change
#define cycle_of_color_change 8
#define RandomCycle 2*pi*T

//color of threshold
#define threshold 100

#define ThresholodformHands 10
#define ThresholodformSensor 20
#define resetloop 100
int cosx[2 * T * pi];
int led[led_count] = {0};
int count = 1;

//choose a color
ShiftRegisterPWM sr(3, 16);
SetFirstValue Set;
Distance distance;
LED shineLED;
FirstColorChange ColorChange;
CountAndReset CountReset;
color InputColor;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_120MS, TCS34725_GAIN_1X);

void shine(int LED_state[], int Array[], int led[], int DarkenValue) {
  int i;
  for (i = 0; i < 8; i++) {
    sr.set(i * 3, (Array[led[i]] + DarkenValue)*LED_state[i * 3]);
    sr.set(i * 3 + 1, (Array[led[i]] + DarkenValue)*LED_state[i * 3 + 1]);
    sr.set(i * 3 + 2, (Array[led[i]] + DarkenValue)*LED_state[i * 3 + 2]);
Serial.print(Array[led[i]]);
Serial.print(",");
  }
  Serial.println("");
}

void setup()
{
  pinMode(2, OUTPUT); // sr data pin
  pinMode(3, OUTPUT); // sr clock pin
  pinMode(4, OUTPUT); // sr latch pin
  randomSeed(analogRead(0));
  Serial.begin(9600);
  //LEDの点灯の周期を決める ランダムで点灯する周期を変える
  ColorChange.InputCycle(cycle_of_color_change);
  CountReset.SetRandomCycle(RandomCycle);

  //  カラーセンサーが正常に動作するか
 /* if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }*/

  // timer1を周波数を更新するために使用
  sr.interrupt(ShiftRegisterPWM::UpdateFrequency::Medium);

  double x = 0;
  int i;
  int count = 0;
  double double_cos[2 * pi * T];
  //  Convert amplitude, period, intercept to double
  double double_T = (double)T;
  double double_A = (double)A;
  double double_B = (double)B;

  //光り方の波形決める
  for (i = 0; i < 2 * T * pi; i++) {
    double_cos[i] = -double_A * cos(x / double_T) + double_B;
    Serial.print("double_cos[i]");
    Serial.println(double_cos[i]);
    x++;
    count++;
  }
  //int型に変換
  Set.intArray(cosx, double_cos, 2 * pi * T);
  Serial.print(" count ");
  Serial.println(count);
  //各LEDの初期値を決める
  Set.SetValue(T, pi, led_count, led);
}

void loop()
{
  int i;
  int red, green, blue, c;
  tcs.getRawData(&red, &green, &blue, &c);
  //0の時消灯 1の時点灯
  static int LED_state[24] = {1, 1, 1, 1, 1, 1, 1, 1,
                              1, 1, 1, 1, 1, 1, 1, 1,
                              1, 1, 1, 1, 1, 1, 1, 1
                             };

  //  p_ColorNumber is the previous color
  static int ColorNumber = 0;
  static int p_ColorNumber = 0;
  static int times = 0;
  static int times_color = 0;
  static int DarkenValue = 0;
  static int TimesColorChanged = 0;
  static int Recordcosx[2 * pi * T];
  int HandsfromSensor = distance.MeasuretheDistance(analogRead(A0));
  int HandsfromWork = distance.MeasuretheDistance(analogRead(A2));

  //LED点灯の箇所をランダムに変える
  CountReset.SetRandom();

  //カラーセンサーに筆が一定の距離に入ったら色を読み取る
  if (ThresholodformSensor > HandsfromSensor) {
    ColorNumber = InputColor.choose(red, blue, green, threshold, p_ColorNumber);
    DarkenValue = InputColor.DarkenColor(red, blue, green, threshold, DarkenValue, maxDarkenValue);
  }

 

  shine(LED_state, cosx, led, DarkenValue);
  CountReset.CountArray(led, led_count);


  //最初の色を変化させる
  ColorChange.Firstcolor(InputColor.InputFirstColor(ColorNumber), LED_state, led_count);
 //最初の色が入力されたかどうか
  TimesColorChanged = InputColor.Juge();



  //2色目を光らせる
  if (TimesColorChanged == 1) {
    ColorChange.Secondcolor(ColorNumber, LED_state, led_count);
  }


  //カラーセンサーで読みった色を記録する
  TimesColorChanged = ColorChange.CheakLEDCount();
  ColorChange.CountColorChange(ColorNumber, p_ColorNumber);
  p_ColorNumber = ColorNumber;


  // 暗くなる時に現在の明るさから暗くなるように光り方の波形を記録する
  shineLED.GetCosx(Recordcosx, cosx, 2 * pi * T, DarkenValue);


  if (ThresholodformHands > HandsfromWork) {
    for (i = 0; i < resetloop; i++) {
      shineLED.MakeLEDDark(Recordcosx, led, 2 * pi * T, 5, DarkenValue);
      shine(LED_state, Recordcosx, led, DarkenValue);
      HandsfromWork = distance.MeasuretheDistance(analogRead(A2));
      if (ThresholodformHands > HandsfromWork) {
        i = 0;
      }
    }
    if (HandsfromWork > ThresholodformHands) {
      shineLED.ResetLEDstate(LED_state, total_pin);
      InputColor.ResetJugeFirst();
      ColorChange.LEDReset();
      ColorNumber = CountReset.ResetValue(ColorNumber);
      // p_ColorNumber=CountReset.ResetValue(p_ColorNumber);
      p_ColorNumber = CountReset.ResetValue(p_ColorNumber);
      DarkenValue = CountReset.ResetValue(DarkenValue);
    }
  }

  /*作品に手をかざした際に作品が暗くなる
     距離センサで入力される値はノイズがあるため
     再度閾値内に手が入ったらfor文iを0にリセットすることで
     暗い状態をキープする

     作品から手が離れた時にリセットする値
     ・LEDの状態 配列名 LED_state  例)赤色から白色に変化
     ・入力された色 変数名 ColorNumber
     ・記録した色　変数名 p_ColorNumber
     ・色を入力し続けた際に濃くなる値　変数名　DarkenValue
  */

  CountReset.ResetArray(led, led_count, T, pi);
}
