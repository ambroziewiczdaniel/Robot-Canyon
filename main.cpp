#include <hFramework.h>
#include <DistanceSensor.h>
using namespace hModules;
// Schemat podpinania:
// Silnik napędowy - hMot1
// Silnik skręcający - hMot2
// Silnik podnoszący - hMot3
// Silnik przesuwający most - hMot4
// Czujnik prawy - sensprawy(hSens1);
// Czujnik lewy - senslewy(hSens5);
DistanceSensor sensprawy(hSens1);
DistanceSensor senslewy(hSens5);
bool state2 = 0;
int state3 = 0;
int distprawy = 1;
int distlewy = 1; //musialem zadeklarowac oba tutaj, poniewaz uzywam ich w void
int maxstol = 10; //MAKSYMALNA WYKRYWANA ODLEGLOSC STOLU OD POJAZDU, ZMIENIĆ, ZALEŻNE OD WYSOKOŚCI POJAZDU
int dlugoscmostu = 3000; // SPRAWDZIC PRZY JAKIEJ LICZBIE POJAZD PRZEJEZDZA CALY MOST
int maxstolmost = 15; //wysokosc przy ktorej most widzi stoł
void skretblok(int ilosc, int moc)
{
    hMot2.rotRel(ilosc, moc, true, INFINITE);
}
void skret(int ilosc, int moc)
{
    hMot2.rotRel(ilosc, moc, false, INFINITE);
}
void jazda(int ilosc,int moc)
{
    hMot1.rotRel(ilosc, moc, false, INFINITE);
}
void jazdablok(int ilosc,int moc)
{
    hMot1.rotRel(ilosc, moc, true, INFINITE);
}
void prosto(int ilosc,int moc)
{
    hMot2.rotAbs(0,200,true, INFINITE); // SPRAWDZIC DLA JAKIEJ "ILOSCI" POJAZD JEDZIE PROSTO, niekoniecznie 0 wyprostuje kola pojazdu
    hMot1.rotRel(ilosc, moc, false, INFINITE);
}
void podnoszenie(int ilosc, int moc)
{
    hMot3.rotRel(ilosc,moc,true,INFINITE);
}
void przesuwanie(int ilosc, int moc)
{
    Serial.printf("Wysuwanie mostu.\n");
    hMot4.rotRel(dlugoscmostu,300,true,INFINITE); //sprawdzic czy dlugosc calego mostu jest osiagnieta
    Serial.printf("Koniec wysuwania mostu\n");
    Serial.printf("Sprawdzamy, czy wykryto stół.\n");
    DistanceSensor sensprawy(hSens1);
    DistanceSensor senslewy(hSens5);
    int distprawy = sensprawy.getDistance();
	int distlewy = senslewy.getDistance();
	Serial.printf("Prawy:%d\r\n", distprawy);
	Serial.printf("Lewy:%d\r\n", distlewy);
    if (distlewy < maxstolmost && distprawy < maxstolmost)
    {
        Serial.printf("Wykryto stół.\n");
        Serial.printf("Podnosimy pojazd.\n");
        hMot3.rotRel(300,300,true);
        Serial.printf("Pojazd podniesiony.\n");
        Serial.printf("Przejazd pojazdu po moście.\n");
        hMot4.rotRel((-(dlugoscmostu)),300,true,INFINITE); //przejazd pojazdu po moscie
        Serial.printf("Pojazd przejechał po moście.\n");
        Serial.printf("Opuszczanie pojazdu.\n");
        hMot3.rotRel(-300,300,true);
        Serial.printf("Przeciąganie mostu.\n");
        hMot4.rotRel(((-dlugoscmostu)/2),300,true,INFINITE);
        



    }
    else             //nie wykryto stołu, cofamy most
    {
        Serial.printf("Nie wykryto stołu.\n");
        Serial.printf("Przeciąganie mostu.\n");
        hMot4.rotRel((-(dlugoscmostu)),300,true,INFINITE);
        Serial.printf("KONIEC PROCEDURY.\n");

        exit(1); //pojazd pozostaje w spoczynku
        
    }
}
void hMain()
{
	while (true)
    {
        bool state1 = hBtn1.isPressed();
        Serial.printf("%d/n",state1);
        if (state1 == true)
        {
            Serial.printf("Tutaj doszło\n");
            state3 += 1;
        } 

        if (state3 > 0)
        {
            {
        Serial.printf("Tutaj doszło\n");
        Serial.printf("Tutaj doszło\n");

        for (;;)
{
    printf("Start!\n");
    DistanceSensor sensprawy(hSens1);
    DistanceSensor senslewy(hSens5);
    int distprawy = sensprawy.getDistance();
	int distlewy = senslewy.getDistance();
	Serial.printf("Prawy:%d\r\n", distprawy);
	Serial.printf("Lewy:%d\r\n", distlewy);


        if (distlewy < maxstol && distprawy < maxstol) // pojazd nie wykryl krawedzi stołu, jedzie prosto
            {

            Serial.printf("Prosto. \n ");
            hMot2.rotAbs(0,300,true);
            hMot1.setPower(600);
            sys.delay(400);

            }
        if (distlewy < maxstol && distprawy > maxstol) // pojazd wykrył prawą krawędź stołu
            {
            Serial.printf("Prawa krawedz. \n "); //słuzy do wyprostowania kół pojazdu
            jazdablok(-130,300);
            skret(300,300); //skret w lewo
            sys.delay(300);
            jazda(300,600);
            sys.delay(1000);
            sys.delay(500);
            }
        
            
        if (distlewy > maxstol && distprawy < maxstol) // pojazd wykrył lewą krawędź stołu
            {
            
            Serial.printf("Lewa krawedz \n");
            jazdablok(-130,300);
            skret(-300,300); //skret w lewo
            sys.delay(300);
            jazda(300,600);
            sys.delay(1000);
            sys.delay(500);
            }

        if (distlewy > maxstol && distprawy > maxstol)
        {
            Serial.printf("Wykryto krawędź stołu.\n ");
            przesuwanie(dlugoscmostu,400); //sprawdzic jaka ilosc przesuwa cały most;
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
		sys.setSysLogDev(&devNull);								//turn off system logs on Serial
		sys.setLogDev(&Serial);									// setting USB-serial as a default printf output
		hExt.serial.init(9600, Parity::None, StopBits::One);
        for (;;)
{
            bool state1 = hBtn1.isPressed();
        Serial.printf("%d\n",state3);
        if (state1 == true)
        {
            state3 +=1;
            sys.delay(2000);
            break;
        } 
        if (state3>0)
        {
            break;
        }
if (hExt.serial.read(received_data, 8, 500)) // checking Serial availability
		{
			char x = received_data[0]; // getting one character from Serial
			switch (x)				 // decision based on received character
			{
			case 'w': //jazda prosto
				Serial.printf("Jazda prosto\r\n"); 
				hMot1.setPower(800);
				break;
			case 's': //jazda do tyłu
				Serial.printf("Jazda do tyłu\r\n"); 
				hMot1.setPower(-800);
				break;
			case 'a': //jazda w lewo
				Serial.printf("Jazda w lewo\r\n"); 
				hMot2.rotAbs(200,500);
				break;
			case 'd': //jazda w prawo
				Serial.printf("Jazda w prawo\r\n"); 
				hMot2.rotAbs(-200,500);
				break;
			case 'g': //koła prosto
				Serial.printf("Koła prosto\r\n"); 
				hMot2.rotAbs(0,500);
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
