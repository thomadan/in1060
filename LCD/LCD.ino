// kode for den arudinoen som styrer LCD

// hent inn LCD-biblioteket som følger med arduinos IDE
#include <LiquidCrystal.h>

// sette pins til LCD (konstant fordi disse bør ikke kunne endres)
const int rs = 12,          // switch for å sende enten kommando eller data til LCD (pin 4 på LCD)
          en = 11,          // åpne for skriving til LCD (pin 6 på LCD)
          d4 = 5,           // pins for å sende 8-bit data til LCD (bruker bare bakerste 4 bits) 
          d5 = 4,
          d6 = 3,
          d7 = 2;

// opprette instans av LCD-klassen med pins som parametere
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int ARDUINO_LINK = 6;             // pin for input fra RFID-boksen
const int LED = 8;                      // pin for LED
String kylling = "Tomt for kylling";    // tekst som skal vises på LCD

// oppsett av det som skal skje før koden begynner å loope
void setup() {
  
  // sette modus for LED-pin til ut
  pinMode(LED, OUTPUT);               

  // starte lcd-objektet, med parametere kolonner og rader av tegn
  lcd.begin(16, 2);
  
  // skrive første beskjed til skjerm
  lcd.print("Venter på beskjed fra RFID");
}

// denne metoden går i løkke så lenge systemet er på
void loop() {

  // hvis det kommer beskjed fra den andre arduinoen
  if (digitalRead(ARDUINO_LINK)) {

    // sette pekeren til hvor på LCD-en det skal skrives
    lcd.setCursor(0, 0);

    // skrive beskjed
    lcd.print(kylling);

    // skru på LED
    digitalWrite(LED, HIGH);
  }

  // hvis det ikke er beskjed fra den andre arduinoen
  else {

    // viske ut beskjed på LCD
    lcd.setCursor(0, 0);
    lcd.print("                ");

    // skru av LED
    digitalWrite(LED, LOW);
  }
}
