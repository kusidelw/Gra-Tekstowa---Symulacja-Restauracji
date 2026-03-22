#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include "Danie.h"
#include "Pracownik.h"

using namespace std;

// ==========================

class Resturacja
{
private:
    int liczba_dni;
    double budzet;
    double pocztakowy_budzet;
    int poziom_prestizu;
    int prestiz;
    int poziom_czystosci;
    vector<unique_ptr<Pracownik>> pracownicy;
    vector<Danie> menu;
    vector<Danie> gotowe_zestawy;
    double dzienny_bilans_losowy; // Zmienna do raportu

public:
    ~Resturacja() {};
    Resturacja(double startowy_budzet, int dni_gry)
    {
        pocztakowy_budzet = startowy_budzet;
        poziom_czystosci = 100;
        prestiz = 0;
        poziom_prestizu = 1;
        liczba_dni = dni_gry;
        budzet = pocztakowy_budzet;
        menu.push_back({"Zestaw 1", 10.0, 25.0});
        menu.push_back({"Zestaw 2", 15.0, 40.0});
        menu.push_back({"Zestaw 3", 20.0, 75.0});
    };

    void zatrudnij(unique_ptr<Pracownik> p)
    {
        pracownicy.push_back(std::move(p));
    }

    void zmien_prestiz(int zmiana)
    {
        if (zmiana > 0)
        {
            while (prestiz >= 50)
            {
                if (poziom_prestizu < 5)
                {
                    poziom_prestizu++;
                    prestiz -= 50;
                    cout << "!!! RESTAURACJA AWANSOWALA NA POZIOM " << poziom_prestizu << " !!!" << endl;
                }
                else
                {
                    prestiz = 50; // Maksymalny prestiz to Lvl 5 i 50/50 punktow
                    break;
                }
            }
        }
        else
        {
            if (prestiz < zmiana && poziom_prestizu == 1)
            {
                cout << "Prestiż jest juz na dnie... Klienci omijaja lokal!" << endl;
                prestiz = 0;
            }
            else if (prestiz < zmiana)
            {
                prestiz = 50 + prestiz - zmiana; // Przy obnizeniu prestizu o 20, jesli mamy tylko 10 prestizu, to spadamy do 40 i tracimy poziom prestizu
                poziom_prestizu--;
                cout << "!!! [UWAGA] Poziom prestizu spada na " << poziom_prestizu << " !!!" << endl;
            }
            else
            {
                prestiz -= zmiana;
            }
        }
    }

    void wyslij_na_szkolenie(int numer_pracownika)
    {
        double koszt_szkolenia = 300.0;

        if (numer_pracownika >= 0 && numer_pracownika < pracownicy.size())
        {

            if (pracownicy[numer_pracownika]->get_poziom() >= 3)
            {
                cout << "Ten pracownik osiagnal juz maksymalny 3. poziom doswiadczenia!" << endl;
                return;
            }

            if (budzet >= koszt_szkolenia)
            {
                budzet -= koszt_szkolenia;
                pracownicy[numer_pracownika]->awansuj();
                cout << "Wydano " << koszt_szkolenia << " zl na szkolenie." << endl;
                zmien_prestiz(20);
            }
            else
            {
                cout << "Brak srodkow na szkolenie! Potrzebujesz " << koszt_szkolenia << " zl." << endl;
            }
        }
        else
        {
            cout << "Nie ma takiego pracownika!" << endl;
        }
    }

    void statystyki_dnia(int ugotowane, int sprzedane, double zarobek, double koszt, double wyplaty, int czystosc_start)
    {
        cout << "\n============================================" << endl;
        cout << "         RAPORT Z KONCA DNIA               " << endl;
        cout << "============================================" << endl;
        cout << " Ugotowano dan:      " << ugotowane << endl;
        cout << " Sprzedano dan:      " << sprzedane << endl;
        cout << " Dania na wydawce:   " << gotowe_zestawy.size() << endl;
        cout << "--------------------------------------------" << endl;
        cout << " Utarg ze sprzedazy: +" << zarobek << " zl" << endl;
        cout << " Koszt skladnikow:   -" << koszt << " zl" << endl;
        cout << " Wyplaty personelu:  -" << wyplaty << " zl" << endl;
        cout << " ZYSK/STRATA NETTO:  " << (zarobek - koszt - wyplaty) << " zl" << endl;
        cout << " Zdarzenia losowe:   " << dzienny_bilans_losowy << " zl" << endl;
        cout << " REALNY ZYSK/STRATA: " << (zarobek - koszt - wyplaty + dzienny_bilans_losowy) << " zl" << endl;
        cout << "--------------------------------------------" << endl;
        cout << " Zmiana czystosci:   " << czystosc_start << " -> " << poziom_czystosci << endl;
        cout << "============================================\n"
             << endl;
    }

