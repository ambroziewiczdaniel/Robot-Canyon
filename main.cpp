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
        while ((distlewy < maxstol) && (distprawy < maxstol)) //pojazd znajduje sie na stole, czujniki nie wykryly krawedzi
        {
            prosto(100,100);
        } 
        while ((distlewy < maxstol) && (distprawy > maxstol)) //prawy czujnik wykryl podloge, lewy nie, pojazd jest za bardzo w prawo
        {
            prosto(200,-100); //cofamy pojazd by nie spadł przy skręcaniu
            skretblok(100, -100); //pojazd skreca, SPRAWDZIC CZY W DOBRA STRONE
            jazda(100,100);
            prosto(0,0); //słuzy do wyprostowania kół pojazdu

        } 
        while ((distlewy > maxstol) && (distprawy < maxstol)) //lewy czujnik wykryl podloge, prawy nie, pojazd jest za bardzo w lewo
        {
            prosto(200,-100); //cofamy pojazd by nie spadł przy skręcaniu
            skretblok(100, 100); //pojazd skreca, SPRAWDZIC CZY W DOBRA STRONE
            jazda(100,100);
            prosto(0,0); //słuzy do wyprostowania kół pojazdu

        } 
        while ((distlewy > maxstol) && (distprawy > maxstol)) //pojazd wykryl prawidlowe ustawienie nad krawedzia stolu
        {
            //procedura wysuwania mostu

        } 

     
        }
    }
}