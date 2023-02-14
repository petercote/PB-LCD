//20x4 LCD
#include <LiquidCrystal_I2C.h>  //SDA = B7[A4], SCL = B6[A5] STM32/[Arduino]
LiquidCrystal_I2C lcd(0x27, 20, 4);

int menuCounter = 0;  //counts the clicks of the rotary encoder between menu items (0-3 in this case)

int speed_MenuValue = 0;  //value within menu 1
int spin_MenuValue = 0;  //value within menu 2
int feed_MenuValue = 0;  //value within menu 3
int osc_MenuEna = 1;  //value within menu 4
int oscRng_Menu = 1;
int oscSpd_Menu = 1;

bool speed_selected = false;  //enable/disable to change the value of menu item
bool spin_selected = false;
bool feed_selected = false;
bool oscEna_selected = false;
bool oscRng_selected = false;
bool oscSpd_selected = false;
//Note: if a menu is selected ">" becomes "X".

//Defining pins
//Arduino interrupt pins: 2, 3.
const int RotaryCLK = 5;    //CLK pin on the rotary encoder
const int RotaryDT = 18;    //DT pin on the rotary encoder
const int PushButton = 19;  //Button to enter/exit menu

//Statuses for the rotary encoder
int CLKNow;
int CLKPrevious;

int DTNow;
int DTPrevious;

bool refreshLCD = true;         //refreshes values
bool refreshSelection = false;  //refreshes selection (> / X)

void setup() {
  setCpuFrequencyMhz(240);
  pinMode(RotaryCLK, INPUT_PULLUP);   //RotaryCLK
  pinMode(RotaryDT, INPUT_PULLUP);    //RotaryDT
  pinMode(PushButton, INPUT_PULLUP);  //Button

  //------------------------------------------------------
  lcd.begin();  // initialize the lcd
  lcd.backlight();
  //------------------------------------------------------
  lcd.setCursor(0, 0);  //Defining positon to write from first row, first column .
  lcd.print("Pickleball Machine");
  lcd.setCursor(0, 1);  //Second row, first column
  lcd.print("Booting...");
  delay(2000);  //wait 2 sec

  lcd.clear();  //clear the whole LCD

  printLCD();  //print the stationary parts on the screen
  //------------------------------------------------------
  //Store states of the rotary encoder
  CLKPrevious = digitalRead(RotaryCLK);
  DTPrevious = digitalRead(RotaryDT);

  attachInterrupt(digitalPinToInterrupt(RotaryCLK), rotate, CHANGE);        //CLK pin is an interrupt pin
  attachInterrupt(digitalPinToInterrupt(PushButton), pushButton, FALLING);  //PushButton pin is an interrupt pin
}

void loop() {
  if (refreshLCD == true)  //If we are allowed to update the LCD ...
  {
    updateLCD();  // ... we update the LCD ...

    //... also, if one of the menus are already selected...
    if (speed_selected == true || spin_selected == true || feed_selected == true || oscEna_selected == true) {
      // do nothing
    } else {
      updateCursorPosition();  //update the position
    }

    refreshLCD = false;  //reset the variable - wait for a new trigger
  }

  if (refreshSelection == true)  //if the selection is changed
  {
    updateSelection();         //update the selection on the LCD
    refreshSelection = false;  // reset the variable - wait for a new trigger
  }
}