    bool dzien_pracujacy()
    {
        double zarobek_z_dan = 0;
        double koszt_skladnikow_suma = 0;
        int ugotowane_produkty = 0;
        int sprzedane_produkty = 0;
        int czystosc_na_starcie = poziom_czystosci;
        int los2 = rand() % 100;

        dzienny_bilans_losowy = 0;

        for (int i = 0; i < 8; i++)
        {
            cout << "\n[Godzina " << i + 1 << "] ";
            int aktywni_pracownicy = 0;

            for (auto &pracownik : pracownicy)
            {
                if (dynamic_cast<Osoba_Sprzatajaca *>(pracownik.get()) == nullptr)
                {
                    if (pracownik->pracuj())
                    {
                        aktywni_pracownicy++;

                        Kucharz *kucharz = dynamic_cast<Kucharz *>(pracownik.get());
                        if (kucharz)
                        {

                            int ilosc_dan = (rand() % kucharz->get_poziom()) + 1;

                            for (int d = 0; d < ilosc_dan; d++)
                            {
                                int max_danie = kucharz->get_poziom() - 1;
                                if (max_danie >= menu.size())
                                    max_danie = menu.size() - 1;

                                int nr_dania = rand() % (max_danie + 1);

                                budzet -= menu[nr_dania].koszt_skladnikow;
                                koszt_skladnikow_suma += menu[nr_dania].koszt_skladnikow;
                                gotowe_zestawy.push_back(menu[nr_dania]);
                                ugotowane_produkty++;
                                cout << "(K: " << menu[nr_dania].nazwa << ") ";
                            }
                        }

                        Kelner *kelner = dynamic_cast<Kelner *>(pracownik.get());
                        if (kelner)
                        {
                            int podane_dania = 0;

                            int max_talerzy = kelner->get_poziom();

                            for (int t = 0; t < max_talerzy; t++)
                            {
                                if (!gotowe_zestawy.empty())
                                {
                                    double cena = gotowe_zestawy.back().cena_dla_klienta;
                                    budzet += cena;
                                    zarobek_z_dan += cena;
                                    gotowe_zestawy.pop_back();
                                    sprzedane_produkty++;
                                    podane_dania++;
                                }
                            }

                            if (podane_dania > 0)
                                cout << "[$ Sprzedano " << podane_dania << "!] ";
                            else
                                cout << "[!] ";
                        }
                    }
                    else
                    {
                        poziom_czystosci += pracownik->wplyw_na_czystosc();
                    }
                }
            }
            if (aktywni_pracownicy == 0)
                zmien_prestiz(-5);
            zdarzenie_losowe();
            if (poziom_czystosci < 0)
                poziom_czystosci = 0;
            if (poziom_czystosci > 100)
                poziom_czystosci = 100;
        }

        cout << "\n\n--- ZAMKNIECIE LOKALU (ROZLICZENIE WYPLAT) ---" << endl;

        double suma_wyplat = 0;
        for (auto &pracownik : pracownicy)
        {
            double w = pracownik->pobierz_wynagrodzenie();
            budzet -= w;
            suma_wyplat += w;
            pracownik->nastepny_dzien();
        }

        statystyki_dnia(ugotowane_produkty, sprzedane_produkty, zarobek_z_dan, koszt_skladnikow_suma, suma_wyplat, czystosc_na_starcie);

        while (prestiz >= 50)
        {
            if (poziom_prestizu < 5)
            {
                poziom_prestizu++;
                prestiz -= 50;
                cout << "!!! RESTAURACJA AWANSOWALA NA POZIOM " << poziom_prestizu << " !!!\n"
                     << endl;
            }
            else
            {
                prestiz = 50;
                break;
            }
        }

        if (budzet <= 0)
        {
            cout << "BANKRUCTWO! KOMORNIK ZAMYKA LOKAL.";
            return false;
        }
        return true;
    }

