#include"light.h"
#include"Arduino.h"
//Set
// Output the input value as double
int SetFirstValue::InttoDouble(int value) {
  return (double) value;
}

// Input double value as int value
void SetFirstValue::intArray(int intArray[], double doubleArray[], int N) {
  int i;
  for (i = 0; i < N; i++) {
    intArray[i] = (int)doubleArray[i];
    Serial.println(intArray[i]);
  }
}

// Determin the initial value of 8 LEDs
int SetFirstValue::GetDelta(int Time, int pi, int LED_count) {
  return (2 * pi * Time) / LED_count;
}

//光り方の波形の周期に対して各LEDの初期値を決める
void SetFirstValue::SetValue(int Time, int pi, int LED_count, int LED_array[]) {
int i;
  LED_array[0] = 0;
  delta = GetDelta(Time, pi, LED_count);
  Serial.println("delta");
  Serial.println(delta);

  // Serial.println(LED_array[0]);
  for (i = 1; i < LED_count; i++) {
    LED_array[i] = LED_array[i - 1] + delta;
    Serial.println(LED_array[i]);
  }
}



//shineLED
int LED::InputCurrentVlue(int Array[], int ElementCount, int DarkenValue) {
  int i;
  for (i = 0; i < ElementCount; i++) {
    Array1[i] = Array[i] + DarkenValue;
  }
}

//暗くなる前の波形を記録する
void LED::GetCosx(int InputArray[], int OutputArray[], int ElementCount, int DarkenValue) {
  int i;
  for (i = 0; i < ElementCount; i++) {
    InputArray[i] = OutputArray[i] + DarkenValue;
  }
}

/*暗くなる際にLEDの明るさがThresholdでないとき
 * 明るさがThresholdになるように値を変える
 */
void LED::MakeLEDDark(int Array[], int led[], int ElementCount, int Threshold, int DarkenValue) {
  int i;
  for (i = 0; i < 8; i++) {
    if (Array[led[i]] != Threshold) {
      if (Array[led[i]] > Threshold) {
        Array[led[i]]--;
      }
      else Array[led[i]]++;
    }
  }
}

//作品が暗くなった後色をリセットする
void LED::ResetLEDstate(int Array[], int TotalPin) {
  int i;
  for (i = 0; i < TotalPin; i++) {
    Array[i] = 1;
  }
}

//最初の色の変化
void FirstColorChange::Firstcolor(int colorNumber,  int LED_state[], int led_count) {
  if (led_count >= LED_count1) {
    if (colorNumber == 1) {
      LED_state[0 + (LED_count1 - 1) * 3] = 0;
      LED_state[1 + (LED_count1 - 1) * 3] = 0;
      LED_count1 = LED_counter(LED_count1, cycle);
    }
    //blue
    else if (colorNumber == 2) {
      LED_state[1 + (LED_count1 - 1) * 3] = 0;
      LED_state[2 + (LED_count1 - 1) * 3] = 0;
      LED_count1 = LED_counter(LED_count1, cycle);
    }
    //green
    else if (colorNumber == 3) {
      LED_state[0 + (LED_count1 - 1) * 3] = 0;
      LED_state[2 + (LED_count1 - 1) * 3] = 0;
      LED_count1 = LED_counter(LED_count1, cycle);
    }
    else {
    }
  }
}

//2色目の点灯
void FirstColorChange::Secondcolor(int colorNumber, int LED_state[], int led_count) {
  if (led_count >= LED_count2) {
    if (colorNumber == 1) {
      LED_state[2 + (LED_count2 - 1) * 3] = 1;
      LED_count2 = LED_counter(LED_count2, cycle);
    }
    
    //blue
    else if (colorNumber == 2) {
      LED_state[0 + (LED_count2 - 1) * 3] = 1;
      LED_count2 = LED_counter(LED_count2, cycle);
    }
    
    //green
    else if (colorNumber == 3) {
      LED_state[1 + (LED_count2 - 1) * 3] = 1;
      LED_count2 = LED_counter(LED_count2, cycle);
    }
    else{
    }
  }
}

//
int FirstColorChange::CheakLEDCount(){
  if(LED_count1>=8){
  return 1;
  }else return 0;
}