void rotate() {
  //-----MENU1--------------------------------------------------------------
  if (speed_selected == true) {
    CLKNow = digitalRead(RotaryCLK);  //Read the state of the CLK pin
    // If last and current state of CLK are different, then a pulse occurred
    if (CLKNow != CLKPrevious && CLKNow == 1) {
      // If the DT state is different than the CLK state then
      // the encoder is rotating in A direction, so we increase
      if (digitalRead(RotaryDT) != CLKNow) {
        if (speed_MenuValue < 75)  //we do not go above 100
        {
          speed_MenuValue++;
        } else {
          speed_MenuValue = 75;
        }
      } else {
        if (speed_MenuValue < 1)  //we do not go below 0
        {
          speed_MenuValue = 1;
        } else {
          // Encoder is rotating B direction so decrease
          speed_MenuValue--;
        }
      }
    }
    CLKPrevious = CLKNow;  // Store last CLK state
  }
  //---MENU2---------------------------------------------------------------
  else if (spin_selected == true) {
    CLKNow = digitalRead(RotaryCLK);  //Read the state of the CLK pin
    // If last and current state of CLK are different, then a pulse occurred
    if (CLKNow != CLKPrevious && CLKNow == 1) {
      // If the DT state is different than the CLK state then
      // the encoder is rotating in A direction, so we increase
      if (digitalRead(RotaryDT) != CLKNow) {
        if (spin_MenuValue < 15)  //we do not go above 100
        {
          spin_MenuValue++;
        } else {
          spin_MenuValue = 15;
        }
      } else {
        if (spin_MenuValue < -15)  //we do not go below 0
        {
          spin_MenuValue = -15;
        } else {
          // Encoder is rotating in B direction, so decrease
          spin_MenuValue--;
        }
      }
    }
    CLKPrevious = CLKNow;  // Store last CLK state
  }
  //---MENU3---------------------------------------------------------------
  else if (feed_selected == true) {
    CLKNow = digitalRead(RotaryCLK);  //Read the state of the CLK pin
    // If last and current state of CLK are different, then a pulse occurred
    if (CLKNow != CLKPrevious && CLKNow == 1) {
      // If the DT state is different than the CLK state then
      // the encoder is rotating in A direction, so we increase
      if (digitalRead(RotaryDT) != CLKNow) {
        if (feed_MenuValue < 30)  //we do not go above 100
        {
          feed_MenuValue++;
        } else {
          feed_MenuValue = 30;
        }
      } else {
        if (feed_MenuValue < 1)  //we do not go below 0
        {
          feed_MenuValue = 1;
        } else {
          // Encoder is rotating B direction so decrease
          feed_MenuValue--;
        }
      }
    }
    CLKPrevious = CLKNow;  // Store last CLK state
  }
  //---MENU4----------------------------------------------------------------
  else if (oscEna_selected == true) {
    CLKNow = digitalRead(RotaryCLK);  //Read the state of the CLK pin
    // If last and current state of CLK are different, then a pulse occurred
    if (CLKNow != CLKPrevious && CLKNow == 1) {
      // If the DT state is different than the CLK state then
      // the encoder is rotating in A direction, so we increase
      if (digitalRead(RotaryDT) != CLKNow) {
        if (osc_MenuEna < 2)  //we do not go above 100
        {
          osc_MenuEna++;
        } else {
          osc_MenuEna = 2;
        }
      } else {
        if (osc_MenuEna < 1)  //we do not go below 0
        {
          osc_MenuEna = 1;
        } else {
          // Encoder is rotating in B direction, so decrease
          osc_MenuEna--;
        }
      }
    }
    CLKPrevious = CLKNow;  // Store last CLK state
  }
  //---MENU2---------------------------------------------------------------
  else if (oscRng_selected == true) {
    CLKNow = digitalRead(RotaryCLK);  //Read the state of the CLK pin
    // If last and current state of CLK are different, then a pulse occurred
    if (CLKNow != CLKPrevious && CLKNow == 1) {
      // If the DT state is different than the CLK state then
      // the encoder is rotating in A direction, so we increase
      if (digitalRead(RotaryDT) != CLKNow) {
        if (oscRng_Menu < 15)  //we do not go above 100
        {
          oscRng_Menu++;
        } else {
          oscRng_Menu = 15;
        }
      } else {
        if (oscRng_Menu < -15)  //we do not go below 0
        {
          oscRng_Menu = -15;
        } else {
          // Encoder is rotating in B direction, so decrease
          oscRng_Menu--;
        }
      }
    }
    CLKPrevious = CLKNow;  // Store last CLK state
  }

  } else                   //MENU COUNTER----------------------------------------------------------------------------
  {
    CLKNow = digitalRead(RotaryCLK);  //Read the state of the CLK pin
    // If last and current state of CLK are different, then a pulse occurred
    if (CLKNow != CLKPrevious && CLKNow == 1) {
      // If the DT state is different than the CLK state then
      // the encoder is rotating in A direction, so we increase
      if (digitalRead(RotaryDT) != CLKNow) {
        if (menuCounter < 5)  //we do not go above 3
        {
          menuCounter++;
        } else {
          menuCounter = 0;
        }
      } else {
        if (menuCounter < 1)  //we do not go below 0
        {
          menuCounter = 5;
        } else {
          // Encoder is rotating CW so decrease
          menuCounter--;
        }
      }
    }
    CLKPrevious = CLKNow;  // Store last CLK state
  }

  //Refresh LCD after changing the counter's value
  refreshLCD = true;
}

