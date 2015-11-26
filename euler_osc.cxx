#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <cstdlib>

using namespace std;

void init(double* const x);													//Startwerte der DGL
void print(const double* const x, const string namedat, const double t);	//Ausgabe
void forward(double* const x, const double t);								//explizites Eulerverfahren
void backward(double* const x, const double t);								//implizites Eulerverfahren
void plot(const string befehle, const string nameplot, const string nameex, const string nameim);						//Ausgabe in Gnuplot

int main(void)
{
	const double TStart=0.,TEnde=20*M_PI;			//Startwerte
	const int nenner=100;							//Nenner zur Bestimmung der Schrittweite
	const double DeltaT=M_PI/nenner;				//Schrittweite berechnen
	const int N=(TEnde-TStart)/DeltaT;				//Anzahl der Iterationen
	const string ex="explizites";
	const string im="implizites";
	stringstream nameex,nameim,nameplot,befehl;
	nameex.str("");	
	nameex<<ex<<"_"<<nenner<<".txt";				//String zusammensetzen für explizites Verfahren in Abhängigkeit von der Schrittweite
	nameim.str("");
	nameim<<im<<"_"<<nenner<<".txt";				//String zusammensetzen für implizites Verfahren in Abhängigkeit von der Schrittweite
	befehl.str("");
	befehl<<"befehle_"<<nenner<<".txt";				//String zusammensetzen für Gnuplot in Abhängigkeit von der Schrittweite
	nameplot.str("");
	nameplot<<"Oszillator_"<<nenner<<".jpeg";		//String zusammensetzen für Bilddatei in Abhängigkeit von der Schrittweite
	
	double* odeex=new double[2];					//dynamisches Array für Daten
	double* odeim=new double[2];
	init(odeex);									//Differentialgleichung initialisieren
	init(odeim);
	for (int i=1;i<=N;i++) 
	{
		forward(odeex,DeltaT);						//Verfahren aufrufen
		backward(odeim,DeltaT);
		
		print(odeex,nameex.str(),i*DeltaT);			//Ergebnisse in Datei schreiben
		print(odeim,nameim.str(),i*DeltaT);
	}
	plot(befehl.str(),nameplot.str(),nameex.str(),nameim.str()); //Plot erstellen 
	
	delete[] odeex;									//dynamischen Speicher freigeben
	delete[] odeim;
	return 0;
}

void init(double* const x)
{
	x[0]=1.;
	x[1]=0.;
}													
void print(const double* const x, const string namedat, const double t)
{
	fstream out(namedat.c_str(),ios_base::out | ios_base::app);			//Öffne Datei für Lösungsdaten, bei bestehender Datei werden die Daten nur angehängt und nicht üvberschrieben
	out<<t<<"\t"<<x[0]<<endl;											//Ausgabe für Gnuplot: Zeit gegen Lösung
	out.close();														//Datei schließen
}											
void forward(double* const x,const double t)
{
	double temp=x[0];		//Zwischenspeichern, um nicht den neuen Wert zu verwenden
	x[0]+=x[1]*t;
	x[1]-=temp*t;
}								
void backward(double* const x, const double t)
{
	double temp=x[0];		//Zwischenspeichern, um nicht den neuen Wert zu verwenden
	x[0]+=x[1]*t;
	x[0]/=(1+t*t);
	x[1]-=temp*t;
	x[1]/=(1+t*t);
}											
void plot(const string befehle, const string nameplot, const string nameex, const string nameim)
{
	fstream plot(befehle.c_str(),ios_base::out);					//Befehlsdatei öffnen
	
	plot<<"set terminal jpeg"<<endl;								//Bildausgabe auf jpeg setzen
	plot<<"set output \""<<nameplot<<"\""<<endl;					//Dateinamen für Bild geben
	plot<<"set st data line"<<endl;									//Plot mit Linien
	plot<<"plot \""<<nameex<<"\" t \"explizit\", \""<<nameim<<"\" t \"implizit\", cos(x) t \"analytisch\""<<endl;	//explizit gegen implizit gegen analytisch
	plot<<"unset output"<<endl;
	plot.close();
	
	string befehl="gnuplot ";
	befehl.append(befehle);											//Systembefehl für Gnuplot erstellen (Eingabe wie in Konsole)
	system(befehl.c_str());	
}