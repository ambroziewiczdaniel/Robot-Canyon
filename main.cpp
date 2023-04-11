#include <hFramework.h>
#include <DistanceSensor.h>
using namespace hModules;
// Schemat podpinania:
// Silnik napędowy - hMot1
// Silnik skręcający - hMot2
//Czujnik prawy - sensprawy(hSens1);
//Czujnik lewy - senslewy(hSens2);

int maxstol = 30; //MAKSYMALNA WYKRYWANA ODLEGLOSC STOLU OD POJAZDU, ZMIENIĆ, ZALEŻNE OD WYSOKOŚCI POJAZDU
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
void prosto(int ilosc,int moc)
{
    hMot2.rotAbs(0,100,true, INFINITE); // SPRAWDZIC DLA JAKIEJ "ILOSCI" POJAZD JEDZIE PROSTO, niekoniecznie 0 wyprostuje kola pojazdu
    hMot1.rotRel(ilosc, moc, false, INFINITE);
}
void hMain()
{
    while (true)
    {
        DistanceSensor sensprawy(hSens1);
        DistanceSensor senslewy(hSens2);
        bool przycisk = hBtn1.isPressed();
        if (przycisk)
        {
        sys.delay(2000);
        hMot1.setEncoderPolarity(Polarity::Reversed); //ustawianie polaryzacji silników i encoderów
        hMot1.setMotorPolarity(Polarity::Normal);
        hMot2.setEncoderPolarity(Polarity::Reversed);
        hMot2.setMotorPolarity(Polarity::Normal);

        int distlewy = senslewy.getDistance();
        int distprawy = senslewy.getDistance();
        switch (true) {
        case (distlewy < maxstol && distprawy < maxstol): // pojazd nie wykryl krawedzi stołu, jedzie prosto
            while (true) {
            prosto(100,100);
            if (distlewy >= maxstol || distprawy >= maxstol) break;
            }
            break;
        case (distlewy < maxstol && distprawy > maxstol): // pojazd wykrył prawą krawędź stołu
            while (true) {
            prosto(200,-100); //cofamy pojazd by nie spadł przy skręcaniu
            skretblok(100, -100); //pojazd skreca, SPRAWDZIC CZY W DOBRA STRONE
            jazda(100,100);
            prosto(0,0); //słuzy do wyprostowania kół pojazdu
            if (distlewy >= maxstol && distprawy <= maxstol) break;
            }
            break;
        case (distlewy > maxstol && distprawy < maxstol): // pojazd wykrył lewą krawędź stołu
            while (true) {
            prosto(200,-100); //cofamy pojazd by nie spadł przy skręcaniu
            skretblok(100, 100); //pojazd skreca, SPRAWDZIC CZY W DOBRA STRONE
            jazda(100,100);
            prosto(0,0); //słuzy do wyprostowania kół pojazdu
            if (distlewy <= maxstol && distprawy >= maxstol) break;
            }
            break;
        case (distlewy > maxstol && distprawy > maxstol):
            //pojazd wykrył prawidłowe ustawienie nad krawędzią stołu
            break;
        default:
            break;
        }

     
        }
    }
}