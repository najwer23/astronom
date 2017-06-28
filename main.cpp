#include <iostream>
#include <time.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

/*
struct tm
{
    int tm_sec; //Sekundy. Zakres [0..61]
    int tm_min; //Minuty. Zakres [0..59]
    int tm_hour; //Godziny. Zakres [0..23]
    int tm_mday; //Dzien miesiaca. Zakres [1..31]
    int tm_mon; //Miesiac. Zakres [0..11]
    int tm_year; //Obecny rok. Lata zaczynaja sie liczyc od roku 1900, czyli: wartosc 0 = 1900 rok.
    int tm_wday; //Dzien tygodnia. Zakres [0..6]. 0 = Niedziela; 1 = Poniedzialek; itd...
    int tm_yday; //Dzien roku. Zakres [0..365].
    int tm_isdst; //Letnie/zimowe przesuniecie czasowe. Jezeli wartosc jest wieksza od 0 to przesuniecie czasowe jest 'aktywne'. Jezli wartosc mniejsza od 0 to informacja jest niedostepna.
}
*/

void aboutProject(struct tm* data)
{
    cout<<" Date:        ";
    //godzina
    cout.width(2);
    cout.fill('0');
    cout<<data->tm_hour<<":";
    //minuty
    cout.width(2);
    cout.fill('0');
    cout<<data->tm_min<<":";
    //sekundy
    cout.width(2);
    cout.fill('0');
    cout<<data->tm_sec<<", ";
    //dzien
    cout<<data->tm_mday<<".";
    //miesiac
    cout.width(2);
    cout.fill('0');
    cout<<data->tm_mon+1<<".";
    //rok
    cout<<data->tm_year+1900<<"r. "<<endl;

    string summer="No"; // zimowy 0
    if(data->tm_isdst)
        summer="Yes"; // letni +1

    cout<<" Author:      Mariusz Najwer"<<endl;
    cout<<" Summer time: "<<summer<<endl;
}

void sun_show(int a, int b)
{
    cout.width(2);
    cout.fill('0');
    cout<<a<<":";
    cout.width(2);
    cout.fill('0');
    cout<<b<<endl;
}

class City
{
public:
    int id;
    string name;
    double latitude; //szerokosc
    double altitude; // dlugosc
    double timeShift;
    City();
    ~City();
    void show();
    void sun_position(struct tm* data);
    bool operator<(const City &a) const
    {
        return name < a.name;
    }
};

City::City()
    : id(0)
    , name("Wroclaw")
    , latitude(51.1)
    , altitude(17.02)
    , timeShift(0)
{
    ;
}

City::~City()
{
    ;
}

void City::show()
{
    cout<<" ";
    cout<< setw(20);
    cout.fill(' ');
    cout<<left<<name<<" Lat: ";
    cout<< setw(7);
    cout<<fixed<<setprecision(2)<<right<<latitude;
    cout<<"  Alt: ";
    cout<< setw(7);
    cout<<fixed<<setprecision(2)<<altitude<<endl;
}

void City::sun_position(struct tm* data)
{
    double n3=M_PI/180;
    double o3=57.29577951;
    double d5=data->tm_year+1900;
    double d6=data->tm_mon+1;
    double d7=data->tm_mday;

    double e6=d6;
    double e7=d5;

    if(d6<=2)
    {
        double e6=d6+12;
        double e7=d5-1;
    }

    double l5=int(d5/100);
    double l6=2-l5+int(l5/4);
    double l7=int(365.25*(e7+4716))+int(30.6001*(e6+1))+d7+l6-1524.5;
    double m3=(l7-2451545)/36525;
    double m4=280.46646+36000.76983*m3+0.0003032*m3*m3;
    double m5=357.52911+35999.05029*m3-0.0001537*m3*m3;
    double n5=m5/360;
    double o5=(n5-int(n5))*360;
    double m6=(1.914602-0.004817*m3-0.000014*m3*m3)*sin(o5*n3);
    double m7=(0.019993-0.000101*m3)*sin(2*o5*n3);
    double m8=0.000289*sin(3*o5*n3);
    double m9=m6+m7+m8;
    double n4=m4/360;
    double o4=(n4-int(n4))*360;
    double n6=o4+m9;
    double n7=125.04-1934.136*m3;
    double n9=n7;

    if(n7<0)
        n9=n7+360;

    double n10=n6-0.00569-0.00478*sin(n9*n3);
    double m11=23.43930278-0.0130042*m3-0.000000163*m3*m3;
    double n11=sin(m11*n3)*sin(n10*n3);
    double n12=asin(n11)*180/M_PI;
    double n15=altitude/15;
    double o15=latitude;
    double m13=(7.7*sin((o4+78)*n3)-9.5*sin(2*o4*n3))/60;
    double o16=cos(n12*n3)*cos(o15*n3);
    double n16=-0.01483-sin(n12*n3)*sin(o15*n3);
    double p15=2*(acos(n16/o16)*o3)/15;

    // WSCHOD
    double p17=13-n15+m13-(p15/2); // wschod slonca liczba
    double sunriseHour=floor(p17); // wschod slonca godziny
    double sunriseMinutes= round((p17-sunriseHour )*60); // wschod slonca minuty

    //zdecydowanie mozna to zrobic lepiej :(
    if(sunriseMinutes>=60)
    {
        sunriseMinutes=0;
        if(sunriseHour<0)
            sunriseHour-=1;
        else
            sunriseHour+=1;
    }

    // GOROWANIE
    double r18=13-n15+m13; // liczba gorowania slonca
    double sunMidHour=floor(r18); // gorowanie slonca godzina
    double sunMidMinutes=round((r18-sunMidHour)*60); // gorowanie slonca minuty

    if(sunMidMinutes>=60)
    {
        sunMidMinutes=0;
        if(sunMidHour<0)
            sunMidHour-=1;
        else
            sunMidHour+=1;
    }

    // ZACHOD
    double q17=13-n15+m13+(p15/2); // liczba zachodu slonca
    double sunsetHour=floor(q17); // godzina zachodu slonca
    double sunsetMinutes=round((q17-sunsetHour)*60); // zachod slonca minuty

    if(sunsetMinutes>=60)
    {
        sunsetMinutes=0;
        if(sunsetHour<0)
            sunsetHour-=1;
        else
            sunsetHour+=1;
    }

    int bonus=0;
    if(data->tm_isdst)
        bonus+=1;

    //WYSWIETLANIE
    cout<<endl<<" "<<name<<endl;
    cout<<" Dzisiejszy wschod Slonca:    ";
    sun_show(sunriseHour+bonus+timeShift,sunriseMinutes);

    cout<<" Dzisiejsze gorowanie Slonca: ";
    sun_show(sunMidHour+bonus+timeShift,sunMidMinutes);

    cout<<" Dzisiejszy zachod Slonca:    ";
    sun_show(sunsetHour+bonus+timeShift,sunsetMinutes);
}