//
void FirstColorChange::CountColorChange(int ColorNumber, int PreviouColorNumber){
  if(ColorNumber!=PreviouColorNumber){
    LED_count2=0;
  }
}

//LEDの色が変わる周期をpraivateに格納
void FirstColorChange::InputCycle(int LED_cycle) {
  cycle = LED_cycle;
}


int FirstColorChange::LED_counter(int LED_count, int cycle) {
  if (cycle > cyclecount) {
    cyclecount++;
    return LED_count;
  } else {
    cyclecount = 0;
    return LED_count + 1;
  }
}

//作品が暗くなった際に2つの変数リセットする
void FirstColorChange::LEDReset() {
  LED_count1 = 0;
  LED_count2 = 0;
}

//ランダムでLEDの配列の値が1となった時、加算しない
void CountAndReset::CountArray(int Array[], int N) {
  int i;
  for (i = 0; i < N; i++) {
    if (LED[i] == 0) {
      Array[i]++;
    }
  }
}


//色のリセット
void CountAndReset::RsetLEDstate(int Array[], int N) {
  int i, j;
  for (i = 0; i < N; i++) {
    Array[i] = LEDcolor[i];
  }
}

/*ランダムで点灯するLEDを決める 
 * LEDの配列の値が0の時加算する
 * LEDの配列の値が1の時加算しない
 */
void CountAndReset::RandomReset() {
  int total = NonGlowingLEDs();
  int a = EvenOrOdd();
  int i, n;

//光らないLEDを決める
  for (i = 0; i < total; i++) {
    n = NumOfLEDs();
    LED[2 * n + a] = 1;
  }
  for(i=0;i<8;i++){
    
  }
}


void CountAndReset::ResetArray1() {
  int i;
  for (i = 0; i < 8; i++) {
    LED[i] = 0;
  }
}

//点灯するLEDをランダムで変わる周期を入力
void CountAndReset::SetRandomCycle(int Cycle){
  RandomCycle=Cycle;
}

/*ループするごとにiを加算する
 * iが周期を超えた時点灯するLEDをリセットする
 */
void CountAndReset::SetRandom() {
  static int i;
  i++;
  if (i > RandomCycle) {
    ResetArray1();
    RandomReset();
    i = 0;
  }
}

//
void CountAndReset::ResetArray(int Array[], int led_count, int Time, int pi) {
  int i;
  for (int i = 0; i < led_count; i++) {
    if (Array[i] > 2 * Time * pi-2) {
     
      Array[i] = 0;
    }
  }
}

//引数をリセットする
int CountAndReset::ResetValue(int Value) {
  Value = 0;
}



/*最初の色入力されたかを判断する
 * 初めて0以外のcolorNumberを入力した時
 * JugeFirstを1にして最初に入力した色を出力する
 */
int color::InputFirstColor(int colorNumber) {
  
  if (JugeFirst == 0 && colorNumber != 0) {
    FirstColorNum = colorNumber;
    JugeFirst = 1;
    return FirstColorNum;
  } else if (JugeFirst == 1) {
    return FirstColorNum;
  }
  else return 0;
}

/*JugeFirstが0の時まだ最初の色が入力されていないため光らない
 * 最初の色が入力されるとJugeFirstが1になるため1を出力する
 * 1を出力するとき2色目を光らせることができる
 */
int color::Juge() {
  if (JugeFirst == 0) {
    return 0;
  } else return 1;
}

void color::ResetJugeFirst() {
  JugeFirst = 0;
}

//色の入力
int color::choose(int red, int blue, int green,  int threshold, int num) {

  if (red < threshold && green < threshold && blue < threshold) return num;
  else {
    int max_col = max( max(red, blue), green);

    if (max_col == red)return 1;
    if (max_col == blue)return 2;
    if (max_col == green)return 3;
  }
}

//色を入力し続けるとDarkenValueを加算する
int color::DarkenColor(int red, int blue, int green, int threshold, int DarkenValue, int maxDarkenValue) {
  if (maxDarkenValue > DarkenValue) {
    if (red < threshold && green < threshold && blue < threshold) return DarkenValue;
    else return DarkenValue + 5;
  } else return DarkenValue;
}

//距離を測る
int Distance::MeasuretheDistance(int value) {

  float V_out = (float)value * 5 / 1024;
  float distance = -(V_out - 2.3) / 0.036;
  return distance;

}
