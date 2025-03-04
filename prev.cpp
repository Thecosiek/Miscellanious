#include <iostream>
#include <climits>
#include <vector>
#include <memory>
#include "prev.h"
using namespace std;


struct wierzcholek { //struktura wierzcholkow, z ktorych bedziemy tworzyc drzewa
	int indeks;
	std::shared_ptr<wierzcholek> lewySyn = nullptr;
	std::shared_ptr<wierzcholek> prawySyn = nullptr;
};

std::vector <std::shared_ptr<wierzcholek>> korzenie; //zbior korzeni (wszystkich drzew)

std::shared_ptr<wierzcholek> stworzWierzcholek(int idx) { //tworzenie nowego wierzcholka
	std::shared_ptr<wierzcholek> nowy = std::make_shared<wierzcholek>();
	nowy -> lewySyn = nullptr;
	nowy -> prawySyn = nullptr;
	nowy -> indeks = idx;
	return nowy;
}

bool isNull(std::shared_ptr<wierzcholek> w) {
	return w == nullptr;
}

void pushBack(int v) { //funkcja tak naprawde odpowiada za tworzenie/aktualizowanie drzewa
	std::shared_ptr<wierzcholek> aktualny;
	std::shared_ptr<wierzcholek> poprzedni;
	int aktualnyIndex;
	if (korzenie.empty()) { //tworzymy nowy typ wierzcholka oraz korzystamy z poprzedniego typu (jesli istnieje)
		poprzedni = nullptr;
		aktualnyIndex = 0;
	}
	else {
		poprzedni = korzenie.back();
		aktualnyIndex = static_cast<int>(korzenie.size());
	}
	aktualny = stworzWierzcholek(aktualnyIndex);
	korzenie.push_back(aktualny);
	long long lewySkraj = INT_MIN;
	long long prawySkraj = INT_MAX;
	while (true) { //patrzymy na wszystkie mozliwe wartosci ciagow z pomoca binsearche'a
		long long srodek = (lewySkraj + prawySkraj) / 2;
		if (srodek < 0 or (srodek == 0 and lewySkraj < 0)) {
			srodek--;
		}
		if (v <= srodek) { //jesli element jest mniejszy niz aktualny srodek
			aktualny -> lewySyn = stworzWierzcholek(aktualnyIndex); //tworzymy kopie obecnego wierzcholka w lewa strone drzewa
			if (!isNull(poprzedni)) { //dopisujemy prawa strone, jesli istnieje
				aktualny -> prawySyn = poprzedni -> prawySyn;
				poprzedni = poprzedni -> lewySyn;
			}
			aktualny = aktualny -> lewySyn; //patrzymy teraz na nowo stworzony wierzcholek
			prawySkraj = srodek;
		}
		else if (v > srodek) { //analogicznie jak dla lewej strony drzewa
			aktualny -> prawySyn = stworzWierzcholek(aktualnyIndex);
			if (!isNull(poprzedni)) {
				aktualny -> lewySyn = poprzedni -> lewySyn;
				poprzedni = poprzedni -> prawySyn;
			}
			aktualny = aktualny -> prawySyn;
			lewySkraj = srodek + 1;
		}
		if (lewySkraj >= prawySkraj) {
			break;
		}
	}
}

int wyszukiwanie(std::shared_ptr<wierzcholek> wezel, int minPrzedzialu, int maxPrzedzialu, long long minAktualne, long long maxAktualne) {
	if (isNull(wezel) or (minAktualne > maxPrzedzialu) or (maxAktualne < minPrzedzialu)) { // jesli wartosci wyjd¹ za zakres albo nie istniej¹ to -1
		return -1;
	}
	if (minAktualne == maxAktualne) { // jesli nasze aktualne min i maks sie zgadzaja, to zwracamy index
		return wezel -> indeks;
	}
	long long srodek = (minAktualne + maxAktualne) / 2; //binsearch po drzewie
	if (srodek < 0 or (srodek == 0 and minAktualne < 0)) {
		srodek--;
	}
	int leweWyszukanie = wyszukiwanie(wezel -> lewySyn, minPrzedzialu, maxPrzedzialu, minAktualne, srodek); // analogicznie przeszukujemy prawe
	int praweWyszukanie = wyszukiwanie(wezel -> prawySyn, minPrzedzialu, maxPrzedzialu, srodek + 1, maxAktualne); // i lewe poddrzewo
	return std::max(leweWyszukanie, praweWyszukanie); // i zwracamy maksimum, zeby index byl najwiekszy
}

int prevInRange(int i, int lo, int hi) { //poprostu wywolujemy wyszukiwanie dla danego indeksu i przedzialu
	return wyszukiwanie(korzenie[i], lo, hi, INT_MIN, INT_MAX);
}

void init(const std::vector<int> &x) { //poczatkowy ciag wprowadzamy z pomoca pushBack'u
	for (int i = 0; i < static_cast<int>(x.size()); i++) {
		pushBack(x[i]);
	}
}

void done() { //nie ma potrzeby czyscic struktury stworzonej z inteligentnych wskaznikow

}