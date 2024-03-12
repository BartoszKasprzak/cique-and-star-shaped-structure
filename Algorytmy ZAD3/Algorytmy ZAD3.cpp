#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

//motyw: AGTAGTTGA

struct point
{
	string oligonukleotyd;
	string nazwa_DNA;
	int start;
};

int czygwiazda(vector<vector<point>> list)
{
	int star = 0;

	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].size() >= 5)
		{
			for (int j = 0; j < list[i].size(); j++)
			{
				cout << list[i][j].nazwa_DNA << list[i][j].oligonukleotyd << list[i][j].start << endl;
			}
			//cout << list[i].size();
			star = 1;
			return star;
		}
	}
	return star;
}

int main()
{
//generowanie instancji
	fstream sekwencje;
	sekwencje.open("sekwencje.fasta", std::ios::in);
	fstream wiarygodnosc;
	wiarygodnosc.open("wiarygodnosc.qual", std::ios::in);

	string zczytanie;
	string tmp;
	vector<string> nazwy;
	vector<string> sek;

	while (!sekwencje.eof())
	{
		getline(sekwencje, zczytanie, '\n');
		if (zczytanie[0] != '>')
		{
			sek.push_back(zczytanie);
		}
		else
		{
			for (int i = 1; i < 15; i++)
			{
				tmp.push_back(zczytanie[i]);
			}
			nazwy.push_back(tmp);
			tmp = "";
		}
	}
	
	string zczytaniew;
	string zapis;
	int liczba; 
	vector<vector<int>> wiarygodnosci;
	vector<int> pomocniczy;
	while (!wiarygodnosc.eof())
	{
		getline(wiarygodnosc, zczytaniew, '\n');
		stringstream tekst(zczytaniew);
		if (zczytaniew[0] != '>')
		{
			while (getline(tekst, zapis, ' '))
			{
				liczba = stoi(zapis);
				cout << liczba << " ";
				pomocniczy.push_back(liczba);
			}
			wiarygodnosci.push_back(pomocniczy);
			cout << endl;
		}
		pomocniczy.clear();
	}
	

// usuwanie 
	point Point;
	cout << "Podaj prog wiarygodnosci: ";
	int prog;
	cin >> prog;
	//cout << prog << endl;

	vector<vector<char>> pousuwaniu;
	vector<vector<int>> idusuniete;

	for (int i = 0; i < 5; i++)
	{
		vector<char> sekpomoc;
		vector<int> idpomoc;
		for (int j = 0; j < wiarygodnosci[i].size(); j++)
		{
			if (wiarygodnosci[i][j] > prog)
			{
				idpomoc.push_back(j + 1);
				sekpomoc.push_back(sek[i][j]);
			}
		}
		pousuwaniu.push_back(sekpomoc);
		idusuniete.push_back(idpomoc);
	}

// TWORZENIE GRAFU
	//wierzcholek zapisany jest w 3 vectorach: nazwa w vectorze z nazwami, oligonukleotydy z tej sekwencji w vectorze dwuwymiarowym z oligonukleotydami, a  poczatek w vectorze dwuwymiarowym z poczatkami
	//polaczenie miedzy tymi 3 wektorami to pierwsza wartosc wektorow dwuwymiarowych oraz wartosc przypisana nazwie. tj. sekwencja o nazwie przy indeksie 2 ma oligo i pocz w vectorach przy indeksie 2 w wek dwuwymiar

	int podciag;
	cout << "Podaj dlugosc podciagu: ";
	cin >> podciag;

	vector<vector<int>> poczatek;
	vector<int> pomocpocz;
	vector<vector<string>> oligo;
	vector<string> pomocoligo;
	string tmpoligo;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < pousuwaniu[i].size() - podciag + 1; j++)
		{
			for (int z = j; z < j + podciag; z++)
			{
				
				tmpoligo.push_back(pousuwaniu[i][z]);
				
			}
			pomocoligo.push_back(tmpoligo);
			tmpoligo = "";
			pomocpocz.push_back(idusuniete[i][j]);
		}
		oligo.push_back(pomocoligo);
		pomocoligo.clear();
		poczatek.push_back(pomocpocz);
		pomocpocz.clear();
	}


	vector<vector<point>> graf;
	vector<point> pomocgraf;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < oligo[i].size(); j++)
		{
			Point.nazwa_DNA = nazwy[i];
			Point.oligonukleotyd = oligo[i][j];
			Point.start = poczatek[i][j];
			pomocgraf.push_back(Point);
		}
		graf.push_back(pomocgraf);
		pomocgraf.clear();
	}