    void zacznij_gre()
    {
        int obecny_dzien = 1;
        while (obecny_dzien <= liczba_dni)
        {
            pokaz_status();
            cout << "\n--- DZIEN " << obecny_dzien << " z " << liczba_dni << " ---" << endl;
            cout << "1. Zatrudnij Kucharza | 2. Kelnera | 3. Sprzatacza" << endl;
            cout << "4. Szkolenie (300zl)  | 5. Lista   | 6. ZWOLNIJ" << endl;
            cout << "7. ROZPOCZNIJ DZIEN PRACY | 0. Wyjscie" << endl;
            cout << "Wybor: ";

            int wybor;
            cin >> wybor;

            if (wybor == 1)
            {
                cout << "\n--- DOSTEPNI KUCHARZE ---" << endl;
                cout << "1. Maciek (Tani) [Koszt: 400 zl] Pensja 15/h, Meczy sie po 2h, Brudzi -7" << endl;
                cout << "2. Ania   (Srednia) [Koszt: 800 zl] Pensja 40/h, Meczy sie po 4h, Brudzi -5" << endl;
                cout << "3. Janek  (Pro) [Koszt: 1500 zl] Pensja 150/h, Meczy sie po 7h, Brudzi -2" << endl;
                cout << "Wybierz kandydata: ";
                int pod_wybor;
                cin >> pod_wybor;

                if (pod_wybor == 1)
                {
                    budzet -= 400;
                    zatrudnij(make_unique<Kucharz>("Maciek", 15.0, 2, -7));
                    cout << "Zatrudniono Macka!\n";
                }
                else if (pod_wybor == 2)
                {
                    budzet -= 800;
                    zatrudnij(make_unique<Kucharz>("Ania", 40.0, 4, -5));
                    cout << "Zatrudniono Anie!\n";
                }
                else if (pod_wybor == 3)
                {
                    budzet -= 1500;
                    zatrudnij(make_unique<Kucharz>("Janek", 150.0, 7, -2));
                    cout << "Zatrudniono Janka!\n";
                }
            }
            else if (wybor == 2)
            {
                cout << "\n--- DOSTEPNI KELNERZY ---" << endl;
                cout << "1. Kasia (Studentka) [Koszt: 250 zl] Pensja 30/h, Meczy sie po 3h, Brudzi -4" << endl;
                cout << "2. Marek (Zawodowiec) [Koszt: 850 zl] Pensja 25/h, Meczy sie po 6h, Brudzi -2" << endl;
                cout << "3. Artur (Mistrz Sali) [Koszt: 1300 zl] Pensja 100/h, Meczy sie po 8h, Brudzi 0" << endl;
                cout << "Wybierz kandydata: ";
                int pod_wybor;
                cin >> pod_wybor;

                if (pod_wybor == 1)
                {
                    budzet -= 250;
                    zatrudnij(make_unique<Kelner>("Kasia", 30.0, 3, -4));
                }
                else if (pod_wybor == 2)
                {
                    budzet -= 850;
                    zatrudnij(make_unique<Kelner>("Marek", 45.0, 6, -2));
                }
                else if (pod_wybor == 3)
                {
                    budzet -= 1300;
                    zatrudnij(make_unique<Kelner>("Artur", 100.0, 8, 0));
                }
            }
            else if (wybor == 3)
            {
                cout << "\n--- DOSTEPNI SPRZATACZE ---" << endl;
                cout << "1. Pan Mietek (+15 czystosci | Koszt: 90 zl/dzien)" << endl;
                cout << "2. Pani Jadzia (+40 czystosci | Koszt: 140 zl/dzien)" << endl;
                cout << "3. Firma BLYSK (+80 czystosci | Koszt: 200 zl/dzien)" << endl;
                cout << "4. Firma BLYSK 2.0 (+100 czystosci | Koszt: 800 zl/dzien)" << endl;
                cout << "Wybierz opcje: ";
                int p;
                cin >> p;

                unique_ptr<Pracownik> sprzatacz = nullptr;

                // TUTAJ JEST ZMIANA! Zamiast 'zatrudnij', przypisujemy do zmiennej 'sprzatacz'
                if (p == 1)
                {
                    sprzatacz = make_unique<Osoba_Sprzatajaca>("Mietek", 90.0, 10, 15);
                }
                else if (p == 2)
                {
                    sprzatacz = make_unique<Osoba_Sprzatajaca>("Jadzia", 140.0, 10, 40);
                }
                else if (p == 3)
                {
                    sprzatacz = make_unique<Osoba_Sprzatajaca>("Firma BLYSK", 200.0, 10, 80);
                }
                else if (p == 4)
                {
                    sprzatacz = make_unique<Osoba_Sprzatajaca>("Firma BLYSK 2.0", 800.0, 10, 100);
                }

                if (sprzatacz != nullptr)
                {
                    double koszt = sprzatacz->pobierz_wynagrodzenie();
                    if (budzet >= koszt)
                    {
                        budzet -= koszt;
                        sprzatacz->pracuj();
                        poziom_czystosci += sprzatacz->wplyw_na_czystosc();

                        if (poziom_czystosci > 100)
                            poziom_czystosci = 100;
                        cout << " -> Aktualna czystosc wynosi: " << poziom_czystosci << "%" << endl;
                    }
                    else
                    {
                        cout << " -> Brak srodkow na te usluge!" << endl;
                    }
                }
            }
            else if (wybor == 4)
            {
                wyswietl_pracownikow();
                cout << "Podaj numer pracownika do awansu: ";
                int nr;
                cin >> nr;
                wyslij_na_szkolenie(nr);
            }
            else if (wybor == 5)
            {
                wyswietl_pracownikow();
            }
            else if (wybor == 6)
            {
                wyswietl_pracownikow();
                cout << "Podaj numer do zwolnienia: ";
                int nr;
                cin >> nr;
                zwolnij_pracownika(nr);
            }
            else if (wybor == 7)
            {
                if (!dzien_pracujacy())
                    break;
                obecny_dzien++;
            }
            else if (wybor == 0)
                break;

            if (poziom_prestizu >= 5 && budzet >= 10000)
            {
                cout << "\n!!! GRATULACJE !!! Twoja restauracja stala sie legenda rynku!" << endl;
                break;
            }
        }
        cout << "\nKoniec symulacji. Finalny budzet: " << budzet << endl;
    }

