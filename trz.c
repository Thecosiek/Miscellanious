#include <stdio.h>

#include <limits.h>

#include <stdlib.h>

const int MAXIMUM = INT_MAX;

int min(int a, int b) {
  return a < b ? a : b;
}

int max(int a, int b) {
  return a > b ? a : b;
}

int czy_trzy_sieci(int * siec, int n) { //sprawdzanie czy wystepuja przynajmniej 3 sieci hoteli
  int pierwszy = 0, drugi = 1;
  while (siec[pierwszy] == siec[drugi]) {
    drugi++;
    if (drugi == n) {
      printf("0 0");
      return 0;
    }
  }
  int trzeci = drugi;
  while (siec[pierwszy] == siec[trzeci] || siec[drugi] == siec[trzeci]) {
    trzeci++;
    if (trzeci == n) {
      printf("0 0");
      return 0;
    }
  }
  return 1;
}

int sprawdz_dlugi_przedzial(int * siec, int * odl, int poczatek, int koniec, int maxp) { //sprawdza max przedzialow
  if (siec[poczatek] == siec[koniec]) { // dla danych A, C  i wszystkich mo¿liwych B													
    return maxp;
  }
  int potencjalny_maks = 0;
  for (int x = poczatek + 1; x < koniec; x++) {
    int aktualny;
    aktualny = min(odl[x] - odl[poczatek], odl[koniec] - odl[x]);
    if (aktualny > potencjalny_maks && siec[x] != siec[poczatek] && siec[x] != siec[koniec]) {
      potencjalny_maks = aktualny;
    }
  }
  return max(maxp, potencjalny_maks);
}

int dlugi_przedzial(int * siec, int * odl, int n) {
  int maxp = 0;
  int * l = (int * ) calloc(3, sizeof(int)); //potencjalne wartosci z lewej
  l[0] = 0;
  l[1] = 0;
  while (siec[l[0]] == siec[l[1]]) {
    l[1]++;
  }
  l[2] = l[1];
  while ((siec[l[2]] == siec[l[1]] || siec[l[2]] == siec[l[0]]) && l[2] < n - 1) {
    l[2]++;
  }

  int * p = (int * ) calloc(3, sizeof(int)); // potencjalne wartosci z prawej
  p[0] = n - 1;
  p[1] = p[0];
  while (siec[p[0]] == siec[p[1]]) {
    p[1]--;
  }
  p[2] = p[1];
  while ((siec[p[2]] == siec[p[1]] || siec[p[2]] == siec[p[0]]) && p[2] >= 0) {
    p[2]--;
  }
  for (int x = 0; x < 3; x++) { //sprawdzenie wszystkich potencjalnych lewych i prawych
    for (int y = 0; y < 3; y++) {
      maxp = sprawdz_dlugi_przedzial(siec, odl, l[x], p[y], maxp);
    }
  }
  free(l);
  free(p);
  return maxp;
}

int sprawdz_krotki_przedzial(int * odl, int A, int B, int C, int krotki) { //sprawdzenie najmniejszego 
  int aktualna = max(odl[B] - odl[A], odl[C] - odl[B]); //maximum dla danych A, B, C 
  return min(aktualna, krotki);
}

int krotki_przedzial(int * siec, int * odl, int * ostatni, int * nastepny, int n) {
  int krotki = MAXIMUM;
  for (int x = 1; x < n - 1; x++) { //przechodzimy po wszystkich najbli¿szych polozeniem sieciach
    int A = ostatni[x]; //
    int C = nastepny[x];
    if (A != -1 && C != n && siec[A] != siec[C]) { //jesli mozemy to kazdy sprawdzamy
      krotki = sprawdz_krotki_przedzial(odl, A, x, C, krotki);
    }
  }
  return krotki;
}

int main() {

  int n;
  scanf("%d", & n);
  int * siec = (int * ) calloc((size_t)(n), sizeof(int)); // tworzymy tablice sieci moteli
  int * odl = (int * ) calloc((size_t)(n), sizeof(int)); // i odleglosci

  int * ostatni = (int * ) calloc((size_t)(n), sizeof(int)); // dla danego i wyznacza takie k < i ze siec(i)=/=siec(k)
  int * nastepny = (int * ) calloc((size_t)(n), sizeof(int)); // analogicznie ale k > i

  for (int j = 0; j < n; j++) {
    scanf("%d", siec + j); //uzupelnienie tablicy moteli
    scanf("%d", odl + j);
  }

  ostatni[0] = -1; //uzupelnienie wyzej wspomnianej tablicy ostatni
  int x = 1;
  while (x < n) {
    if (siec[x] == siec[x - 1]) {
      ostatni[x] = ostatni[x - 1];
    } else {
      ostatni[x] = x - 1;
    }
    x++;
  }

  nastepny[n - 1] = n; //uzupelnienie wyzej wspomnianej tablicy nastepny
  int y = n - 2;
  while (y >= 0) {
    if (siec[y] == siec[y + 1]) {
      nastepny[y] = nastepny[y + 1];
    } else {
      nastepny[y] = y + 1;
    }
    y--;
  }

  int k = 0; //zmienna ktora sprawdza czy nie wypisane zostaly dwa 0

  if (czy_trzy_sieci(siec, n) == 0) { //sprawdzamy czy s¹ przynajmniej 3 sieci moteli
    k = 1;
  }

  int minofmax = dlugi_przedzial(siec, odl, n); // wartosc najmniejsze maksimum
  int maxofmin = krotki_przedzial(siec, odl, ostatni, nastepny, n); //wartosc najwieksze minimum

  if (k == 0) {
    printf("%d %d\n", maxofmin, minofmax);
  }

  free(siec);
  free(odl);
  free(nastepny);
  free(ostatni);

  return 0;
}
