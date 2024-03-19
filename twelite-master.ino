#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial MWSerial(13, 12);  // GP12-RX, GP13-TX

#define BLOCK_MAX 10

// データ列を区切り文字で分割してString型の配列に入れる。
int split(char* source, char delimiter, String* result) {
  char* p = source;
  char tmp[81]; //長さが81の配列
  memset(tmp, 0, sizeof(tmp)); //tmpのメモリ領域を0で埋め,配列のサイズを取得している
  int n     = 0;
  int block = 0;

  // ヌル文字が来るか分けたブロック数がBLOCK_MAXになったらやめる。
  while (*p != 0 && block < BLOCK_MAX) {
    // 区切り文字が来たら、String型に変換する。
    if (*p == delimiter) {
      // 分割したデータが0バイトの時は何もしない。
      if (n > 0) {
        // 念のためヌル文字を末尾に代入。
        tmp[n]        = 0;
        result[block] = String(tmp);
        block++;

        n = 0;

        memset(tmp, 0, sizeof(tmp));
      }
    } else {
      tmp[n] = *p;
      n++;
    }
    p++;
  }

  return block;
}

void setup() {
  Serial.begin(38400);
  MWSerial.begin(38400);
}

void loop() {

  //char str1[] = "distance" ;

  while (MWSerial.available()) {
    static char buf[256] = { 0 };
    static char* p = buf;
    char before = *p;
    *p          = MWSerial.read();

    if (before == '\r' && *p == '\n') {
      *p = 0;                            // 末尾をNull文字にする
      String str[BLOCK_MAX];             // 区切り文字で分割した文字列を入れる
      int block = split(buf, ';', str);  // データの分割

      if (block == 3) {
        //Serial.print("Message: ");
        //Serial.println(buf);
        //Serial.print("Serial No.: ");
        //Serial.println(str[0]);
        //Serial.print("LQI: ");
        //Serial.println(str[1]);
        //Serial.print("Data: ");

//masterではここの記述はいらない
//もしくは，sdカードに直接書き込む

        Serial.println(str[2]);
        Serial.println();
      }

      p = buf;
      memset(buf, 0, sizeof(buf));
    } else {
      if (*p >= 0x20) {
        p++;
      }
    }
  }
  //センサーの値を取り出すときはここはいじらなくていい

  while (Serial.available()) { MWSerial.write(Serial.read()); }
  //Serial.read = 読み込めるデータが存在していれば実行する
  // write = Serialにデータを送っている
  //availableで実行可能かを判定している
  //センサーの値を送るにはwhileの中をいじればいい
}