void pushButton() {
  switch (menuCounter) {
    case 0:
      speed_selected = !speed_selected;  //we change the status of the variable to the opposite
      break;

    case 1:
      spin_selected = !spin_selected;
      break;

    case 2:
      feed_selected = !feed_selected;
      break;

    case 3:
      oscEna_selected = !oscEna_selected;
      break;
    case 4:
      oscRng_selected = !oscRng_selected;
      break;
  }

  refreshLCD = true;        //Refresh LCD after changing the value of the menu
  refreshSelection = true;  //refresh the selection ("X")
}

void printLCD() {
  //These are the values which are not changing the operation

  lcd.setCursor(1, 0);  //1st line, 2nd block
  lcd.print("Speed:");   //text
  //----------------------
  lcd.setCursor(1, 1);  //2nd line, 2nd block
  lcd.print("Spin:");    //text
  //----------------------
  lcd.setCursor(1, 2);  //3rd line, 2nd block
  lcd.print("Feed:");    //text
  //----------------------
  lcd.setCursor(1, 3);  //4th line, 2nd block
  lcd.print("Osc:");     //text
  //----------------------
  lcd.setCursor(10, 0);  //1st line, 14th block
  lcd.print("OscRng: ");    //counts - text
  //----------------------
  lcd.setCursor(10, 1);  //1st line, 14th block
  lcd.print("OscSpd: ");    //counts - text
}

void updateLCD() {
  lcd.setCursor(17, 0);    //1st line, 18th block
  lcd.print(menuCounter);  //counter (0 to 3)

  lcd.setCursor(7, 0);     //1st line, 10th block
  lcd.print("   ");        //erase the content by printing space over it
  lcd.setCursor(7, 0);     //1st line, 10th block
  lcd.print(speed_MenuValue);  //print the value of speed_MenuValue variable

  lcd.setCursor(9, 1);
  lcd.print("   ");
  lcd.setCursor(7, 1);
  lcd.print(spin_MenuValue);  //

  lcd.setCursor(7, 2);
  lcd.print("   ");
  lcd.setCursor(7, 2);
  lcd.print(feed_MenuValue);  //

  if (osc_MenuEna == 1) {
    lcd.setCursor(5, 3);
    lcd.print("   ");
    lcd.setCursor(5, 3);
    lcd.print("OFF");  //
  }
  if (osc_MenuEna == 2) {
    lcd.setCursor(5, 3);
    lcd.print("   ");
    lcd.setCursor(6, 3);
    lcd.print("ON");
  }
  lcd.setCursor(17, 0);
  lcd.print("   ");
  lcd.setCursor(17, 0);
  lcd.print(oscRng_Menu);  //
}

void updateCursorPosition() {
  //Clear display's ">" parts
  lcd.setCursor(0, 0);  //1st line, 1st block
  lcd.print(" ");       //erase by printing a space
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.setCursor(0, 2);
  lcd.print(" ");
  lcd.setCursor(0, 3);
  lcd.print(" ");
  lcd.setCursor(9, 0);
  lcd.print(" ");
  lcd.setCursor(9, 2);
  lcd.print(" ");

  //Place cursor to the new position
  switch (menuCounter)  //this checks the value of the counter (0, 1, 2 or 3)
  {
    case 0:
      lcd.setCursor(0, 0);  //1st line, 1st block
      lcd.print(">");
      break;
    //-------------------------------
    case 1:
      lcd.setCursor(0, 1);  //2nd line, 1st block
      lcd.print(">");
      break;
    //-------------------------------
    case 2:
      lcd.setCursor(0, 2);  //3rd line, 1st block
      lcd.print(">");
      break;
    //-------------------------------
    case 3:
      lcd.setCursor(0, 3);  //4th line, 1st block
      lcd.print(">");
      break;
          //-------------------------------
    case 4:
      lcd.setCursor(0, 9);  //4th line, 1st block
      lcd.print(">");
      break;
  }
}

void updateSelection() {
  //When a menu is selected ">" becomes "X"

  if (speed_selected == true) {
    lcd.setCursor(0, 0);  //1st line, 1st block
    lcd.print("X");
  }
  //-------------------
  if (spin_selected == true) {
    lcd.setCursor(0, 1);  //2nd line, 1st block
    lcd.print("X");
  }
  //-------------------
  if (feed_selected == true) {
    lcd.setCursor(0, 2);  //3rd line, 1st block
    lcd.print("X");
  }
  //-------------------
  if (oscEna_selected == true) {
    lcd.setCursor(0, 3);  //4th line, 1st block
    lcd.print("X");
  }
    //-------------------
  if (oscRng_selected == true) {
    lcd.setCursor(0, 9);  //4th line, 1st block
    lcd.print("X");
  }
}