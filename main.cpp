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
        hMot1.setEncoderPolarity(Polarity::Reversed); //ustawianie polaryzacji silników i encoderów
        hMot1.setMotorPolarity(Polarity::Normal);
        hMot2.setEncoderPolarity(Polarity::Reversed);
        hMot2.setMotorPolarity(Polarity::Normal);
        hMot3.setEncoderPolarity(Polarity::Reversed); //ustawianie polaryzacji silników i encoderów
        hMot3.setMotorPolarity(Polarity::Normal);
        hMot4.setEncoderPolarity(Polarity::Reversed);
        hMot4.setMotorPolarity(Polarity::Normal);
        DistanceSensor sensprawy(hSens1);
        DistanceSensor senslewy(hSens5);
        for (;;)
{
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
            hMot1.setPower(400);
            sys.delay(400);

            }
        if (distlewy < maxstol && distprawy > maxstol) // pojazd wykrył prawą krawędź stołu
            {
            Serial.printf("Prawa krawedz. \n "); //słuzy do wyprostowania kół pojazdu
            jazdablok(-530,300);
            skret(300,200); //skret w lewo
            sys.delay(300);
            jazda(1230,300);
            sys.delay(1000);
            sys.delay(500);
            }
        
            
        if (distlewy > maxstol && distprawy < maxstol) // pojazd wykrył lewą krawędź stołu
            {
            
            Serial.printf("Lewa krawedz \n");
            jazdablok(-530,300);
            skret(-300,200); //skret w lewo
            sys.delay(300);
            jazda(1230,300);
            sys.delay(1000);
            sys.delay(500);
            }

        if (distlewy > maxstol && distprawy > maxstol)
        {
            Serial.printf("Wykryto krawędź stołu.\n ");
            przesuwanie(dlugoscmostu,100); //sprawdzic jaka ilosc przesuwa cały most;
            sys.delay(1000);
        }
    }   
     
        
}
