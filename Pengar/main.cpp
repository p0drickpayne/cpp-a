//
//  main.cpp
//  Pengar
//
//  Created by Jakob Salomonsson on 2020-12-18.
//

#include <iostream>
#include <fstream>

using namespace std;

const int MAX_KOMPISAR = 30;
const int MAX_TRANSAKTIONER = 30;
const int MAX_PERSONER = 30;

// KLASSDEKLARATION
// KPERSON
class Person
{
private:
    string namn;
    double betalat_andras;   // ligger ute med totalt
    double skyldig;          // skyldig totalt
    
public:
    Person();
    Person( string n, double b, double s );
    string haemta_namn();
    double haemta_betalat();
    double haemta_skyldig();
    void skrivUt();
    //...eventuellt div. annat...
};
// KPERSON

// KPLISTA
class PersonLista
{
private:
    int antal_pers;
    Person pers[MAX_PERSONER];
    
public:
    PersonLista();
    //~PersonLista();
    void laggTillEn( Person pny );
    void skrivUtOchFixa();
    double summaSkyldig();
    double summaBetalat();
    bool finnsPerson(const string& namn);
    //...eventuellt div. annat...
};
// KPLISTA

// KTRANS
class Transaktion
{
private:
    string datum;
    string typ;
    string namn;
    double belopp;
    int ant_kompisar;
    string kompisar[MAX_KOMPISAR];
    
public:
    //Transaktion();
    //~Transaktion();
    string haemta_namn();
    double haemta_belopp();
    int haemta_ant_kompisar();
    bool finnsKompis( string namnet );
    bool laesEnTrans( istream &is );
    void skrivEnTrans( ostream &os );
    // ...eventuellt div. annat...
};
// KTRANS

// KTLISTA
class TransaktionsLista
{
private:
    Transaktion trans[MAX_TRANSAKTIONER];
    int antalTrans;
    
public:
    //TransaktionsLista();
    //~TransaktionsLista();
    void laesin( istream & is );
    void skrivut( ostream & os );
    void laggTill( Transaktion & t );
    double totalkostnad();
    double liggerUteMed( string namnet );
    double aerSkyldig( string namnet );
    PersonLista FixaPersoner();
    //...eventuellt div. annat...
};
// KTLISTA
// KLASSDEKLARATION

// MAIN
void meny(TransaktionsLista t, PersonLista p) {
    ofstream utfilen;
    
    bool run = 1;
    char val;
    
    string namnet;
    double a;
    double b;
    double c;
    while (run) {
        cout << "Meny " << endl;
        cout << "Gör ett val: " << endl;
        cout << "0: Avsluta" << endl;
        cout << "1: Läs in en transaktion från tangentbordet." << endl;
        cout << "2: Skriv ut information om alla transaktioner." << endl;
        cout << "3: Beräkna totala kostnaden." << endl;
        cout << "4: Hur mycket är en viss person skyldig?" << endl;
        cout << "5: Hur mycket ligger en viss person ute med?" << endl;
        cout << "6: Lista alla personer mm och FIXA!!!" << endl;
        
        cin >> val;
        if (!isdigit(val)) {
            cout << "Försök igen: " << endl;
        } else{
            switch (val) {
                case '0':
                    cout << "Alla transaktioner sparas på fil. Hejdå!" << endl;
                    utfilen.open("utresa.txt");
                    t.skrivut(utfilen);
                    run = 0;
                    break;
                case '1':
                    t.laesin(cin);
                    p = t.FixaPersoner();
                    break;
                case '2':
                    t.skrivut(cout);
                    break;
                case '3':
                    a = t.totalkostnad();
                    cout << a << " är den totala summan" << endl;
                    break;
                case '4':
                    cout << "Ge namnet: " << endl;
                    cin >> namnet;
                    b = t.aerSkyldig(namnet);
                    cout << b << endl;
                    break;
                case '5':
                    cout << "Ge namnet: " << endl;
                    cin >> namnet;
                    c = t.liggerUteMed(namnet);
                    cout << c << endl;
                    break;
                case '6':
                    p.skrivUtOchFixa();
                    break;
                default:
                    cout << "Försök igen: " << endl;
                    break;
            }
        }
    }
}

int main() {
    TransaktionsLista tlista;
    
    ifstream indata("resa.txt");
    tlista.laesin(indata);
    
    PersonLista plista;
    plista = tlista.FixaPersoner();
    
    meny(tlista, plista);
    return 0;
}
// MAIN

