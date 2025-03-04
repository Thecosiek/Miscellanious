#ifndef _ARY_H_
#define _ARY_H_

#include "stdbool.h"

typedef struct wartosc {
   double start;
   double koniec;
   bool anty;
  } wartosc;

/* Implicite zak�adamy, �e wszystkie argumenty typu double s� liczbami  */
/* rzeczywistymi, tzn. s� r�ne od HUGE_VAL, -HUGE_VAL i NAN. */

/* wartosc_dokladnosc(x, p) zwraca warto�� reprezentuj�c�  */
/* x +/- p%                                                */
/* warunek pocz�tkowy: p > 0                               */
wartosc wartosc_dokladnosc(double x, double p);

/* wartosc_od_do(x, y) zwraca warto�� reprezentuj�c� [x;y] */
/* warunek poczatkowy: x <= y                              */
wartosc wartosc_od_do(double x, double y);

/* wartosc_dokladna(x) zwraca warto�� reprezentuj�c� [x;x] */
wartosc wartosc_dokladna(double x);

/* in_wartosc(w, x) = x \in w                              */
bool in_wartosc(wartosc w, double x);

/* min_wartosc(w) = najmniejsza mo�liwa warto�� w,         */
/* lub -HUGE_VAL je�li brak dolnego ograniczenia.          */
double min_wartosc(wartosc w);

/* max_wartosc(w) = najwi�ksza mo�liwa warto�� w,          */
/* lub HUGE_VAL je�li brak g�rnego ograniczenia.           */
double max_wartosc(wartosc w);

/* �rodek przedzia�u od min_wartosc do max_wartosc,        */
/* lub nan je�li min i max_wartosc nie s� okre�lone.       */
double sr_wartosc(wartosc w);

/* Operacje arytmetyczne na niedok�adnych warto�ciach.     */
wartosc plus(wartosc a, wartosc b);
wartosc minus(wartosc a, wartosc b);
wartosc razy(wartosc a, wartosc b);
wartosc podzielic(wartosc a, wartosc b);

#endif
