
//https://wokwi.com/projects/353652009872567297
#include <LiquidCrystal.h>


#define POTPIN A0
#define STARTPIN 2
#define STOPPIN 3
#define RELAYPIN 8

unsigned long prevMs = 0UL;
unsigned long interval = 1000UL;

bool run;
bool pause;
int time;

LiquidCrystal lcd (12, 11, 4, 5, 6, 7);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  pinMode(STARTPIN, INPUT);
  pinMode(STOPPIN, INPUT);
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);
  
}

void loop() {
  int startbuttonState = digitalRead(STARTPIN);
  int stopbuttonState = digitalRead(STOPPIN);
  unsigned long currMs = millis();

  if (startbuttonState == HIGH){
    if (run == true){
      if (pause == true){
        pause = false;
      }
      else{
        pause = true;
      }
    }
    else if (run == false){
      run = true;
    }
    delay(100);
  }

  if (stopbuttonState == HIGH){
    digitalWrite(RELAYPIN, LOW);
    run = false;
  }

  

  if (run == false){
    time = readPotentioMeter();
    lcd.setCursor(0, 0);
  }


  if (run == true){
    lcd.setCursor(0, 0);
    if (!pause){
      if (currMs - prevMs > interval){
        time = time - 1;
        prevMs = currMs;
        if (time > 0){
          digitalWrite(RELAYPIN, HIGH);
        }
        else{
          digitalWrite(RELAYPIN, LOW);
          run = false;
          
        }
      }
    }
  }

  displayTime(time);

}

int minutes(int time){
  return time / 60;
}

int seconds(int time){
  return time % 60;
}


 void runtimer(int time){
  if (time > 0){
    displayTime(time);
    time = time - 1;
    Serial.println(time);
    delay(1000);
    lcd.clear();
  }
}

void displayTime(int time){
  int m = minutes(time);
  String mins;
  if (m < 10){
    mins = String(0) + String(m);
  }
  else{
    mins = String(m);
  }
  int s = seconds(time);
  String secs;
  if (s < 10){
    secs = String(0) + String(s);
  }
  else{
    secs = String(s);
  }
  String format = mins + ":" + secs;
  lcd.print(format);
}


int readPotentioMeter(){
  int potData = analogRead(POTPIN);
  int perFiveSecs = map(potData, 0, 1023, 0, 240);
  perFiveSecs = perFiveSecs * 5;
  return perFiveSecs;
}