vector<City> load(vector <City>& citys)
{
    City city;
    fstream plik;
    string linia; // linia tekstu
    int nr_linii=1; // liczy linie wczytanego pliku
    int i=0;
    string::size_type sz;

    plik.open("citys.txt", ios::in);
    if(plik.good()==true)
    {
        while (getline(plik,linia))
        {
            switch(nr_linii)
            {
            case 1:
            {
                city.name=linia;
                break;
            }

            case 2:
            {
                city.latitude=stod(linia.c_str(),&sz);
                break;
            }

            case 3:
            {
                city.altitude=stod(linia.c_str(),&sz);
                break;
            }

            case 4:
            {
                city.timeShift=stod(linia.c_str(),&sz);
                break;
            }

            }

            if(nr_linii==4)
            {
                citys.push_back(city);
                nr_linii=0;
                i++;
            }
            nr_linii++;
        }
        return citys;
        plik.close();
    }
    else
    {
        cout<<" Brak dostepu do pliku city.txt :("<<endl;
    }
}

void menu()
{
    cout<<endl<<" Menu"<<endl;
    cout<<" h --- Home (Wroclaw)"<<endl; //Wroclaw
    cout<<" p --- Pokaz Menu"<<endl;
    cout<<" d --- Dostepne miasta"<<endl;
    cout<<" w --- Pozycja Slonca dla wszystkich lokalizacji"<<endl;
    cout<<" k --- Koniec"<<endl<<endl;
    cout<<" --> ";
}

int main()
{
    time_t ticTac;
    time(&ticTac);
    tm* data=localtime(&ticTac);
    aboutProject(data);

    City home;
    vector <City> citys;

    char pytanie='p';    //pytanie do menu
    while(pytanie!='k')
    {
        switch(pytanie)
        {
        case 'p' :
        {
            menu();
            cin>>pytanie;
            break;
        }
        case 'h' :
        {
            home.sun_position(data);
            cout<<endl<<" Co chcesz zrobic?"<<endl;
            cout<<" --> ";
            cin.sync();
            cin>>pytanie;
            break;
        }

        case 'd' :
        {
            cout<<endl;
            citys=load(citys);
            sort(citys.begin(),citys.end()); // sort od A do Z
            for( int i=0; i<citys.size(); i++ )
            {
                citys[i].show();
            }
            citys.clear();
            cout<<endl<<" Co chcesz zrobic?"<<endl;
            cout<<" --> ";
            cin.sync();
            cin>>pytanie;
            break;
        }

        case 'w' :
        {
            citys=load(citys);
            sort(citys.begin(),citys.end()); // sort od A do Z
            for( int i=0; i<citys.size(); i++ )
            {
                citys[i].sun_position(data);
            }
            citys.clear();
            cout<<endl<<" Czasy sa wzgledem Polski"<<endl;

            cout<<endl<<" Co chcesz zrobic?"<<endl;
            cout<<" --> ";
            cin.sync();
            cin>>pytanie;
            break;
        }

        default  :
        {
            cout<<" Bledny znak!! Sprobuj ponownie :)"<<endl;
            cout<<" --> ";
            cin>>pytanie;
            break;
        }
        }
    }
    cout<<" Koniec pracy programu"<<endl<<endl;
    system("pause");
    return 0;

}
