#ifndef light_h
#define light_h

#include"Arduino.h"
class SetFirstValue {
  public:
    void SetValue(int Time, int pi, int LED_count, int LED_array[]);
    int InttoDouble(int value);
    void intArray(int intArray[], double doubleArray[], int N);
  private:
    int GetDelta(int Time, int pi, int LED_count);
    int delta;

};

class LED {
  public:
    int InputCurrentVlue(int Array[], int ElementCount, int DarkenValue);
    void SetElementCount(int ElementCount);
    void MakeLEDDark(int Array[], int led[], int ElementCount, int Threshold, int DarkenValue);
    void GetCosx(int InputArray[], int OutputArray[], int ElementCount, int DarkenValue);
    void ResetLEDstate(int Array[] , int TotalPin);
  public:
    int Array1[24];  
};

class FirstColorChange {
  public:
    void Firstcolor(int colorNumber,  int LED_state[], int led_count);
    void Secondcolor(int colorNumber, int LED_state[], int led_count);
    void InputCycle(int LED_cycle);
    void LEDReset();
    void CountColorChange(int ColorNumber, int PreviouColorNumber);
    int CheakLEDCount();
  private:
    void ShiningCycle(int* LED_count);
    int LED_counter(int LED_count, int cycle);
    int LED_count1 = 1;
    int LED_count2 = 1;
    int colorNumber;
    int FirstColorNum = 0;
    int cyclecount;
    int cycle;

};

class CountAndReset {
  public:
    void CountArray(int Array[], int N);
    void ResetArray(int Array[], int led_count, int Time, int pi);
    int Counter(int LED_count, int maxvalue);
    void DetermineCycle(int Cycle, int LED_state[], int maxvalue);
    void RandomReset();
    void SetRandomCycle(int Cycle);
    void SetRandom();
    void RsetLEDstate(int Array[], int N);
    int ResetValue(int Value);
  private:
    int Reset(int LED_count, int maxvalue);
    void ResetArray1();
    
    //偶数か奇数か
    int EvenOrOdd() {
      return random(0, 2);
    }
    //点灯しないLEDを決める    
    int NumOfLEDs() {
      return random(0, 4);
    }
    //点灯しないLEDの総数
    int NonGlowingLEDs() {
      return random(0, 5);
    }
    
    int LED[8] = {0};
//色リセットする際に使用
    int LEDcolor[24] = {0};
    int ColorPin[24] = {0};
    int RandomCycle=0;
    int ColorOn=0;
};

class color {
  public:
    int choose(int red, int blue, int green, int threshold, int num);
    int InputFirstColor(int colorNumber);
    byte JugeFirst;
    int FirstColorNum;
    int DarkenColor(int red, int blue, int green, int threshold, int DarkenValue, int maxDarkenValue);
    void ResetJugeFirst();
    int Juge();
};

class Distance {
  public:
    int MeasuretheDistance(int value);
};
#endif
