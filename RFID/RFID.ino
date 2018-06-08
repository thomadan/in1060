// kode for den delen av prototypen som tar imot RFID-brikker

// importere RFID-biblioteket: https://github.com/miguelbalboa/rfid    
#include <MFRC522.h> 
  
// importere bibliotekt for SPI-protokollen, som brukes av RFID      
#include <SPI.h>                    

const int SS_PIN = 10;                    // pin for RFID-leseren
const int RST_PIN = 9;                    // pin for RFID-leseren
const int ARDUINO_LINK = 3;               // pin for link-kabel mellom arduino-ene
bool rfidStatus = false;                  // status for RFID 
unsigned long brikkeSistGodkjent = 0;     // holde tiden på når brikken sist ble godkjent
unsigned long brikkeSistRegistrert = 0;   // holde tiden på når brikken sist ble registrert
int debounceForsinkelse = 500;            // hvor lenge det må gå mellom hver godkjenning
String lestKode = "";                     // holder for koden som leses fra en RFID-brikke

// opprette instans av MFRC522-klassen
MFRC522 mfrc522(SS_PIN, RST_PIN);

// oppsett før loopen begynner
void setup() {
  SPI.begin();                            // starte SPI
  mfrc522.PCD_Init();                     // klargjøre MFRC522
  pinMode(ARDUINO_LINK, OUTPUT);          // setter link-pin til ut-modus
}

// koden som går igjen og igjen
void loop() {
 
  // se etter nye kort
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // lese av kort
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // få tak i koden til kortet, byte for byte
  for (byte i = 0; i < mfrc522.uid.size; i++) {

     // legge til tegn i strengen for avlest kode
     // hvis byte er bare ett tegn, legg til en 0, slik at det blir to tall: "00"
     lestKode.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));

     // legge til tegn i lestKode, som HEX
     lestKode.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  // hvis det er kort 1, gi beskjed til den andre arduinoen
  // begynne på 1, siden 0 er mellomrom
  if (lestKode.substring(1) == "33 73 80 b9") {

    // si fra at brikken ble registrert nå
    brikkeSistRegistrert = millis();

    // hvis det er lenge nok siden sist kortet ble godkjent
    // slik at det ikke kommer mange godkjenninger selv om 
    // bruker bare mener å trekke kortet en gang
    if (sjekkDebounce() == true) {

      // si fra at kortet er godkjent
      rfidState = true;

      // markere tiden kortet sist ble godkjent, for debounce
      brikkeSistGodkjent = millis();

      // sette link-pin til på, slik at den andre arduinoen får beskjed
      digitalWrite(ARDUINO_LINK, HIGH);
    }
  }

// hvis det er kort 2, si fra til den andre arduinoen at den skal skru av
  else if (lestKode.substring(1) == "70 4d 92 58") {
    
    // si fra til den andre arduinoen at den skal skru av
    digitalWrite(ARDUINO_LINK, LOW);

    // markere at RFID er av
    rfidState = 0;
  }
} 

// sjekke om det har gått rimelig tid siden sist kortet ble godkjent
bool sjekkDebounce() {

  // hvis det har gått lenger enn debounce-tiden ..
  if ((millis() - brikkeSistGodkjent) > debounceDelay) {

    // .. og hvis RFID ikke allerede er på
    if (rfidState == false) {
      return true;
    }
  }

  // hvis det ikke har gått lang nok tid siden sist, ikke godkjenn brikken
  return false;
}