    void wyswietl_pracownikow()
    {
        cout << "\n--- LISTA ZALOGI ---" << endl;
        if (pracownicy.empty())
            cout << "[Brak zatrudnionych pracownikow]" << endl;

        for (int i = 0; i < pracownicy.size(); i++)
        {
            string rola = "Pracownik";
            if (dynamic_cast<Kucharz *>(pracownicy[i].get()))
                rola = "Kucharz";
            else if (dynamic_cast<Kelner *>(pracownicy[i].get()))
                rola = "Kelner";

            cout << i << ". [" << rola << "] - Lvl " << pracownicy[i]->get_poziom() << endl;
        }
    }

    void zwolnij_pracownika(int indeks)
    {
        if (indeks < 0 || indeks >= pracownicy.size())
        {
            cout << "Bledny numer pracownika!" << endl;
            return;
        }
        int liczba_kucharzy = 0, liczba_kelnerow = 0;
        for (auto &p : pracownicy)
        {
            if (dynamic_cast<Kucharz *>(p.get()))
                liczba_kucharzy++;
            else if (dynamic_cast<Kelner *>(p.get()))
                liczba_kelnerow++;
        }
        Pracownik *zwalniany = pracownicy[indeks].get();
        if (dynamic_cast<Kucharz *>(zwalniany) && liczba_kucharzy <= 1)
        {
            cout << "Blad! Musisz miec chociaz 1 kucharza!" << endl;
            return;
        }
        if (dynamic_cast<Kelner *>(zwalniany) && liczba_kelnerow <= 1)
        {
            cout << "Blad! Musisz miec chociaz 1 kelnera!" << endl;
            return;
        }
        cout << "Pracownik zostal zwolniony!" << endl;
        pracownicy.erase(pracownicy.begin() + indeks);
    }

