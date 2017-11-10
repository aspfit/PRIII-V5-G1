#include<iostream>
using namespace std;

template<class T, int max>
class Kolekcija{
	T* _elementi;
	int _trenutnoElemenata;
public:
	Kolekcija():_trenutnoElemenata(0), _elementi(nullptr) {};
	~Kolekcija() {
		delete[] _elementi;
		_elementi = nullptr;
	}
};

class Prisustvo{
	char* _brojIndeksa;
	int _brojSati;
public:
	Prisustvo(char* brojIndeksa="N/A", int brojSati=0) : _brojSati(brojSati)
	{
		int size = strlen(brojIndeksa) + 1;
		_brojIndeksa = new char[size];
		strcpy_s(_brojIndeksa, size, brojIndeksa);
	}
	Prisustvo(Prisustvo& nekoPrisustvo) {
		int size = strlen(nekoPrisustvo._brojIndeksa) + 1;
		_brojIndeksa = new char[size];
		strcpy_s(_brojIndeksa, size, nekoPrisustvo._brojIndeksa);
		_brojSati = nekoPrisustvo._brojSati;
	}
	~Prisustvo()
	{
		delete[] _brojIndeksa;
		_brojIndeksa = nullptr;
	}
	Prisustvo& operator+=(int broj) {
		if (broj >= 0)
			_brojSati += broj;
		return *this;
	}
	Prisustvo& operator=(Prisustvo& nekoPrisustvo) {
		int size = strlen(nekoPrisustvo._brojIndeksa) + 1;
		_brojIndeksa = new char[size];
		strcpy_s(_brojIndeksa, size, nekoPrisustvo._brojIndeksa);
		_brojSati = nekoPrisustvo._brojSati;
		return *this;
	}
	char * getIndeks() {
		return _brojIndeksa;
	}
	int getSati() {
		return _brojSati;
	}
	void setSati(int sati) {
		_brojSati = sati;
	}
	bool indeksManji(char * nekiIndeks) {
		int drugiIndeks = atoi(nekiIndeks + 2);
		int prviIndeks = atoi(_brojIndeksa + 2);
		if (prviIndeks < drugiIndeks)
			return true;
		return false;
	}
	void info() {
		cout << _brojIndeksa << " " << _brojSati << endl;
	}
	friend Prisustvo operator+(Prisustvo& nekoPrisustvo, int broj) {
		Prisustvo novo = nekoPrisustvo;
		if (broj >= 0)
			novo._brojSati += broj;
		return novo;
	}
};

