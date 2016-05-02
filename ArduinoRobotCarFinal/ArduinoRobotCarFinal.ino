//Center Servos
#include <Servo.h>

//sensor pins instellen
#define trigPin A0 
#define echoPin A1 

//motor A == links motor
#define enA 13
#define motorA1 10
#define motorA2 11

//motor B == rechtse motor
#define enB 12
#define motorB1 5
#define motorB2 6

//Infrarood
#define IR1 7
#define IR2 4
#define IR3 3

Servo servo;

int beweging = 0;

//sensor + Richting
int servopos = 90;
int sensorLinks = 0;
int sensorVooruit = 0;
int sensorRechts = 0;
bool lijnvolgen = false;

//bluetooth variabelen
int state = 0;
int flag = 0; 
int vooruit = 48;
int achteruit = 49;
int links = 50;
int rechts = 51;
bool besturing;
int manueel = 52;
int automa = 53;

//---------------------------------------------------------------------------------------------------------------------------------\\

void setup() {
  // put your setup code here, to run once:

  Serial.begin (9600);

  //sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //motor
  pinMode (enA, OUTPUT);
  pinMode (motorA1, OUTPUT);
  pinMode (motorA2, OUTPUT);
  pinMode (enB, OUTPUT);
  pinMode (motorB1, OUTPUT);
  pinMode (motorB2, OUTPUT);

  //servo
  servo.attach(9);

  //InfraroodSensor
  pinMode (IR1, INPUT);
  pinMode (IR2, INPUT);
  pinMode (IR3, INPUT);


  servo.write(0);
  servo.write(90);
  servo.write(180);
  servo.write(90);
  StartRijden();
  Stilstaan();

}

void loop() {

  if(Serial.available() > 0){
    state = Serial.read();
    flag=0;
    if(state==automa){
      besturing=true;
    }
    else if(state==manueel){
      Stilstaan();
      besturing =false;
    }
    
    Serial.println(besturing);
    if(besturing==false){
  
      if (state == vooruit) {
        Serial.println("vooruit");
        StartRijden();
        delay(100);
    
        if(flag == 0){
        flag = 1;
   
        }
      }
      else if (state == links) {
        Serial.println("links");
        NaarLinks();
        delay(100);
        
        if(flag == 0){
          flag = 1;
        }
      }
      else if (state == achteruit) {
        Serial.println("achter");
        Achteruit();
        delay(100);
        
        if(flag == 0){
          flag = 1;
        }
      }
      else if (state == rechts) {
        Serial.println("rechts");
        NaarRechts();
        delay(100);
        
        if(flag == 0){
          flag = 1;
        }
      }
  
      if(flag==1)
      {
    
        Serial.println("stop");
        Stilstaan();
      }
  
    }
    else{
      Serial.println("auto");
      LijnDetectie();
    }

  }

}


void LijnDetectie(){
  
    int sensor1 = digitalRead(IR1);
    int sensor2 = digitalRead(IR2);
    int sensor3 = digitalRead(IR3);
        
    //Alles Is Zwart (of op lijn, of op grond waarop alles zwart is)
    if (sensor1 == 1 && sensor2 == 1 && sensor3 == 1){
      
      lijnvolgen = true;
      if (beweging <= 2){
        StartRijden();
        beweging++;
      } 
      else {
        Stilstaan();
        beweging = 0;
      }
    }
    //linkse sensor zwart
    else if ((sensor1 == 0 && sensor2 == 1 && sensor3 == 1)) {
      servo.write(90);
      lijnvolgen = true;
      if (beweging <= 2){
        KortRechts();
        beweging++;
      } 
      else {

        Stilstaan();
        beweging = 0;
      }
    }
    else if(lijnvolgen && sensor1 == 0 && sensor2 == 0 && sensor3 == 1){
      servo.write(90);
      if (beweging <= 2){
        KortRechts();
        beweging++;
      } 
      else {

        Stilstaan();
        beweging = 0;
      }
    }
    //Rechtse sensor is zwart of middelste en rechtse zijn zwart
    else if (sensor1 == 1 && sensor2 == 1 && sensor3 == 0){
      servo.write(90);
      lijnvolgen = true;
      if (beweging <= 2){
        KortLinks();
        beweging++;
      } 
      else {
        Stilstaan();
        beweging = 0;
      }
    }

    else if(lijnvolgen && sensor1 == 1 && sensor2 == 0 && sensor3 == 0){
      servo.write(90);
      if (beweging <= 2){
        KortLinks();
        beweging++;
      } 
      else {
        Stilstaan();
        beweging = 0;
      }
    }
    
    else {
      lijnvolgen = false;
      beweging = 0;
      ObstacleRace();
    }
}


void ObstacleRace(){


  //alle3 de richtingen controleren op obstakels
  if(servopos == 90){
    sensorVooruit = BerekenAfstand();
    servo.write(60);
    servopos = 60;
    delay(200);
  }
  else if(servopos == 60){
    sensorLinks = BerekenAfstand();
    servo.write(120);
    servopos = 120;
    delay(200);
  }  
  else if(servopos == 120){
    sensorRechts = BerekenAfstand();
    servo.write(90);
    servopos = 90;
    delay(200);
  } 

  if(sensorVooruit < sensorRechts && sensorVooruit < sensorLinks && sensorVooruit < 20 && sensorVooruit > 0){
    if(sensorRechts > sensorLinks){
      NaarRechts();
      delay(10);
    }
    else{
      NaarLinks();
      delay(10);
    }
  }

  else if(sensorRechts < sensorVooruit && sensorRechts < sensorLinks && sensorRechts < 20 && sensorRechts > 0){
    if(sensorVooruit < 20 && sensorVooruit > 0){
      NaarLinks();
      delay(10);
    }
    else{
      KortLinks();
      delay(10);
    }
  }

  else if(sensorLinks < sensorVooruit && sensorLinks < sensorRechts && sensorLinks < 20 && sensorLinks > 0){
    if(sensorVooruit < 20 && sensorVooruit > 0){
      NaarRechts();
      delay(10);
    }
    else{
      KortRechts();
      delay(10);
    }
  }
  else{
    StartRijden();
  }
  
}


long BerekenAfstand(){
    long duration, afstand;
    
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    //  delayMicroseconds(1000); - Removed this line
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    afstand = (duration/2) / 29.1;

    return afstand;
}


void StartRijden(){
  digitalWrite(motorA1, HIGH); //A vooruit
  digitalWrite(motorA2, LOW);
  analogWrite(enA, 255);

  digitalWrite(motorB1, HIGH); //B vooruit
  digitalWrite(motorB2, LOW);
  analogWrite(enB, 255);
}

void Stilstaan(){
  digitalWrite(motorA1, HIGH); //A Stil
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH); //B stil
  digitalWrite(motorB2, HIGH);
}

void NaarLinks(){
  digitalWrite(motorA1, HIGH); //A Vooruit
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH); //B stil
  digitalWrite(motorB2, LOW);
}

void NaarRechts(){
  digitalWrite(motorA1, HIGH); //A stil
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH); //B Vooruit
  digitalWrite(motorB2, HIGH);
}

void KortLinks(){
  digitalWrite(motorA1, HIGH);  //A Achteruit
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW); //B Stil
  digitalWrite(motorB2, HIGH);
}

void KortRechts(){
  digitalWrite(motorA1, LOW); //A Stil
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH);  //B Achteruit
  digitalWrite(motorB2, LOW);
}

void Achteruit(){
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}