    void zdarzenie_losowe()
    {
        int los = rand() % 100; // Losujemy od 0 do 99

        if (los >= 0 && los <= 1)
        {
            cout << "!!! [ZDARZENIE] Kontrola SANEPIDU !!!" << endl;
            if (poziom_czystosci < 50)
            {
                cout << " -> Syf w kuchni! Mandat 300 zl i tracisz prestiz [-20]." << endl;
                budzet -= 300;
                dzienny_bilans_losowy -= 300;
                zmien_prestiz(-20);
            }
            else
            {
                cout << " -> Czysto! Sanepid gratuluje. Prestiz w gore [+10]!" << endl;
                prestiz += 10;
            }
        }
        else if (los >= 2 && los <= 3)
        {
            cout << "!!! [ZDARZENIE] Student rozlal zupe! [-30 czystosc] !!!" << endl;
            poziom_czystosci -= 30;
        }
        else if (los >= 4 && los <= 5)
        {
            cout << "!!! [ZDARZENIE] Bogaty biznesmen daje napiwek! [+250 zl do budzetu] !!!" << endl;
            budzet += 250;
            dzienny_bilans_losowy += 250;
        }
        else if (los >= 6 && los <= 7)
        {
            cout << "!!! [ZDARZENIE] Stluczone talerze! [-50 zl do budzetu] !!!" << endl;
            budzet -= 50;
            dzienny_bilans_losowy -= 50;
        }
        else if (los >= 8 && los <= 9)
        {
            cout << "!!! [ZDARZENIE] Tajny Krytyk Kulinarny! !!!" << endl;
            if (poziom_czystosci > 80 && !gotowe_zestawy.empty())
            {
                cout << " -> Recenzja 5 gwiazdek! [+40 do prestizu]" << endl;
                prestiz += 40;
            }
            else
            {
                cout << " -> Fatalna recenzja. [-30 do prestizu]" << endl;
                zmien_prestiz(-30);
            }
        }
        else if (los >= 10 && los <= 11)
        {
            cout << "!!! [ZDARZENIE] Awaria pradu! Tracisz zapasy. [-150 zl do budzetu] !!!" << endl;
            budzet -= 150;
            dzienny_bilans_losowy -= 150;
        }
        else if (los >= 12 && los <= 13)
        {
            cout << "!!! [ZDARZENIE] Znany influencer wrzucil zdjecie! [+25 do prestizu] !!!" << endl;
            prestiz += 25;
        }
        else if (los >= 14 && los <= 15)
        {
            cout << "!!! [ZDARZENIE] Hurtownia dala rabat! [+100 zl do budzetu] !!!" << endl;
            budzet += 100;
            dzienny_bilans_losowy += 100;
        }
        else if (los >= 16 && los <= 17)
        {
            cout << "!!! [ZDARZENIE] Zlodziej ukradl pieniadze! [-200 zl do budzetu] !!!" << endl;
            budzet -= 200;
            dzienny_bilans_losowy -= 200;
        }
        else if (los >= 18 && los <= 19)
        {
            cout << "!!! [ZDARZENIE] Festiwal Jedzenia - utarg w gore! [+400 zl do budzetu] !!!" << endl;
            budzet += 400;
            dzienny_bilans_losowy += 400;
        }
        else if (los >= 20 && los <= 21)
        {
            cout << "!!! [ZDARZENIE] Pekla rura w lazience! [-100 zl, -20 czystosc] !!!" << endl;
            budzet -= 100;
            dzienny_bilans_losowy -= 100;
            poziom_czystosci -= 20;
        }
        else if (los >= 22 && los <= 23)
        {
            cout << "!!! [ZDARZENIE] Autobus turystow! [+300 zl, -15 czystosc] !!!" << endl;
            budzet += 300;
            dzienny_bilans_losowy += 300;
            poziom_czystosci -= 15;
        }
        else if (los >= 24 && los <= 25)
        {
            cout << "!!! [ZDARZENIE] Kelner oddal zgubiony portfel! [+50 zl, +10 prestizu] !!!" << endl;
            budzet += 50;
            dzienny_bilans_losowy += 50;
            prestiz += 10;
        }
        else if (los >= 26 && los <= 27)
        {
            cout << "!!! [ZDARZENIE] Zapalil sie kosz na smieci! [-60 zl gasnica, -10 prestizu] !!!" << endl;
            budzet -= 60;
            dzienny_bilans_losowy -= 60;
            zmien_prestiz(-10);
        }
        else if (los >= 28 && los <= 29)
        {
            cout << "!!! [ZDARZENIE] Nagle zamowienie cateringowe! [+350 zl do budzetu] !!!" << endl;
            budzet += 350;
            dzienny_bilans_losowy += 350;
        }
        else if (los >= 30 && los <= 31)
        {
            cout << "!!! [ZDARZENIE] Darmowa reklama w lokalnym radiu! [+20 do prestizu] !!!" << endl;
            prestiz += 20;
        }
        else if (los >= 32 && los <= 33)
        {
            cout << "!!! [ZDARZENIE] Dzieci pomalowaly sciane! [-50 zl farba, -20 czystosc] !!!" << endl;
            budzet -= 50;
            dzienny_bilans_losowy -= 50;
            poziom_czystosci -= 20;
        }
        else if (los >= 34 && los <= 35)
        {
            cout << "!!! [ZDARZENIE] Dostawca warzyw zgubil droge! [-80 zl do budzetu] !!!" << endl;
            budzet -= 80;
            dzienny_bilans_losowy -= 80;
        }
        else if (los >= 36 && los <= 37)
        {
            cout << "!!! [ZDARZENIE] Niespodziewana premia ze stowarzyszenia! [+150 zl do budzetu] !!!" << endl;
            budzet += 150;
            dzienny_bilans_losowy += 150;
        }
        else if (los >= 38 && los <= 39)
        {
            cout << "!!! [ZDARZENIE] Kontrola Urzedu Skarbowego. Drobna kara! [-150 zl do budzetu] !!!" << endl;
            budzet -= 150;
            dzienny_bilans_losowy -= 150;
        }
        else if (los >= 40 && los <= 41)
        {
            cout << "!!! [ZDARZENIE] Przeterminowane produkty zepsuly sie! [-100 zl, -15 czystosc] !!!" << endl;
            budzet -= 100;
            dzienny_bilans_losowy -= 100;
            poziom_czystosci -= 15;
        }
        else if (los >= 42 && los <= 43)
        {
            cout << "!!! [ZDARZENIE] Swieto Miasta - ogromny ruch! [+250 zl, -15 czystosc] !!!" << endl;
            budzet += 250;
            dzienny_bilans_losowy += 250;
            poziom_czystosci -= 15;
        }
        else if (los >= 44 && los <= 45)
        {
            cout << "!!! [ZDARZENIE] Awantura klientow na sali! [-20 prestiz, -20 czystosc] !!!" << endl;
            zmien_prestiz(-20);
            poziom_czystosci -= 20;
        }
        else if (los >= 46 && los <= 47)
        {
            cout << "!!! [ZDARZENIE] Wizyta lokalnego Burmistrza! [+30 prestiz] !!!" << endl;
            prestiz += 30;
        }
        else if (los >= 48 && los <= 49)
        {
            cout << "!!! [ZDARZENIE] Przypalona nowa patelnia! [-70 zl za sprzet] !!!" << endl;
            budzet -= 70;
            dzienny_bilans_losowy -= 70;
        }
        else if (los >= 50 && los <= 51)
        {
            cout << "!!! [ZDARZENIE] Awaria terminala - klienci odeszli! [-100 zl do budzetu] !!!" << endl;
            budzet -= 100;
            dzienny_bilans_losowy -= 100;
        }
        else if (los >= 52 && los <= 53)
        {
            cout << "!!! [ZDARZENIE] Zepsuta klimatyzacja (goraco!) [-150 zl naprawa, -10 prestiz] !!!" << endl;
            budzet -= 150;
            dzienny_bilans_losowy -= 150;
            zmien_prestiz(-10);
        }
        else if (los >= 54 && los <= 55)
        {
            cout << "!!! [ZDARZENIE] Wspaniala pogoda - ogródek pelen ludzi! [+180 zl do budzetu] !!!" << endl;
            budzet += 180;
            dzienny_bilans_losowy += 180;
        }
        else if (los >= 56 && los <= 57)
        {
            cout << "!!! [ZDARZENIE] Pomylka hurtowni - darmowe skrzynki jablek! [+80 zl] !!!" << endl;
            budzet += 80;
            dzienny_bilans_losowy += 80;
        }
        else if (los >= 58 && los <= 59)
        {
            cout << "!!! [ZDARZENIE] Inspektor BHP wlepia mandat za brak apteczki! [-100 zl] !!!" << endl;
            budzet -= 100;
            dzienny_bilans_losowy -= 100;
        }
        else
        {

            cout << "Nic ciekawego sie nie wydarzylo." << endl;
        }
    }
    void pokaz_status()
    {
        cout << "\n================ STATYSTYKI ================" << endl;
        cout << " Budzet: " << budzet << " zl | Prestiz: Lvl " << poziom_prestizu << " (" << prestiz << "/50)" << endl;
        cout << " Pracownicy: " << pracownicy.size() << " | Czystosc sali: " << poziom_czystosci << endl;
        cout << "============================================" << endl;
    }
};