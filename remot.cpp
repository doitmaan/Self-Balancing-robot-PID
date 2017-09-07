

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned int Display_Counter, Button_Delay = 0;

struct Axis {
  uint16_t axis_1;
  uint16_t axis_2;
  uint16_t axis_3;
  uint16_t axis_4;
  uint16_t axis_5;
  uint16_t axis_6;
  uint16_t axis_7;
  uint16_t axis_8;
};
Axis axis_x;

struct Gesture {
  float angle;
  float omega;
  int speed;
  int P;
  int I;
  int D;
  uint16_t null_1;
  uint16_t null_2;
};
Gesture data;
  +
void setup() {
  pinMode(2, INPUT); //pin 2 as input 
  lcd.init();  // inittilaizing lcd 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SainSmartProduct");// initilaizing the lcd 
  lcd.setCursor(0, 1);
  lcd.print("UprightRover 3.0");

  Mirf.spi = &MirfHardwareSpi;  // initilaizing the bluetooth connection 
  Mirf.init();
  Mirf.setRADDR((byte *)"clie1"); // Sets the receiving address
  Mirf.payload = 16; // 16 bits comunocation 
  Mirf.config();

  delay(4000);
}

void loop() {
  unsigned long time = millis(); // an instance of the time 
  /**********************************************************************************************************/
  axis_x.axis_1 = analogRead(A0); // the objects from the struct are being read from the analoge pins of both joy stick 1-2 , 3-4
  axis_x.axis_2 = analogRead(A1);
  axis_x.axis_3 = analogRead(A2);
  axis_x.axis_4 = analogRead(A3);

  Mirf.setTADDR((byte *)"serv1"); //Sets the transmitting address
  Mirf.send((/.yu *)&axis_x); // Sends a data package to the default address \. Be sure to send the correct
// amount of bytes as configured 16 as payload on the receiver.
  while (Mirf.isSending()) {
  }
  /**********************************************************************************************************/
 
  while (!Mirf.dataReady()) // Checks if data is available for reading , if not and the time passed ssince the loop is more the 2 print wait 
  {
    if ( ( millis() - time ) > 2000) {
      lcd.setCursor(0,0);
      lcd.print("   Waiting...   ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      return;
      }
    }


    Mirf.getData((byte *) &data); // cast the data adress/ refrence to byte pointer 
    //Reads payload bytes into data array
    /**********************************************************************************************************/
    digitalWrite(2, HIGH); // set pin 2 high which is  master slave of the second joystick 
    while (digitalRead(2) == LOW) {
      Button_Delay++;
      delay(1);
    }

    if (Button_Delay > 10) {
      lcd.clear();
      Display_Counter++;
    }
    Button_Delay = 0;

    while (digitalRead(2) == LOW) {
      delay(1);
    }

    if (Display_Counter & 1) {
      PID_Display();
    }
    else {
      Gesture_Display();
    }
    /**********************************************************************************************************/
    /*Serial.print("Ping:");
    Serial.println((millis() - time));*/
  }
  /**********************************************************************************************************/
  void PID_Display()
  {
    lcd.setCursor(0, 0);
    lcd.print("Parameter:P=");
    if (data.P < 1000) {
      lcd.print('0');
      if (data.P < 100) {
        lcd.print('0');
        if (data.P < 10) {
          lcd.print('0');
        }
      }
    }
    lcd.print(data.P);
    /**********************************************************************************************************/
    lcd.setCursor(0, 1);
    lcd.print(" I=");
    if (data.I < 1000) {
      lcd.print('0');
      if (data.I < 100) {
        lcd.print('0');
        if (data.I < 10) {
          lcd.print('0');
        }
      }
    }
    lcd.print(data.I);
    /**********************************************************************************************************/
    lcd.print("  D=");
    if (data.D < 1000) {
      lcd.print('0');
      if (data.D < 100) {
        lcd.print('0');
        if (data.D < 10) {
          lcd.print('0');
        }
      }
    }
    lcd.print(data.D);
  }
  /**********************************************************************************************************/
  void Gesture_Display()
  {
    lcd.setCursor(0, 0);
    lcd.print("Gesture:A=");
    if (10 <= data.angle) {
      lcd.print("+");
      lcd.print(data.angle);
    }
    if (0 < data.angle && data.angle < 10) {
      lcd.print("+0");
      lcd.print(data.angle);
    }
    if (-10 < data.angle && data.angle < 0) {
      data.angle = -data.angle;
      lcd.print("-0");
      lcd.print(data.angle);
    }
    if (data.angle <= -10) {
      lcd.print(data.angle);
    }
    /**********************************************************************************************************/
    lcd.setCursor(0, 1);
    lcd.print("O=");
    if (100 <= data.omega) {
      lcd.print("+");
      lcd.print(data.omega);
    }
    if (10 <= data.omega && data.omega < 100) {
      lcd.print("+0");
      lcd.print(data.omega);
    }
    if (0 < data.omega && data.omega < 10) {
      lcd.print("+00");
      lcd.print(data.omega);
    }
    if (-10 < data.omega && data.omega <= 0) {
      data.omega = -data.omega;
      lcd.print("-00");
      lcd.print(data.omega);
    }
    if (-100 < data.omega && data.omega <= -10) {
      data.omega = -data.omega;
      lcd.print("-0");
      lcd.print(data.omega);
    }
    if (data.omega <= -100) {
      lcd.print(data.omega);
    }
    /**********************************************************************************************************/
    lcd.setCursor(9, 1);
    lcd.print(" S=");
    if (data.speed <= -100) {
      lcd.print(data.speed);
    }
    if (-100 < data.speed && data.speed <= -10) {
      data.speed = -data.speed;
      lcd.print("-0");
      lcd.print(data.speed);
    }
    if (-10 < data.speed && data.speed <= 0) {
      data.speed = -data.speed;
      lcd.print("-00");
      lcd.print(data.speed);
    }
    if (0 < data.speed && data.speed < 10) {
      lcd.print("+00");
      lcd.print(data.speed);
    }
    if (10 <= data.speed && data.speed < 100) {
      lcd.print("+0");
      lcd.print(data.speed);
    }
    if (100 <= data.speed) {
      lcd.print("+");
      lcd.print(data.speed);
    }
  }

