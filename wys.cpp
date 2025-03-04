#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <limits>
#include "wys.h"
using namespace std;

int DUZA_WARTOSC = 100;

int liczby, klamstwa, gry; // n, k, g

map<std::vector<int>, int > ruchyGry; // mapa mo¿liwych stanów gry


int preprocessing_stanow(std::vector<int> stan, int n) {
	if (ruchyGry.count(stan)) { // jeœli dany stan ju¿ wyst¹pi³ to znamy jego wartoœæ
		return ruchyGry[stan];
	}
	//warunek ktory konczy rekurencje, gdy zostaje jedna mozliwa wartosc
	int ileWartosci = 0;
	for (int x = 1; x <= n; x++) {
		if (stan[x] != 0) {
			ileWartosci++;
		}
	}
	if (ileWartosci <= 1) {
		return ruchyGry[stan] = 1;
	}
	//wynikiem jest najlepsze zapytanie w danym momencie (takie ktore da jak najszybsza odpowiedz)
	int najlepszy = DUZA_WARTOSC;
	//przechodzimy po mozliwych pytaniach (zakladamy ze pytanie czy < 1 jest nieoptymalne)
	for (int x = 2; x <= n; x++) {
		//tworzymy wektory do obu odpowiedzi podczas pytania, po czym je zmniejszamy
		std::vector<int>jesliTak = stan;
		std::vector<int>jesliNie = stan;
		for (int i = 1; i < x; i++) { //jesli odp tak, zmniejszamy wszystkie wartosci < x;
			if (jesliTak[i] > 0) {
				jesliTak[i]--;
			}
		}
		for (int j = x; j <= n; j++) { //jesli nie, zmniejszamy wartosci >= x
			if (jesliNie[j] > 0) {
				jesliNie[j]--;
			}
		}
		int wyniki = DUZA_WARTOSC; //ustawiamy wartosc tak, zeby w razie czego nie miala wplywu na odpowiedz
		if (jesliNie != stan and jesliTak != stan) { //jesli pytanie niezaleznie od odpowiedzi poprawia stan to patrzymy na
			wyniki = std::max(preprocessing_stanow(jesliNie, n), preprocessing_stanow(jesliTak, n)); //gorszy przypadek
		}
		if (wyniki < najlepszy) { //jesli jest lepszy niz dotychczasowe odpowiedzi to staje sie potencjalna odpowiedzia
			najlepszy = wyniki;
		}
	}
	return ruchyGry[stan] = najlepszy + 1; // zapisujemy wynik dla danego stanu gry
}


int main() {
	dajParametry(liczby, klamstwa, gry);
	for (int z = 0; z < gry; z++) {
		std::vector<int> stan(liczby + 1, klamstwa + 1); //tworzymy wektor ktory kazdej liczbie daje k+1
		while (preprocessing_stanow(stan, liczby) != 1) { //dopoki nie wiadomo jaka to liczba (funkcja oddaje ilosc ruchow + 1)
			int najlepsza_wartosc = DUZA_WARTOSC;
			int najlepsze_pytanie = 1;
			std::vector<int> najlepszeTak = stan;
			std::vector<int> najlepszeNie = stan;
			for (int x = 2; x <= liczby; x++){ //szukamy takie pytanie
				std::vector<int>jesliTak = stan;
				std::vector<int>jesliNie = stan;
				for (int i = 1; i < x; i++) {
					if (jesliTak[i] > 0) {
						jesliTak[i]--;
					}
				}
				for (int j = x; j <= liczby; j++) {
					if (jesliNie[j] > 0) {
						jesliNie[j]--;
					}
				} 
				int wyniki = DUZA_WARTOSC; //ktore niezaleznie od odpowiedzi bedzie najlepsze
				if (jesliNie != stan and jesliTak != stan and ruchyGry[stan] - ruchyGry[jesliNie] >= 1 and ruchyGry[stan] - ruchyGry[jesliTak] >= 1) {
					wyniki = std::max(preprocessing_stanow(jesliNie, liczby), preprocessing_stanow(jesliTak, liczby));
				}
				if (wyniki < najlepsza_wartosc) {
					najlepsza_wartosc = wyniki; //zapamietujemy pytanie oraz stany po nim
					najlepsze_pytanie = x;
					najlepszeNie = jesliNie;
					najlepszeTak = jesliTak;
				}
			}
			if (!mniejszaNiz(najlepsze_pytanie)) { // w zaleznosci od odpowiedzi Ali zmieniamy stan
				stan = najlepszeTak;
			}
			else {
				stan = najlepszeNie;
			}
		}
		for (int x = 1; x < int(stan.size()); x++) { //jesli nie potrzeba pytan, znaczy ze poczatkowa tablica ma tylko jedna wartosc
			if (stan[x] > 0) { //wieksza niz 0, ta wartosc bedzie odpowiedzia dla danej gry
				odpowiedz(x);
				break;
			}
		}
	}
	return 0;
}