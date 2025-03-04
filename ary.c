// This code paired with ary.h can effectively do atythmetic operation between segments(a, b, q) where a is start of segment, b is
// end of segment and q is bool which determines if segment is [a;b] or [[-inf;a]u[b;inf]]. In code there are some mathematical simplifications
// like: 1/inf = 0 etc.

#include "ary.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>


const long double eps = 1e-10;

bool isequal(double a, double b){
	return fabs(a - b) < eps;
}
bool iszero(double a){
	return fabs(a) < eps;
}

struct wartosc wartosc_dokladnosc(double x, double p) { //Konstruktor - zwraca x +/- p% (dla p>0)
  struct wartosc wynik;
  if(isinf(x) || isnan(x)) {
  wynik.koniec = NAN;
  wynik.start = NAN;
  wynik.anty = 0;
  return wynik;
  }
  double blad = x * (p / 100.0);
  wynik.start = fmin(x - blad,x + blad);
  wynik.koniec = fmax(x + blad, x-blad);
  wynik.anty = 0;
  return wynik;
}

struct wartosc wartosc_od_do(double x, double y) { //Konstruktor - zwraca (x+y)/2 +/- (y-x)/2 przy założeniu: y>x
  struct wartosc wynik;
  if (isinf(x) || isinf(y) || isnan(x) || isnan(y)) {
    wynik.start = NAN;
    wynik.koniec = NAN;
    wynik.anty = 0;
    return wynik;
  }
  double a = (x + y) / 2;
  double b = (y - x) / 2;
  wynik.start = fmin(a - b, a + b);
  wynik.koniec = fmax(a - b, a + b);
  wynik.anty = 0;
  return wynik;
}

struct wartosc wartosc_dokladna(double x) { //Konstruktor - zwraca x +/- 0 czyli 'przedział' od x do x;
  struct wartosc wynik;
  if (isinf(x) || isnan(x)) {
    wynik.start = NAN;
    wynik.koniec = NAN;
    wynik.anty = 0;
    return wynik;
  }
  wynik.start = x;
  wynik.koniec = x;
  wynik.anty = 0;
  return wynik;
}

bool in_wartosc(struct wartosc x, double y) { //Selektor - zwraca prawdę jeśli y jest w przedziale określonym przez x
  if ((isinf(x.start) && isinf(x.koniec)) && x.anty == 1) {
    return false;
  }
  if ((isnan(x.start) || isnan(x.koniec))) {
    return false;
  }
  if ((isinf(x.start) && isinf(x.koniec)) && x.anty == 0) {
    return true;
  }
  if (x.start <= y + eps && x.koniec >= y - eps && x.anty == 0) {
    return true;
  }
  if ((x.start >= y - eps || x.koniec <= y + eps) && x.anty == 1) {
    return true;
  }
  return false;
}

double min_wartosc(struct wartosc x) { //Selektor - zwraca kres dolny
  if (isinf(x.start) && x.anty == 0) {
    return -HUGE_VAL;
  }
  if (isinf(x.start) && x.anty == 1) {
    if (isinf(x.koniec)) {
      return NAN;
    } else {
      return x.koniec;
    }
  }
  if (x.anty == 1) {
    return -HUGE_VAL;
  }
  return x.start;
}

double max_wartosc(struct wartosc x) { //Selektor - zwraca kres górny
  if (isinf(x.koniec) && x.anty == 0) {
    return HUGE_VAL;
  }
  if (isinf(x.koniec) && x.anty == 1) {
    if (isinf(x.start)) {
      return NAN;
    } else {
      return x.start;
    }
  }
  if (x.anty == 1) {
    return HUGE_VAL;
  }
  return x.koniec;
}

double sr_wartosc(struct wartosc x) { //Selektor - zwraca średnią arytmetyczną
  if(isinf(x.start) && x.anty == 0) {
	if(isinf(x.koniec)){
		return NAN;
	}
	else{
		return -HUGE_VAL;
	}
  }
  if(isinf(x.koniec) && x.anty == 0) {
	if(isinf(x.start)){
		return NAN;
	}
	else{
		return HUGE_VAL;
	}
  }
  if (x.anty == 1) {
    return NAN;
  }
  return (x.start + x.koniec) / 2;
}

