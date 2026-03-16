#pragma once
#include <iostream>
#include <string>

using namespace std;
class Pracownik
{
protected:
    string imie;
    int poziom_doswiadczenia;
    int poziom_zmeczenia;
    double wynagrodzenie_za_godzine_pracy;
    int liczba_przepracowanych_godzin;
    int pozostaly_czas_odpoczynku;
    double wynagrodzenie_koncowe;
    int poziom_wyczerpania;
    int czystosc;

    void odpocznij()
    {
       cout << " [Zzz] " << imie << " pije kawe na zapleczu i odzyskuje sily..." << endl;
        poziom_zmeczenia -= 3;
        if (poziom_zmeczenia < 0) {
            poziom_zmeczenia = 0; 
        }
    }
   

    void przepracuj_godzine()
    {
        liczba_przepracowanych_godzin++;
        poziom_zmeczenia++;
        
    }

public:
    virtual ~Pracownik() {} 
    int get_poziom() const { return poziom_doswiadczenia; }
    Pracownik(string im, double startowa_pensja, int wyczerpanie, int wplyw_na_czystosc)
    {
        imie = im;
        wynagrodzenie_za_godzine_pracy = startowa_pensja;
        poziom_doswiadczenia = 1;
        poziom_zmeczenia = 0;
        liczba_przepracowanych_godzin = 0;
        pozostaly_czas_odpoczynku = 0;
        wynagrodzenie_koncowe = 0;
        poziom_wyczerpania = wyczerpanie;
        czystosc = wplyw_na_czystosc;
    }

    virtual bool pracuj() = 0;
    virtual double pobierz_wynagrodzenie() = 0;
    virtual int wplyw_na_czystosc() = 0;
    //virtual double generuj_zysk() = 0;
    void nastepny_dzien()
    {
        poziom_zmeczenia = 0;
        liczba_przepracowanych_godzin = 0;
        pozostaly_czas_odpoczynku = 0;
    }

     void awansuj()
    {
        poziom_doswiadczenia++;
        wynagrodzenie_za_godzine_pracy += 5.0;
        poziom_wyczerpania += 2;
        if (czystosc < 0) {
            czystosc += 1; 
        }
         cout << imie << " awansowal! Poziom doswiadczenia: " << poziom_doswiadczenia << ", wynagrodzenie za godzine pracy: " << wynagrodzenie_za_godzine_pracy << " zl, poziom wyczerpania: " << poziom_wyczerpania << ", wplyw na czystosc: " << czystosc << endl;
    }
};

class Kucharz : public Pracownik
{
public:
    Kucharz(string im, double startowa_pensja, int wyczerpanie, int czystosc) : Pracownik(im, startowa_pensja, wyczerpanie, czystosc) {}

    bool pracuj() override
    {
        if (poziom_zmeczenia < poziom_wyczerpania)
        {
            cout << imie << " przygotowuje posilek." << endl;
            przepracuj_godzine();
            return true;
        }
        else
        {
            cout << imie << " jest zbyt zmeczony, aby pracowac." << endl;
            pozostaly_czas_odpoczynku = poziom_wyczerpania;
            odpocznij();
            return false;
        }
    }

    double pobierz_wynagrodzenie() override
    {
        wynagrodzenie_koncowe = wynagrodzenie_za_godzine_pracy * liczba_przepracowanych_godzin;
        cout << imie << " otrzymuje wynagrodzenie za godzine_pracy: " << wynagrodzenie_koncowe << " zl." << endl;
        return wynagrodzenie_koncowe;
    }
    int wplyw_na_czystosc() override
    {
        return czystosc;
    }
};

class Kelner : public Pracownik
{
public:
    Kelner(string im, double startowa_pensja, int wyczerpanie, int czystosc) : Pracownik(im, startowa_pensja, wyczerpanie, czystosc) {}

    bool pracuj() override
    {
        if (poziom_zmeczenia < poziom_wyczerpania)
        {
            cout << imie << " obsluguje klientow." << endl;
            przepracuj_godzine();
            return true;
        }
        else
        {
            cout << imie << " jest zbyt zmeczony, aby pracowac." << endl;
            pozostaly_czas_odpoczynku = poziom_wyczerpania;
            odpocznij();
            return false;
        }
    }

    double pobierz_wynagrodzenie() override
    {
        wynagrodzenie_koncowe = wynagrodzenie_za_godzine_pracy * liczba_przepracowanych_godzin;
        cout << imie << " otrzymuje wynagrodzenie za godzine_pracy: " << wynagrodzenie_koncowe << " zl." << endl;
        return wynagrodzenie_koncowe;
    }

    int wplyw_na_czystosc() override
    {
        return czystosc;
    }
};

class Osoba_Sprzatajaca : public Pracownik {
public:
    Osoba_Sprzatajaca(string im, double koszt_uslugi, int wyczerpanie, int czystosc) : Pracownik(im, koszt_uslugi, wyczerpanie, czystosc) {}
    
    bool pracuj() override {
        cout << " -> " << imie << " wkracza z mopem i blyskawicznie sprzata sale!" << endl;
        return true;
    }
    
    double pobierz_wynagrodzenie() override {
        
        return wynagrodzenie_za_godzine_pracy; 
    }
    
    int wplyw_na_czystosc() override { return czystosc; }

};