#include <iostream>
#include <ctime>
#include "Restauracja.h"


using namespace std;
int main()
{
 
    srand(static_cast<unsigned int>(time(NULL)));
    setlocale(LC_ALL, "");

    
    cout << "============================================" << endl;
    cout << "    WITAJ W SYMULATORZE RESTAURACJI 2026    " << endl;
    cout << "============================================" << endl;

  
    Resturacja r(5000, 20); 
    cout << "\nTwoja ekipa startowa gotowa do pracy:" << endl;
    cout << "-> Robert (Kucharz):  Pensja 30/h, Wytrzymalosc 2h, Zabrudzenie-5" << endl;
    cout << "-> Alicja (Kelnerka): Pensja 20/h, Wytrzymalosc 3h, Zabrudzenie -2" << endl;
    cout << "--------------------------------------------" << endl;
 
    r.zatrudnij(make_unique<Kucharz>("Robert (Start)", 30.0, 2, -5));
    r.zatrudnij(make_unique<Kelner>("Alicja (Start)", 20.0, 3, -2));

    
    r.zacznij_gre();

    cout << "\nDziekujemy za gre!" << endl;
    return 0;
}