struct wartosc plus(struct wartosc a, struct wartosc b) { //Modyfikat|| - dodawanie
  	struct wartosc wynik;
  if((isinf(a.start) && isinf(a.koniec) && a.anty==1)||(isinf(b.start) && isinf(b.koniec) && b.anty==1)
  ||(isnan(b.start) || isnan(b.koniec) 
  || isnan(a.start) || isnan(a.koniec))){
  	wynik.start = NAN;
  	wynik.koniec = NAN;
  	wynik.anty = 0;
  	return wynik;
  }
  else if ((a.anty == 1 && b.anty == 1) 
  || (a.anty == 1 && isinf(b.start) && isinf(b.koniec)) || 
  (b.anty == 1 && isinf(a.start) && isinf(a.koniec))){
    wynik.start = -HUGE_VAL;
    wynik.koniec = HUGE_VAL;
    wynik.anty = 0;
    return wynik;
  } 
  else if (a.anty == 0 && b.anty == 0) {
    wynik.start = fmin(a.start + b.start, a.start + b.koniec);
    wynik.koniec = fmax(a.koniec + b.koniec, a.koniec + b.start);
    wynik.anty = 0;
    return wynik;
  } else if (a.anty == 1) {
    wynik.start = a.start + b.koniec;
    wynik.koniec = a.koniec + b.start;
    if (wynik.start > wynik.koniec) {
      wynik.start = NAN;
      wynik.koniec = NAN;
    }
    wynik.anty = 1;
    return wynik;
  } 
  else if (b.anty == 1) {
    wynik.start = b.start + a.koniec;
    wynik.koniec = b.koniec + a.start;
    if (wynik.start > wynik.koniec) {
      wynik.start = NAN;
      wynik.koniec = NAN;
    }
    wynik.anty = 1;
    return wynik;
  }
  return wynik;
}

struct wartosc minus(struct wartosc a, struct wartosc b) { //Modyfikat|| - odejmowanie
  struct wartosc wynik;
   if((isinf(a.start) && isinf(a.koniec) && a.anty==1)||(isinf(b.start) && isinf(b.koniec) && b.anty==1)||(isnan(b.start) || isnan(b.koniec) || isnan(a.start) || isnan(a.koniec))){
  	wynik.start = NAN;
  	wynik.koniec = NAN;
  	wynik.anty = 0;
  	return wynik;
  }
  else if ((a.anty == 1 && b.anty == 1) || (a.anty == 1 && isinf(b.start) && isinf(b.koniec)) || (b.anty == 1 && isinf(a.start) && isinf(a.koniec))) {
    wynik.start = -HUGE_VAL;
    wynik.start = HUGE_VAL;
    wynik.anty = 0;
    return wynik;
  } else if (a.anty == 0 && b.anty == 0) {
    wynik.start = fmin(a.start - b.start, a.start-b.koniec);
    wynik.koniec = fmax(a.koniec - b.koniec, a.koniec-b.start);
    wynik.anty = 0;
    return wynik;
  } else if (a.anty == 1) {
    wynik.start = fmax(a.start - b.koniec, a.start-b.start);
    wynik.koniec = fmin(a.koniec - b.start, a.koniec-b.koniec);
    if (wynik.start > wynik.koniec) {
      wynik.start = NAN;
      wynik.koniec = NAN;
    }
    wynik.anty = 1;
    return wynik;
  } else if (b.anty == 1) {
    wynik.start = b.start - a.koniec;
    wynik.koniec = b.koniec - a.start;
    if (wynik.start > wynik.koniec) {
      wynik.start = NAN;
      wynik.koniec = NAN;
    }
    wynik.anty = 1;
    return wynik;
  }
  return wynik;
}

