// ================================================================
// Sookmyung Women's University Electronic Engineering 21
// Soomin Kwon 2115185
// rnjss8m@sm.ac.kr
// ----------------------------------------------------------------
// PROJECT NAME  : arduino_biped_robot_v2
// FILE NAME     : Final_biped_v2.ino
// ABOUT         : 
// CREATION DATE : 2022.06.18
// ================================================================


#include<Wire.h>
#include <Servo.h>

const int MPU1 = 0x68, MPU2 = 0x69;
int AcX, AcY, AcZ;
int delayVal = 100; //딜레이 값 지정
int delayVal2 = 500;

bool time = true; //time이라는 변수를 true로 지정함
//Creating Servo Objects
Servo R_Ankle;
Servo L_Ankle;
Servo R_Knee;
Servo L_Knee;
Servo R_Hip;
Servo L_Hip; //서보모터 이름 지정

//Function Prototype//
void stand();
void L_forward();
void R_forward();

int R_AnkPos = 70;
int R_KneePos = 80;
int R_HipPos = 65;
int L_AnkPos = 70;
int L_KneePos = 65;
int L_HipPos = 65;

void setup () {
  //Servo input pins
  R_Ankle.attach (3);
  R_Knee.attach (5);
  R_Hip.attach (6);
  L_Ankle.attach (9);
  L_Knee.attach (10);
  L_Hip.attach (11);
  stand();
  delay (2000);

  Wire .begin ();
  Wire .beginTransmission (MPU1);
  Wire .write (0x6B);
  Wire .write (0); //MPU6050 1을 동작 대기 모드로 변경
  Wire .endTransmission (true);

  Wire .begin ();
  Wire .beginTransmission (MPU2);
  Wire .write (0x6B);
  Wire .write (0); //MPU6050 2를 동작 대기 모드로 변경
  Wire .endTransmission (true);
  Serial .begin (9600);
}

void loop () {
  while (1) {
    //받아온 센서값을 출력
    GetMPUValue(MPU1);
    if (AcX > -10000 && AcZ > 0) { //왼쪽다리 앞으로 걸으면
      Serial .print (" Walk");
      Serial .println ();
      L_forward(); //왼쪽다리부터 보행하는 코드 실행
      delay (800);
      break ;
    }
    Serial .print ("\t ||| \t");

    GetMPUValue(MPU2);
    if (AcX > -10000 && AcZ > 0) { //오른쪽다리 앞으로 걸으면
      Serial .print (" Walk");
      R_forward(); //오른쪽다리부터 보행하는 코드 실행
    }
    Serial .println ();

    delay (800);
  }
}

void GetMPUValue(const int MPU) {

  Wire .beginTransmission (MPU); //MPU6050 호출
  Wire .write (0x3B); //AcX 레지스터 위치 요청
  Wire .endTransmission (false);
  Wire .requestFrom (MPU, 14, true); //14byte의 데이터를 요청

  AcX = Wire .read () << 8 | Wire .read (); //두개의 나뉘어진 바이트를 하나로 이어붙임
  AcY = Wire .read () << 8 | Wire .read ();
  AcZ = Wire .read () << 8 | Wire .read ();

  Serial .print ("X: ");
  Serial .print (AcX);
  Serial .print (" Y: ");
  Serial .print (AcY);
  Serial .print (" Z: ");
  Serial .print (AcZ);
}

//Function Definition//

void stand () { //초기 포지션. 각 모터가 출력할 값 지정
  R_Ankle.write (70);
  R_Knee.write (80);
  R_Hip.write (65);
  L_Ankle.write (70);
  L_Knee.write (65);
  L_Hip.write (65);
}

////////////FORWARD///////////////

///////////Left Leg///////////////

void L_forward() { //left leg first walk

  for (int i = 0; i < 6; i += 1) { //무게중심 옮기기
    R_AnkPos += 3;
    L_AnkPos -= 5;
    R_Ankle.write (R_AnkPos );
    L_Ankle.write (L_AnkPos );
    delay (delayVal + 50);
  }
  delay (delayVal2);

  for (int i = 0; i < 6; i += 1) { //왼다리 내딛기
    L_AnkPos += 5;
    L_KneePos -= 4;
    L_HipPos -= 5;
    L_Ankle.write (L_AnkPos );
    L_Knee.write (L_KneePos);
    L_Hip.write (L_HipPos );

    delay (delayVal);
  }
  delay (delayVal2);

  for (int i = 0; i < 6; i += 1) { // 무게중심 원위치
    R_AnkPos -= 3;
    R_Ankle.write (R_AnkPos );

    delay (delayVal);
  }
  delay (delayVal2);


  for (int i = 0; i < 6; i += 1) { //무게중심 반대로 옮기기
    L_AnkPos -= 4;
    L_Ankle.write (L_AnkPos );

    delay (delayVal);
  }
  delay (delayVal2);

  for (int i = 0; i < 6; i += 1) { // 왼다리 원위치(발목 제외 standpos)
    L_KneePos += 4;
    L_HipPos += 5;
    L_Knee.write (L_KneePos);
    L_Hip.write (L_HipPos );

    delay (delayVal);
  }
  delay (delayVal2);

  for (int i = 0; i < 6; i += 1) { // 무게중심 원위치 (전체 standpos)
    L_AnkPos += 4;
    L_Ankle.write (L_AnkPos );

    delay (delayVal);
  }
  stand();
  delay (delayVal2);

  time = false;
}
///////////Rightt leg///////////////

void R_forward() { //right leg first walk

  for (int i = 0; i < 6; i += 1) { //무게중심 옮기기
    L_AnkPos -= 3;
    R_AnkPos += 5;
    R_Ankle.write (R_AnkPos );
    delay (100);
    L_Ankle.write (L_AnkPos );
    delay (150);
  }
  delay (delayVal2);

  for (int i = 0; i < 6; i += 1) { //오른다리 내딛기
    R_AnkPos -= 5;
    R_KneePos += 5;
    R_HipPos += 5;
    R_Ankle.write (R_AnkPos );
    R_Knee.write (R_KneePos);
    R_Hip.write (R_HipPos );

    delay (delayVal);
  }
  delay (delayVal2);

  for (int i = 0; i < 6; i += 1) { //무게중심 원위치
    L_AnkPos += 3;
    L_Ankle.write (L_AnkPos );

    delay (delayVal);
  }
  delay (delayVal2);


  for (int i = 0; i < 6; i += 1) { //무게중심 반대로 옮기기
    R_AnkPos += 4;
    R_Ankle.write (R_AnkPos );

    delay (delayVal);
  }
  delay (delayVal2);

  for (int i = 0; i < 6; i += 1) { // 오른다리 원위치(발목 제외 standpos)
    R_KneePos -= 5;
    R_HipPos -= 5;
    R_Knee.write (R_KneePos);
    R_Hip.write (R_HipPos );

    delay (delayVal);
  }
  delay (delayVal2);

  for (int i = 0; i < 6; i += 1) { // 무게중심 원위치 (전체 standpos)
    R_AnkPos -= 5;
    R_Ankle.write (R_AnkPos );

    delay (delayVal);
  }
  stand();
  delay (delayVal2);

  time = false;
}

