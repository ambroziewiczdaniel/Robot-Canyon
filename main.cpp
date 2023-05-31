#include <hFramework.h>

#include <DistanceSensor.h>

using namespace hModules;
// Schemat podpinania:
// Silnik napędowy - hMot3
// Silnik skręcający - hMot2
// Silnik podnoszący - hMot1
// Silnik przesuwający most - hMot4
// Czujnik prawy - sensprawy(hSens1);
// Czujnik lewy - senslewy(hSens5);
DistanceSensor sensprawy(hSens5);
DistanceSensor senslewy(hSens1);
DistanceSensor sensmost(hSens3);
int wykryto = 0;
int skrettak = 0;
bool state1 = 0;
int state3 = 0;
int distprawy = 1;
int distlewy = 1;
int distmost = 1;
int maxstol = 10; //MAKSYMALNA WYKRYWANA ODLEGLOSC STOLU OD POJAZDU
int dlugoscmostu = 105000; // długość mostu w tickach silnika
int maxstolmost = 28; //wysokosc przy ktorej most widzi stoł
int brakskretu = 0;
void przesuwanie(int ilosc, int moc) {
  hMot1.setPower(0); //wyłaczanie silnika napędowego
  hMot2.rotAbs(0, 500); //prostowanie kół

  Serial.printf("Wysuwanie mostu.\n");
  DistanceSensor sensprawy(hSens5); //pobieranie danych z czujników
  DistanceSensor senslewy(hSens1);
  DistanceSensor sensmost(hSens3);
  int distprawy = sensprawy.getDistance();
  int distlewy = senslewy.getDistance();
  int distmost = sensmost.getDistance();
  Serial.printf("Prawy:%d\r\n", distprawy);
  Serial.printf("Lewy:%d\r\n", distlewy);
  Serial.printf("Most:%d\r\n", distmost);
  if (distmost > maxstolmost) //jeżeli czujnik na moście nie wykrywa stołu
  {
    Serial.printf("Nie wykryto stołu\n");
    hMot1.setPower(0);
    hMot2.setPower(0);
    hMot3.setPower(0);
    hMot4.setPower(0);
    exit(1);

  }
  if (distmost <= maxstolmost) //jeżeli czujnik na moście wykryje stół
  {

    Serial.printf("Wykryto stół!\n");
    Serial.printf("%d\n", distmost);
    hMot4.rotAbs(-dlugoscmostu, 800, true); //wysuwamy most

    Serial.printf("Podnosimy pojazd.\n");
    hMot3.rotRel(12000, 800, true);
    Serial.printf("Pojazd podniesiony.\n");
    Serial.printf("Przejazd pojazdu po moście.\n");
    hMot4.rotRel((30000), 800, true, INFINITE); //przejazd pojazdu po moscie
    Serial.printf("Pojazd przejechał po moście.\n");
    while (wykryto < 1) //jeżeli żaden czujnik nie wykrył mostu
    {
      hMot4.rotRel(10000, 900, true);
      hMot1.setPower(100);
      distprawy = sensprawy.getDistance();
      distlewy = senslewy.getDistance();
      if (distprawy < maxstolmost) {
        wykryto += 1;
      }
      if (distlewy < maxstolmost) //jeżeli czujniki wykryją most, zmienna > 0
      {
        wykryto += 1;
      }
    }
    wykryto = 0;
    hMot4.setPower(0);
    hMot1.setPower(0);
    hMot4.rotRel(108000, 900, true); //przesuwamy pojazd o jego długość po moście
    Serial.printf("Opuszczanie pojazdu.\n");
    hMot3.rotRel(-12000, 800, true);
    Serial.printf("Przeciąganie mostu.\n");
    hMot4.rotAbs(0, 900, true);
    brakskretu += 1; //zmienna powodująca inne zachowanie skręcania przy pierwszym skręcie po pokonaniu mostu
  }

}
void hMain() {
  while (true) {
    DistanceSensor sensprawy(hSens5);
    DistanceSensor senslewy(hSens1);
    DistanceSensor sensmost(hSens3);
    int distprawy = sensprawy.getDistance();
    int distlewy = senslewy.getDistance();
    int distmost = sensmost.getDistance();
    Serial.printf("Prawy:%d\r\n", distprawy);
    Serial.printf("Lewy:%d\r\n", distlewy);
    Serial.printf("Most:%d\r\n", distmost);
    bool state1 = hBtn1.isPressed(); //bool zmieniający wartość gdy przycisk zostaje wcisniety
    Serial.printf("%d/n", state1);
    if (state1 == true) { //jeżeli przycisk zostaje wciśnięty, zmienna state3 > 0
      state3 += 1;
    }

    if (state3 > 0) { //jeżeli zmienna state3 > 0, rozpoczynamy procedurę autonomiczną
      {

        for (;;) {
          DistanceSensor sensprawy(hSens5);
          DistanceSensor senslewy(hSens1);
          DistanceSensor sensmost(hSens3);
          int distprawy = sensprawy.getDistance();
          int distlewy = senslewy.getDistance();
          int distmost = sensmost.getDistance();
          Serial.printf("Prawy:%d\r\n", distprawy);
          Serial.printf("Lewy:%d\r\n", distlewy);
          Serial.printf("Most:%d\r\n", distmost);

          if (distlewy < maxstol && distprawy < maxstol) // pojazd nie wykryl krawedzi stołu, jedzie prosto
          {
            skrettak = 0; //zmienna odpowiadająca za stopień sketu kół

            Serial.printf("Prosto. \n ");
            hMot1.setPower(200);
            hMot2.rotAbs(0, 300, true);
            sys.delay(400);

          }
          if (distlewy < maxstol && distprawy > maxstol) // pojazd wykrył prawą krawędź stołu
          {
            Serial.printf("Prawa krawedz 50%. \n "); //instrukcja do sprawdzenia czy na pewno nie znajdujemy się nad krawędzia stołu
            hMot1.setPower(200);
            sys.delay(200);
            if (distlewy > maxstol) {
              Serial.printf("Prawa krawedz cofamy \n ");
              hMot1.setPower(-200);
              sys.delay(600);
              hMot1.setPower(0);
            } else {

              if (brakskretu > 0) {
                Serial.printf("Prawa krawedz. \n "); //słuzy do wyprostowania kół pojazdu

                hMot2.rotAbs(-200, 500, true);
                hMot1.setPower(200);
                sys.delay(700);
                hMot1.setPower(0);
                hMot2.rotAbs(0, 500, false);
                distprawy = sensprawy.getDistance();
                distlewy = senslewy.getDistance();
                brakskretu = 0;
              } else {
                skrettak -= 30;
                if (skrettak < -130) {
                  skrettak = -130;
                }
                Serial.printf("Prawa krawedz. \n "); //słuzy do wyprostowania kół pojazdu
                hMot1.setPower(0);
                hMot2.rotAbs(skrettak, 500, true);
                hMot1.setPower(200);
                sys.delay(700);
                hMot1.setPower(0);
                distprawy = sensprawy.getDistance();
                distlewy = senslewy.getDistance();
              }
            }
          }

          if (distlewy > maxstol && distprawy < maxstol) // pojazd wykrył lewą krawędź stołu
          {
            hMot1.setPower(200);
            sys.delay(200);
            if (distprawy > maxstol) {
              hMot1.setPower(-200);
              sys.delay(600);
              hMot1.setPower(0);
            } else {
              if (brakskretu > 0) {
                hMot2.rotAbs(200, 500, true);
                hMot1.setPower(200);
                sys.delay(700);
                hMot1.setPower(0);
                hMot2.rotAbs(0, 500, false);
                distprawy = sensprawy.getDistance();
                distlewy = senslewy.getDistance();
                brakskretu = 0;
              } else
                skrettak += 30;
              if (skrettak > 130) {
                skrettak = 130;
              }
              Serial.printf("Lewa krawedz \n");
              hMot1.setPower(0);
              hMot2.rotAbs(skrettak, 500, true);
              hMot1.setPower(200);
              sys.delay(700);
              hMot1.setPower(0);
              distprawy = sensprawy.getDistance();
              distlewy = senslewy.getDistance();
            }
          }

          if (distlewy > maxstol && distprawy > maxstol) {
            Serial.printf("Wykryto krawędź stołu.\n ");
            distmost = sensmost.getDistance();
            przesuwanie(dlugoscmostu, 800); //sprawdzic jaka ilosc przesuwa cały most;
            sys.delay(1000);
          }
        }

      }
    }

    // Kod dla trybu manualnego
    hMot1.setEncoderPolarity(Polarity::Reversed); //ustawianie polaryzacji silników i encoderów
    hMot1.setMotorPolarity(Polarity::Normal);
    hMot2.setEncoderPolarity(Polarity::Reversed);
    hMot2.setMotorPolarity(Polarity::Normal);
    hMot3.setEncoderPolarity(Polarity::Reversed); //ustawianie polaryzacji silników i encoderów
    hMot3.setMotorPolarity(Polarity::Normal);
    hMot4.setEncoderPolarity(Polarity::Reversed);
    hMot4.setMotorPolarity(Polarity::Normal);
    char received_data[20];
    sys.setSysLogDev( & devNull); //turn off system logs on Serial
    sys.setLogDev( & Serial); // setting USB-serial as a default printf output
    hExt.serial.init(9600, Parity::None, StopBits::One);
    for (;;) {
      DistanceSensor sensprawy(hSens5);
      DistanceSensor senslewy(hSens1);
      DistanceSensor sensmost(hSens3);
      int distprawy = sensprawy.getDistance();
      int distlewy = senslewy.getDistance();
      int distmost = sensmost.getDistance();
      Serial.printf("Prawy:%d\r\n", distprawy);
      Serial.printf("Lewy:%d\r\n", distlewy);
      Serial.printf("Most:%d\r\n", distmost);
      bool state1 = hBtn1.isPressed();
      Serial.printf("%d\n", state3);
      if (state1 == true) {
        state3 += 1;
        sys.delay(2000);
        break;
      }
      if (state3 > 0) {
        break;
      }
      if (hExt.serial.read(received_data, 8, 500)) // checking Serial availability
      {
        char x = received_data[0]; // getting one character from Serial
        switch (x) // decision based on received character
        {
        case 'w': //jazda prosto
          Serial.printf("Jazda prosto\r\n");
          hMot1.setPower(500);
          break;
        case 's': //jazda do tyłu
          Serial.printf("Jazda do tyłu\r\n");
          hMot1.setPower(-500);
          break;
        case 'a': //jazda w lewo
          Serial.printf("Jazda w lewo\r\n");
          hMot2.rotAbs(-100, 500);
          break;
        case 'd': //jazda w prawo
          Serial.printf("Jazda w prawo\r\n");
          hMot2.rotAbs(100, 500);
          break;
        case 'g': //koła prosto
          Serial.printf("Koła prosto\r\n");
          hMot2.rotAbs(0, 500);
          break;
        case 'i': //podnoszenie
          Serial.printf("Podnoszenie\r\n");
          hMot3.setPower(800);
          break;
        case 'k': //opuszczanie
          Serial.printf("Opuszczanie\r\n");
          hMot3.setPower(-800);
          hLED1.on();
          break;
        case 'o': //przesuwanie do przodu
          Serial.printf("Przesuwanie do przodu\r\n");
          hMot4.setPower(900);
          break;
        case 'l': //przesuwanie do tyłu
          Serial.printf("Przesuwanie do tyłu\r\n");
          hMot4.setPower(-900);
          break;
        case 'r': //reset
          hMot1.setPower(0);
          hMot3.setPower(0);
          hMot4.setPower(0);
          break;
        case 'p': //tryb auto
          state3 += 1;
          break;
        default: // in other case
          hMot1.setPower(0);
          hMot2.setPower(0);
          hMot3.setPower(0);
          hMot4.setPower(0);
          break;
        }

      }
      sys.delay(10);
    }
  }
}