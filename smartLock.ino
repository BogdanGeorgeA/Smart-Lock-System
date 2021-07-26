#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo sm;
int led_pin = 13;
int buzzer_pin = 9;

struct MusicStruct {
  int A = 550;
  int As = 582;
  int B = 617;
  int C = 654;
  int Cs = 693;
  int D = 734;
  int Ds = 777;
  int E = 824;
  int F = 873;
  int Fs = 925;
  int G = 980;
  int Gs = 1003;
  int A2 = 1100;
  int A2s = 1165;
  int B2 = 1234;
  int C3 = 1308;
  int C3s = 1385;
  int D3 = 1555;
}Music;

struct LengthStruct {
  float half = 0.5;
  float one = 1.0;
  float one_half = 1.5;
  float two = 2.0;
  float two_half = 2.5;
}Length;
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; // row pinouts of the keypad R1 = D8, R2 = D7, R3 = D6, R4 = D5
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


LiquidCrystal lcd(12, 11, 17, 16, 15, 14);

int tempo = 400;
int redLed = 18;
int greenLed = 19;


void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Introdu parola");
  lcd.setCursor(0, 1);
  lcd.print("Si apasa *");
  sm.write(180);
  sm.attach(10);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
}

void setTone(int pin, int note, int duration) {
  tone(pin, note, duration);
  delay(duration);
  noTone(pin);
}

void play_yes()
{
    setTone(buzzer_pin, Music.B, tempo * Length.one);
    setTone(buzzer_pin, Music.E, tempo * Length.one_half);
}

void play_no()
{
  setTone(buzzer_pin, Music.F, tempo * Length.one);
  setTone(buzzer_pin, Music.B, tempo * Length.two_half);
}
String pass = "147258";
String inputPass = "";
bool isOpen = false;
int noWrongPass = 0;

void alarm()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alarma!");
  digitalWrite(redLed, HIGH);
  for(int i=0;i<=5;++i)
  {
    play_no();
  }
  noWrongPass = 0;
  digitalWrite(redLed, LOW);
}

void wrongPass()
{
  digitalWrite(redLed, HIGH);
  noWrongPass++;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parola gresita");
  lcd.setCursor(0, 1);
  lcd.print(String("de " + String(noWrongPass) + " ori!"));
  play_no();
  inputPass = "";
  digitalWrite(redLed, LOW);
  if(noWrongPass > 3)
  {
    alarm();
  }
  delay(10);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Introdu parola");
  lcd.setCursor(0, 1);
  lcd.print("Si apasa *");
}

void correctPass()
{
  digitalWrite(greenLed, HIGH);
  inputPass = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parola corecta!");
  play_yes();
  noWrongPass = 0;
  if(!isOpen)
  {
    lcd.setCursor(0, 1);
    lcd.print("Usa deschisa!");
    sm.write(0);
    isOpen = true;
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("Usa inchisa!");
    sm.write(180);
    isOpen = false;
  }
  digitalWrite(greenLed, LOW);
  delay(10);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Introdu parola");
  lcd.setCursor(0, 1);
  lcd.print("Si apasa *");
}

void loop() {
  char key = keypad.getKey();
  if(key)
  {
    if(key == '*')
    {
      setTone(buzzer_pin, Music.B, tempo * Length.half);
      Serial.println(inputPass);
      if(pass == inputPass)
        correctPass();
      else
        wrongPass();
    }
    else
    {
      setTone(buzzer_pin, Music.E, tempo * Length.half);
      inputPass.concat(key);
    }
  }
}
