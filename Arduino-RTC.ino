#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//0x3F or 0x27
LiquidCrystal_I2C lcd(0x3F, 16, 2);   //LCD Object

ThreeWire myWire(11, 10, 12);        // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);    // RTC Object

void setup ()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();

  Rtc.Begin();

  //RtcDateTime currentTime = RtcDateTime(__DATE__ , __TIME__);
  //Rtc.SetDateTime(currentTime);
}

void loop ()
{
  RtcDateTime now = Rtc.GetDateTime();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.print(now.Day());
  lcd.print("/");
  lcd.print(now.Month());
  lcd.print("/");
  lcd.print(now.Year());

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(now.Hour());
  lcd.print(":");
  lcd.print(now.Minute());
  lcd.print(":");
  lcd.print(now.Second());

  //Set the Date and Time Manually when '*' key is pressed
  char c = customKeypad.getKey();
  if (c == '*') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Year: ");
    int year = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Month: ");
    int month = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter DayOfWeek: ");
    int day = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Hours: ");
    int hour = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Minutes: ");
    int minute = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Seconds: ");
    int second = getData();

    RtcDateTime newTime = RtcDateTime(year, month, day, hour, minute, second);
    Rtc.SetDateTime(newTime);

  }
  
  delay(500);
}

int getData() {
  String container = "";
  lcd.setCursor(0, 1);
  while (true) {
    char c = customKeypad.getKey();
    if (c == '#') {
      break;
    } else if (isDigit(c)) {
      container += c;
      lcd.print(c);
    } else {
      //Nothing
    }
  }
  return container.toInt();
}