// FUNCS
// FUNCTRANS
string Transaktion::haemta_namn() {
    return namn;
}
double Transaktion::haemta_belopp() {
    return belopp;
}
int Transaktion::haemta_ant_kompisar() {
    return ant_kompisar;
}
bool Transaktion::finnsKompis(string namnet){
    bool kompis_finns = false;
    for (int i=0; i<ant_kompisar; i++) {
        if(namnet == kompisar[i]){
            kompis_finns = true;
        }
    }
    return kompis_finns;
}
void Transaktion::skrivEnTrans( ostream &os ) {
    //Nya transaktioner läggs inte till i Transaktionslistan
    os << datum << '\t';
    os << typ << '\t';
    os << namn << '\t';
    os << belopp << '\t';
    os << ant_kompisar << '\t';
    for (int i=0; i<ant_kompisar; i++) {
        if (i==ant_kompisar-1) {
            os << kompisar[i];
        } else {
            os << kompisar[i] << '\t';
        }
        
    }
    os << '\n';
}
bool Transaktion::laesEnTrans(istream &is){
    string str;
    cout << "Datum: ";
    is >> datum;
    cout << "Typ: ";
    is >> typ;
    cout << "Namn: ";
    is >> namn;
    cout << "Belopp: ";
    is >> belopp;
    cout << " Antal kompisar: ";
    is >> ant_kompisar;
    
    if (ant_kompisar > 0) {
        //cout << ant_kompisar << endl;
        for (int i=0; i<ant_kompisar; i++) {
            is >> kompisar[i];
            cout << "Kompis: " << kompisar[i];
        }
        is.get();
    }
    
    if (&is == &std::cin){
        is.get(); // Tar bort nyrad?
        return is.eof();
    }
    
    return !is.eof();
}
// FUNCTRANS

// FUNCTLISTA
void TransaktionsLista::skrivut( ostream &os ) {
    for (int i = 0; i < antalTrans; i++) {
        trans[i].skrivEnTrans(os);
    }
}
void TransaktionsLista::laesin( istream & is ) {
    Transaktion t;
    
    if (&is == &cin){
        if (!t.laesEnTrans(is)) {
            laggTill(t);
        }
        
        
    } else{
        // Så länge det går bra att läsa (filen ej slut)
        while (t.laesEnTrans(is))
        {
            laggTill(t);
        }
    }
}
void TransaktionsLista::laggTill(Transaktion &t) {
    
    trans[antalTrans] = t;
    
    antalTrans++;
}
double TransaktionsLista::aerSkyldig( string namnet ) {
    double tot = 0;
    bool yes = true;
    
    for (int i = 0; i < antalTrans; i++) {
        yes = trans[i].finnsKompis(namnet);

        if(yes){
            tot += trans[i].haemta_belopp() / (trans[i].haemta_ant_kompisar()+1);
        }
    }
    return tot;
}
double TransaktionsLista::liggerUteMed( string namnet ) {
    string haemtat_namn = "";
    double tot = 0;

    for (int i = 0; i < antalTrans; i++) {
        haemtat_namn = trans[i].haemta_namn();

        if (haemtat_namn == namnet) {
            double kostnad = trans[i].haemta_belopp();
            int antal_som_delar = trans[i].haemta_ant_kompisar() +1;
            double jag_betalat = kostnad/antal_som_delar;
            tot += kostnad - jag_betalat;
        }
    }
    return tot;
}
double TransaktionsLista::totalkostnad() {
    double tot = 0;

    for (int i = 0; i < antalTrans; i++) {
        tot += trans[i].haemta_belopp();
    }

    return tot;
}
PersonLista TransaktionsLista::FixaPersoner(){
    PersonLista plista;
    
    string namn;
    double aer_skyldig;
    double ligger_ute_med;
    
    for (int i = 0; i<antalTrans; i++) {
        cout << trans[i].haemta_namn() << endl;
        
        namn = trans[i].haemta_namn();
        bool personExist = plista.finnsPerson(namn);
        
        if (!personExist) {
            aer_skyldig = aerSkyldig(namn);
            ligger_ute_med = liggerUteMed(namn);
            cout << namn << " Ligger ute med: " << ligger_ute_med << endl;
            Person per(namn, ligger_ute_med, aer_skyldig);
            plista.laggTillEn(per);
        }
    }
    return plista;
}
// FUNCTLISTA

// FUNCPERSON
Person::Person() {
    
}
Person::Person(string n, double ba, double sk) {
    namn = n;
    betalat_andras = ba;
    skyldig = sk;
}
string Person::haemta_namn(){
    return namn;
}
double Person::haemta_betalat(){
    return betalat_andras;
}
double Person::haemta_skyldig(){
    return skyldig;
}
void Person::skrivUt(){
    double skillnad = skyldig - betalat_andras;

    cout << namn << " ligger ute med : " << betalat_andras <<" och är skyldig: " << skyldig;
    if(betalat_andras < skyldig){

      cout << ". Skall betala " << skillnad << " till potten!" << endl;
    } else {
        skillnad = skillnad*-1;
        cout << ". Skall ha " << skillnad <<" från potten!" << endl;
    }
}
// FUNCPERSON

// FUNCPLISTA
PersonLista::PersonLista() {
    antal_pers = 0;
}
void PersonLista::laggTillEn( Person pny ){
    pers[antal_pers] = pny;
    antal_pers++;
}
bool PersonLista::finnsPerson(const string& namn){
    bool personExist = 0;
    string pnamn;
    for (int i = 0; i<antal_pers; i++) {
        pnamn = pers[i].haemta_namn();
        if (namn == pnamn) {
            personExist = 1;
        }
    }
    return personExist;
}
void PersonLista::skrivUtOchFixa(){
    double tot = 0.0;
    
    for (int i = 0; i<antal_pers; i++) {
        
        tot += pers[i].haemta_betalat() - pers[i].haemta_skyldig();
        
        pers[i].skrivUt();
    }
    if (tot == 0) {
        cout << "potten går jämnt ut " << endl;
    }
}
// FUNCPLISTA
// FUNCS