// robienie polaczen miedyz wierzcholkami

	int roznica = 0; 
	vector<vector<point>> lista_sasiedztwa;
	vector<point> pomoclista;
	for (int i = 0; i < 5; i++)
	{
		for (int z = 0; z < graf[i].size(); z++)
		{
			pomoclista.push_back(graf[i][z]);

			for (int j = 0; j < 5; j++)
			{
				for (int x = 0; x < graf[j].size(); x++)
				{
					if (i != j)
					{
						roznica = graf[i][z].start - graf[j][x].start;
						if (roznica < 0)
						{
							roznica = roznica * -1;
						}
						if (graf[i][z].oligonukleotyd == graf[j][x].oligonukleotyd && roznica <= (podciag * 10))
						{
							pomoclista.push_back(graf[j][x]);
						}
					}
				}
			}
			if (pomoclista.size() > 1)
			{
				lista_sasiedztwa.push_back(pomoclista);
			}
			pomoclista.clear();
		}
	}


// ZNALEZIENEI GWIAZDY 

	
	int zapamietaj = 0;
	
	if (czygwiazda(lista_sasiedztwa) == 0)
	{
		cout << "GRAF NIE POSIADA GWIAZDY " << endl;
	}
	else
	{
		cout << "SZUKAM GWIAZDY" << endl;
		for (int i = 0; i < lista_sasiedztwa.size(); i++)
		{
			int skw1 = 0;
			int skw2 = 0;
			int skw3 = 0;
			int skw4 = 0;
			int skw5 = 0;
			if (lista_sasiedztwa[i].size() > 4)
			{
				
				for (int j = 0; j < lista_sasiedztwa[i].size(); j++)
				{
					
					if (lista_sasiedztwa[i][j].nazwa_DNA == nazwy[0])
					{
						skw1++;
					}
					else if (lista_sasiedztwa[i][j].nazwa_DNA == nazwy[1])
					{
						skw2++;
					}
					else if (lista_sasiedztwa[i][j].nazwa_DNA == nazwy[2])
					{
						skw3++;
					}
					else if (lista_sasiedztwa[i][j].nazwa_DNA == nazwy[3])
					{
						skw4++;
					}
					else if (lista_sasiedztwa[i][j].nazwa_DNA == nazwy[4])
					{
						skw5++;
					}

					cout << skw1 << skw2 << skw3 << skw4 << skw5 << endl;
					if ((skw1 == 1) && (skw2 == 1) && (skw3 == 1) && (skw4 == 1) && (skw5 == 1))
					{
						zapamietaj = i;
						cout << zapamietaj << endl;
						break;
					}
				}
			}
			if (lista_sasiedztwa[zapamietaj].size() == 5)
			{
				break;
			}
		}

		if (lista_sasiedztwa[zapamietaj].size() < 5)
		{
			cout << "NIEZNALEZIONO GWIAZDY" << endl;
		}
		else
		{
			cout << "Znaleziona gwiazda: " << endl;
			for (int i = 0; i < lista_sasiedztwa[zapamietaj].size(); i++)
			{
				cout << "Sekwencja DNA: " << lista_sasiedztwa[zapamietaj][i].nazwa_DNA << endl;
				cout << "Oligonukleotyd: " << lista_sasiedztwa[zapamietaj][i].oligonukleotyd << endl;
				cout << "Pozycja poczatkowa olionukleotydu: " << lista_sasiedztwa[zapamietaj][i].start << endl;
				cout << endl;
			}
		}
	}
}

