#ifndef _ARY_H_
#define _ARY_H_

#include "stdbool.h"

typedef struct wartosc {
   double start;
   double koniec;
   bool anty;
  } wartosc;

/* Implicite zak³adamy, ¿e wszystkie argumenty typu double s¹ liczbami  */
/* rzeczywistymi, tzn. s¹ ró¿ne od HUGE_VAL, -HUGE_VAL i NAN. */

/* wartosc_dokladnosc(x, p) zwraca wartoœæ reprezentuj¹c¹  */
/* x +/- p%                                                */
/* warunek pocz¹tkowy: p > 0                               */
wartosc wartosc_dokladnosc(double x, double p);

/* wartosc_od_do(x, y) zwraca wartoœæ reprezentuj¹c¹ [x;y] */
/* warunek poczatkowy: x <= y                              */
wartosc wartosc_od_do(double x, double y);

/* wartosc_dokladna(x) zwraca wartoœæ reprezentuj¹c¹ [x;x] */
wartosc wartosc_dokladna(double x);

/* in_wartosc(w, x) = x \in w                              */
bool in_wartosc(wartosc w, double x);

/* min_wartosc(w) = najmniejsza mo¿liwa wartoœæ w,         */
/* lub -HUGE_VAL jeœli brak dolnego ograniczenia.          */
double min_wartosc(wartosc w);

/* max_wartosc(w) = najwiêksza mo¿liwa wartoœæ w,          */
/* lub HUGE_VAL jeœli brak górnego ograniczenia.           */
double max_wartosc(wartosc w);

/* œrodek przedzia³u od min_wartosc do max_wartosc,        */
/* lub nan jeœli min i max_wartosc nie s¹ okreœlone.       */
double sr_wartosc(wartosc w);

/* Operacje arytmetyczne na niedok³adnych wartoœciach.     */
wartosc plus(wartosc a, wartosc b);
wartosc minus(wartosc a, wartosc b);
wartosc razy(wartosc a, wartosc b);
wartosc podzielic(wartosc a, wartosc b);

#endif
