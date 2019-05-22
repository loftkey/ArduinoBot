#include<SoftwareSerial.h>
#include <Servo.h>

// Mor bluetooth control:
#define TxD 6
#define RxD 5
#define RLED_PIN 8
#define GLED_PIN 9
#define BLED_PIN 10
#define RIGHT_LED_PIN 7
#define LEFT_LED_PIN 4
#define LASER_PIN A1

// Pin Assignments //
#define DIRA 12 // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 13 // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B
#define buzzerPin 2

Servo myservo;
int temp;
bool newVal;
SoftwareSerial bluetoothSerial(TxD, RxD);
char t;
char told;
int pos = 0;

void setup() {
  setupArdumoto();
  bluetoothSerial.begin(9600);
  Serial.begin(9600);
  pinMode(RLED_PIN, OUTPUT);
  pinMode(GLED_PIN, OUTPUT);
  pinMode(BLED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  pinMode(LEFT_LED_PIN, OUTPUT);

  pinMode(DIRA,OUTPUT);   //left motors forward
  pinMode(PWMA,OUTPUT);   //left motors reverse
  pinMode(DIRB,OUTPUT);   //right motors forward
  pinMode(PWMB,OUTPUT);   //right motors reverse
  pinMode(LASER_PIN, OUTPUT);
  temp = 0;
  newVal = false;
  RGB(139,0,139);
  t = 'Q';
  myservo.attach(A2);
  pinMode (buzzerPin, OUTPUT);
}

void loop() {
    if(bluetoothSerial.available()){
      t = bluetoothSerial.read();
      Serial.println(t);
       newVal = true;
    }
    else
      newVal = false;
    
    if(t == 'F'){            //move forward(all motors rotate in forward direction)
      STOP();
      digitalWrite(DIRA,HIGH);
      digitalWrite(DIRB,HIGH);
      digitalWrite(PWMA,HIGH);
      digitalWrite(PWMB,HIGH);
      RGB(0, 255, 0);
    }
     
    else if(t == 'B'){      //move reverse (all motors rotate in reverse direction)
      STOP();
      digitalWrite(PWMA,HIGH);
      digitalWrite(PWMB,HIGH);
      RGB(0, 0, 255);
    }
     
    else if(t == 'R'){      //turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
     STOP();
      digitalWrite(DIRB,HIGH);
      digitalWrite(PWMB,HIGH);
      digitalWrite(RIGHT_LED_PIN,HIGH);
      RGB(255,255,102);
    }
     
    else if(t == 'L'){      //turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
      STOP();
      digitalWrite(DIRA,HIGH);
      digitalWrite(PWMA,HIGH);
      digitalWrite(LEFT_LED_PIN,HIGH);
      RGB(255,255,102);
    }
    else if(t == 'Z'){
    
        unsigned char i;
        for(i=0;i<80;i++)
        {
          digitalWrite(buzzerPin,HIGH);
          delay(1);//wait for 1ms
          digitalWrite(buzzerPin,LOW);
          delay(1);//wait for 1ms
        }
        //output another frequency
        for(i=0;i<100;i++)
        {
          digitalWrite(buzzerPin,HIGH);
          delay(2);//wait for 2ms
          digitalWrite(buzzerPin,LOW);
          delay(2);//wait for 2ms
        }
      
    }
    else if(t == 'X') //laser
    {
       if(temp == 0 && newVal)
       {
        digitalWrite(LASER_PIN, HIGH); 
        temp = 1;
       }
       else if (newVal)
       {
        digitalWrite(LASER_PIN, LOW); 
        temp = 0;
       }
        
    }
    else if(t == 'I' || t == 'J')
    {
       if(t == 'I' && newVal)
       {
        pos += 5;
        myservo.write(pos);
       }
       else if (t == 'J' && newVal)
       {
        pos -= 5;
        myservo.write(pos);
       }
      
    }
    else if(t == 'S'){      //STOP (all motors stop)
      digitalWrite(DIRA,LOW);
      digitalWrite(PWMA,LOW);
      digitalWrite(DIRB,LOW);
      digitalWrite(PWMB,LOW);
      digitalWrite(RIGHT_LED_PIN,LOW);
      digitalWrite(LEFT_LED_PIN,LOW);
      RGB(255, 0, 0);
    }
    delay(100);
}

void RGB(int R,int G, int B)
{
  analogWrite(RLED_PIN, R);
  analogWrite(GLED_PIN, G);
  analogWrite(BLED_PIN, B);
}


void STOP()
{      //STOP (all motors stop)
      digitalWrite(DIRA,LOW);
      digitalWrite(PWMA,LOW);
      digitalWrite(DIRB,LOW);
      digitalWrite(PWMB,LOW);
      digitalWrite(RIGHT_LED_PIN,LOW);
      digitalWrite(LEFT_LED_PIN,LOW);
}

// setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