class Nastava{
	char _datum[12];
	char _tipNastave; 
	int _odrzanoSati;
	Prisustvo* _prisutni;
	int _brojStudenata;
public:
	Nastava(char datum[], char tipNastave, int odrzanoSati):_prisutni(nullptr), _brojStudenata(0) {
		strncpy_s(_datum, datum, _TRUNCATE);
		_tipNastave = tipNastave;
		_odrzanoSati = odrzanoSati;
	}
	~Nastava() {
		delete[] _prisutni;
		_prisutni = nullptr;
	}
	void info() {
		cout << _datum << endl;
		cout << _tipNastave << endl;
		cout << _odrzanoSati << endl;
		for (int i = 0; i < _brojStudenata; i++)
			_prisutni[i].info();
		cout << endl;
	}
	Nastava& operator+=(Prisustvo& nekoPrisustvo) {
		for (int i = 0; i < _brojStudenata; i++)
			if (strcmp(_prisutni[i].getIndeks(), nekoPrisustvo.getIndeks()) == 0)
				return *this;
		Prisustvo * temp = new Prisustvo[_brojStudenata + 1];
		for (int i = 0; i < _brojStudenata; i++)
			temp[i] = _prisutni[i];
		delete[] _prisutni; _prisutni = temp; temp = nullptr;
		_prisutni[_brojStudenata++] = nekoPrisustvo;
		bool pronadjen = true;
		while (pronadjen) {
			pronadjen = false;
			for (int i = 0; i < _brojStudenata-1; i++) {
				if (_prisutni[_brojStudenata - 1].indeksManji(_prisutni[i].getIndeks())) {
					swap(_prisutni[_brojStudenata - 1], _prisutni[i]);
					pronadjen = true;
				}
			}
		}
		return *this;
	}
	Prisustvo& operator[](char * brojIndeksa) {
		for (int i = 0; i < _brojStudenata; i++) {
			if (strcmp(_prisutni[i].getIndeks(), brojIndeksa) == 0)
				return _prisutni[i];
		}
		return Prisustvo();
	}
	Nastava& operator()(char * brojIndeksa, int brojSati) {
		for (int i = 0; i < _brojStudenata; i++) {
			if (strcmp(_prisutni[i].getIndeks(), brojIndeksa) == 0) {
				_prisutni[i] += brojSati;
				if (_prisutni[i].getSati() > _odrzanoSati)
					_prisutni[i].setSati(_odrzanoSati);
			}
		}
	}
	friend Nastava operator+(Nastava& prvaNastava, Nastava& drugaNastava);
};
Nastava operator+(Nastava& prvaNastava, Nastava& drugaNastava) {
	if (&prvaNastava != &drugaNastava && (prvaNastava._tipNastave != drugaNastava._tipNastave)) {
		Nastava novaNastava = prvaNastava;
		bool neispunjava = false;
		for (int i = 0; i < drugaNastava._brojStudenata; i++) {
			neispunjava = false;
			for (int j = 0; j < prvaNastava._brojStudenata; j++) {
				if (strcmp(prvaNastava._prisutni[j].getIndeks, drugaNastava._prisutni[j].getIndeks()) == 0)
					neispunjava = true;
			}
			if(!neispunjava)
				prvaNastava += drugaNastava._prisutni[i];
		}
	}
}

class Predmet
{
	char* _naziv;
	int _godinaStudija;
	Kolekcija<Nastava, 90> _odrzanaNastava;
public:
	Predmet(char* naziv, int godinaStudija) : _godinaStudija(godinaStudija){
		int size = strlen(naziv) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, naziv);
	}
	~Predmet(){
		delete[] _naziv;
		_naziv = nullptr;
	}
	void evidentirajNastavu() {

	}

	//Funkciju za evidenciju održane nastave na predmetu.

	/*Funkciju koja na osnovu datuma i tipa održane nastave dodaje prisustvo studentu sa proslijeđenim brojem indeksa i brojem sati.
	Voditi računa da broj sati prisustva ne može biti veći od broja sati održane nastave.*/

	/*Funkciju koja na osnovu datuma i tipa održane nastave uklanja prisustvo studentu sa proslijeđenim brojem indeksa.*/

	/*Funkciju koja na osnovu broja indeksa studenta vraća sve podatke o njegovom prisustvu na nastavi*/

	/*Funkciju koja na osnovu tipa nastave (P ili V) vrši ispis prisustva za sve studente (sumarno) u datom formatu:
	IB140001 30/40 75%, gdje je prva kolona "broj indeksa", druga kolona "broj sati prisustva / ukupan broj sati održane nastave" i
	treca kolona "procenat prisustva studenta na nastavi".*/

	friend ostream& operator<<(ostream&, const Predmet&);
};
ostream& operator<<(ostream& cout, const Predmet& p)
{
	cout << "Predmet: " << p._naziv << endl;
	cout << "Godina studija: " << p._godinaStudija << endl;
	return cout;
}

int main(){
	Prisustvo alem("IB160042", 3);
	Prisustvo bakir("IB160016", 3);
	Prisustvo anes("IB160043", 3);
	Nastava danas("10.11.2017", 'v', 3);
	danas += alem;
	danas += bakir;
	danas += anes;
	danas.info();
	getchar();
	return 0;
}