struct wartosc razy(struct wartosc a, struct wartosc b) { //Modyfikator - mnożenie przedziałów
	if((isinf(a.start) && isinf(a.koniec) && a.anty==1)
	||(isinf(b.start) && isinf(b.koniec) && b.anty==1)
	||(isnan(b.start) || isnan(b.koniec) || isnan(a.start) 
	|| isnan(a.koniec))){
  	struct wartosc wynik;
  	wynik.start = NAN;
  	wynik.koniec = NAN;
  	wynik.anty = 0;
  //	printf("1\n");
  	return wynik;
  }
  if((iszero(a.koniec) && iszero(a.start) && a.anty == 0) 
  || (b.anty == 0  && iszero(b.koniec) && iszero(b.start))){
  	struct wartosc wynik;
    wynik.start = 0.0;
    wynik.koniec = 0.0;
    wynik.anty = 0;
    return wynik;
  }
  if (a.anty == 0 && b.anty == 0) {
    double w1, w2, w3, w4;
    w1 = a.start * b.start;
    w2 = a.koniec * b.koniec;
    w3 = a.start * b.koniec;
    w4 = a.koniec * b.start;
    w1 = isnan(w1) ? 0 : w1;
    w2 = isnan(w2) ? 0 : w2;
    w3 = isnan(w3) ? 0 : w3;
    w4 = isnan(w4) ? 0 : w4;
    struct wartosc wynik;
    wynik.start = fmin(w1, fmin(w2, fmin(w3, w4)));
    wynik.koniec = fmax(w1, fmax(w2, fmax(w3, w4)));
    wynik.anty = 0;
    return wynik;
  } 
  else if(a.anty == 0 && b.anty == 1 
  && (isinf(a.start) || isinf(a.koniec))) {
    struct wartosc wynik;
    if(isinf(a.start)){
    	wynik.start = b.start*a.koniec;
    	wynik.start = isnan(wynik.start) ? 0 : wynik.start;
    	wynik.koniec = b.koniec*a.koniec;
    	wynik.koniec = isnan(wynik.koniec) ? 0 : wynik.koniec;
	}
	else if(isinf(a.koniec)){
    	wynik.start = b.start*a.start;
    	wynik.start = isnan(wynik.start) ? 0 : wynik.start;
    	wynik.koniec = b.koniec*a.start;
    	wynik.koniec = isnan(wynik.koniec) ? 0 : wynik.koniec;
	}
    wynik.anty = 1;
    return wynik;
  } 
  else if (a.anty == 0 && b.anty == 1){
    struct wartosc wynik;
    if(a.koniec > -eps && a.start < eps){
    	wynik.anty = 0;
    	wynik.koniec = HUGE_VAL;
    	wynik.start = -HUGE_VAL;
		return wynik;
	}
	else if(a.koniec < eps){
		wynik.start = a.koniec * b.koniec;
		wynik.start = isnan(wynik.start) ? 0 : wynik.start;
		wynik.anty = 1;
		wynik.koniec = a.koniec * b.start;
		wynik.koniec = isnan(wynik.koniec) ? 0 : wynik.koniec;
		return wynik;
	}
	else if(a.start > -eps){
		wynik.anty = 1;
		wynik.koniec = a.start * b.koniec;
		wynik.koniec = isnan(wynik.koniec) ? 0 : wynik.koniec;
		wynik.start = a.start * b.start;
		wynik.start = isnan(wynik.start) ? 0 : wynik.start;
		return wynik;
	}
    return wynik;
  }
  else if(a.anty == 1 && b.anty == 0){
  	struct wartosc wynik;
  	if(b.start < eps && b.koniec > -eps){
  		wynik.anty = 0;
  		wynik.start = -HUGE_VAL;
  		wynik.koniec = HUGE_VAL;
  		return wynik;
	}
	else if(b.start > -eps){
		wynik.anty = 1;
		wynik.koniec = b.start * a.koniec;
		wynik.koniec = isnan(wynik.koniec) ? 0 : wynik.koniec;
		wynik.start = b.start * a.start;	
		wynik.start = isnan(wynik.start) ? 0 : wynik.start;
		return wynik;
	} 
	else if(b.koniec < eps){
		wynik.koniec = b.koniec * a.start;
		wynik.koniec = isnan(wynik.koniec) ? 0 : wynik.koniec;
		wynik.start = b.koniec * a.koniec;
		wynik.start = isnan(wynik.start) ? 0 : wynik.start;
		wynik.anty = 1;
		return wynik;
		}
		return wynik;
  	} 
   else {
    struct wartosc wynik;
    if(a.start > -eps || a.koniec < eps || b.start > -eps || b.koniec < eps){
    	wynik.start = -HUGE_VAL;
    	wynik.koniec = HUGE_VAL;
    	wynik.anty = 0;
    	return wynik;
	}
	double w1, w2, w3, w4;
    	w1 = a.start * b.start;
    	w2 = a.koniec * b.koniec;
    	w3 = a.start * b.koniec;
    	w4 = a.koniec * b.start;
    	w1 = isnan(w1) ? 0 : w1;
    	w2 = isnan(w2) ? 0 : w2;
   		w3 = isnan(w3) ? 0 : w3;
    	w4 = isnan(w4) ? 0 : w4;
    wynik.start = fmax(w3, w4);
    wynik.koniec = fmin(w1, w2);
    if (wynik.start > wynik.koniec) {
      wynik.start = NAN;
     wynik.koniec = NAN;
    }
    wynik.anty = 1;
    return wynik;
  }
}

struct wartosc podzielic(struct wartosc a, struct wartosc b) {
  	wartosc odwroc;
  	odwroc.koniec = NAN;
  	odwroc.start = NAN;
  	odwroc.anty = 0;
  	if(isnan(a.start) || isnan(b.start) ||
	   isnan(a.koniec) || isnan(b.koniec) || 
	  (isinf(a.start) && isinf(a.koniec) && a.anty == 1) || 
	  (isinf(b.start) && isinf(b.koniec) && b.anty == 1)||
	   (isequal(b.koniec, 0) && isequal(b.start, 0))){
	  	return odwroc;	
	  }
  	else if(iszero(b.koniec) && b.anty == 1){
  		odwroc.anty = 0;
  		odwroc.koniec = HUGE_VAL;
  		odwroc.start = 1 / b.start;
	  }
	else if(iszero(b.start) && b.anty == 1){
		odwroc.anty = 0;
		odwroc.koniec = 1 / b.koniec;
		odwroc.start = -HUGE_VAL;
	}
	else if(iszero(b.koniec) && b.anty == 0){
		odwroc.anty = 0;
		odwroc.koniec = 1 / b.start;
		odwroc.start = -HUGE_VAL;
	}
	else if(iszero(b.start) && b.anty == 0){
		odwroc.anty = 0;
		odwroc.start = 1 / b.koniec;
		odwroc.koniec = HUGE_VAL;
	}
	else if(b.koniec > -eps && b.start < eps){
		odwroc.anty = 1 - b.anty;
		odwroc.koniec = 1 / b.koniec;
		odwroc.start = 1 / b.start;
	}
	else{
		odwroc.anty = b.anty;
		odwroc.koniec = fmax(1/b.start, 1/b.koniec);
		odwroc.start = fmin(1/b.koniec, 1/b.start);	
	}
  	return razy(a, odwroc);
  
}
