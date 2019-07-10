/*---------
  Code: Giftia, QQ: 1005056803, WeChat: Giftia, Website: https://giftia.moe
  禁止商用
  ---------*/
#include <Servo.h>
#include <Wire.h>

Servo Neck;
const int SensorLeftD0 = 0;//右侧的距离感应D0接GPIO0，即D3
const int SensorLeftA0 = 2;//右侧的距离感应A0接GPIO2，即D4
const int SensorRightD0 = 14;//左侧的距离感应D0接GPIO14，即D5
const int SensorRightA0 = 12;//左侧的距离感应A0接GPIO12，即D6
const int SensorFace = 13;//脸的光敏感应接GPIO13,即D7
const int Calibration = 0;//矫正参数，如果舵机和头的相对位置斜了可以调整这个参数
const int Stbypos = 90 + Calibration;//起始位置，90度是脸朝舵机线引出来的地方，范围0到180度
const bool Mode = 1;//这里设置对光反应还是对遮挡反应，0对光反应，对环境要求不高，最好是在昏暗环境；1对遮挡反应，对环境要求高，需要在四周全亮的环境
int LFR = 0;
int Flag = 233;

void setup() {
  Serial.begin(9600);
  pinMode(SensorLeftD0, INPUT);
  pinMode(SensorLeftA0, INPUT);
  pinMode(SensorRightD0, INPUT);
  pinMode(SensorLeftA0, INPUT);
  pinMode(SensorFace, INPUT);
  Neck.attach(4);//舵机接GPIO4，即D2
  Neck.write(Stbypos);
};

void loop() {
  if (Mode) {
    LFR = digitalRead(SensorLeftD0) * 100 + !digitalRead(SensorFace) * 10 + digitalRead(SensorRightD0);
  } else {
    LFR = !digitalRead(SensorLeftD0) * 100 + digitalRead(SensorFace) * 10 + !digitalRead(SensorRightD0);
  };
  switch (LFR) {
    case 111:
      if (Flag == 111) {
        Neck.write(90 + Calibration);
        Serial.println("111，环境正常，没有遮挡，保持往前看");
        break;
      };
      Flag = 111;
      break;

    case 0:
      if (Flag == 0) {
        Neck.write(90 + Calibration);
        Serial.println("000，环境太暗，不正常，保持往前看");
        break;
      };
      Flag = 0;
      break;

    case 10:
      if (Flag == 10) {
        Serial.println("010，环境不正常，可能是光源在面前两侧没有光，保持");
        break;
      };
      Flag = 10;
      break;

    case 101:
      if (Flag == 101) {
        Serial.println("101，环境不正常，可能是光源在两侧面前没有光，保持");
        break;
      };
      Flag = 101;
      break;

    case 110:
      if (Flag == 110 && analogRead(SensorRightA0) > 30) {
        Serial.println("110，右侧被手挡，向左看");
        Neck.write(60 + Calibration);
        break;
      } else if (Flag == 110 && analogRead(SensorRightA0) <= 10) {
        Serial.println("110，右侧被光照，保持");
      };
      Flag = 110;
      break;

    case 100:
      if (Flag == 100 && analogRead(SensorRightA0) > 30) {
        Serial.println("100，右侧和前被脸挡，向右前看");
        Neck.write(120 + Calibration);
        break;
      } else if (Flag == 100 && analogRead(SensorRightA0) <= 10) {
        Serial.println("100，右侧被光照，保持");
      };
      Flag = 100;
      break;

    case 11:
      if (Flag == 11 && analogRead(SensorLeftA0) > 30) {
        Serial.println("011，左侧被手挡，向右看");
        Neck.write(120 + Calibration);
        break;
      } else if (Flag == 11 && analogRead(SensorLeftA0) <= 10) {
        Serial.println("011，左侧被光照，保持");
      };
      Flag = 11;
      break;

    case 1:
      if (Flag == 1 && analogRead(SensorLeftA0) > 30) {
        Serial.println("001，左侧和前被脸挡，向左前看");
        Neck.write(60 + Calibration);
        break;
      } else if (Flag == 1 && analogRead(SensorLeftA0) <= 10) {
        Serial.println("001，左侧被光照，保持");
      };
      Flag = 1;
      break;
  };
  delay(200);
};
