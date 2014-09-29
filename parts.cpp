#include "parts.h"
#include <iostream>

supplies::supplies(): water(0), minerals(0), sugar(0) {}
supplies::supplies(double w, double m, double s): water(w), minerals(m), sugar(s) {}
std::ostream& operator<<(std::ostream& output, const supplies& s){
	output << "(" << s.water << "," <<  s.minerals << "," <<  s.sugar << ")" << std::endl;
	return output;
}
supplies supplies::normalise(const supplies& s){
	supplies tmp;
	tmp.water = water * s.water;
	tmp.minerals = minerals * s.minerals;
	tmp.sugar = sugar * s.sugar;
	return tmp;
}

supplies supplies::operator+(const supplies& s){
	supplies tmp;
	tmp.water = water + s.water;
	tmp.minerals = minerals + s.minerals;
	tmp.sugar = sugar + s.sugar;
	return tmp;
}

supplies& supplies::operator+=(const supplies& s){
	water += s.water;
	minerals += s.minerals;
	sugar += s.sugar;
	return *this;
}

supplies supplies::operator-(const supplies& s){
	supplies tmp;
	tmp.water = water - s.water;
	tmp.minerals = minerals - s.minerals;
	tmp.sugar = sugar - s.sugar;
	return tmp;
}

supplies& supplies::operator-=(const supplies& s){
	water -= s.water;
	minerals -= s.minerals;
	sugar -= s.sugar;
	return *this;
}

supplies supplies::operator*(const double d){
	supplies s;
	s.water = water * d;
	s.minerals = minerals * d;
	s.sugar = sugar * d;
	return s;
}

supplies supplies::operator/(const double d){
	supplies s;
	s.water = water / d;
	s.minerals = minerals / d;
	s.sugar = sugar / d;
	return s;
}

supplies supplies::operator/(const supplies& a){
	supplies s;
	s.water = water / a.water;
	s.minerals = minerals / a.minerals;
	s.sugar = sugar / a.sugar;
	return s;
}

bool supplies::operator>(const supplies& s){
	return (water > s.water || minerals > s.minerals || sugar > s.sugar);
}

bool supplies::operator<(const supplies& s){
	return (water < s.water || minerals < s.minerals || sugar < s.sugar);
}

hormones::hormones(): auxin(0), growth(0), abscisicAcid(0){}
hormones::hormones(double a, double g, double aA): auxin(a), growth(g), abscisicAcid(aA){}
std::ostream& operator<<(std::ostream& output, const hormones& s){
	output << "(" << s.auxin << "," <<  s.growth << "," <<  s.abscisicAcid << ")";
	return output;
}

hormones hormones::operator+(const hormones& h){
	hormones tmp;
	tmp.auxin = auxin + h.auxin;
	tmp.growth = auxin + h.growth;
	tmp.abscisicAcid = abscisicAcid + h.abscisicAcid;
	return tmp;
}

hormones& hormones::operator+=(const hormones& h){
	auxin += h.auxin;
	growth += h.growth;
	abscisicAcid += h.abscisicAcid;
	return *this;
}

hormones hormones::operator-(const hormones& h){
	hormones tmp;
	tmp.auxin = auxin - h.auxin;
	tmp.growth = growth - h.growth;
	tmp.abscisicAcid = abscisicAcid - h.abscisicAcid;
	return tmp;
}

hormones hormones::operator*(const double d){
	hormones h;
	h.auxin = auxin * d;
	h.growth = growth * d;
	h.abscisicAcid = h.abscisicAcid * d;
	return h;
}

hormones hormones::operator/(const double d){
	hormones h;
	h.auxin = auxin / d;
	h.growth = growth / d;
	h.abscisicAcid = h.abscisicAcid / d;
	return h;
}

bool hormones::operator<(const hormones& h){
	return (auxin < h.auxin && growth < h.growth && abscisicAcid < h.abscisicAcid);
}

bool hormones::operator>(const hormones& h){
	return (auxin > h.auxin && growth > h.growth && abscisicAcid > h.abscisicAcid);
}

/* nasty... */
int part::renderRound = 0; 

part::part(part* p, supplies u, supplies g): parent(p), uses(u), horm(hormones(0,0,0)), togrow(0),
			state(1), conductivity(u + g){
	devAngle = 15 - ((double)(random()%1000))/33.3f;
	health = 1000;
}

part* part::transport(supplies& s, hormones& h){
	if(feed(s) == -1)
		return NULL;

	return this;
}

/* each plant part uses a certain amount of supplies to survive. if there isn't enough 
 * (or maybe too much) of a certain supply then this part dies off. 
 */
int part::feed(supplies& s){
	//TODO tmp precaution to ensure bits aren't killed during winter
	if(horm.abscisicAcid > 0)
		return 0;

	s -= uses;
//	std::cout << "after use: " << s << std::endl;
	if(s.water < 0 or s.minerals < 0 or s.sugar < 0){
		if(s.water < 0)
			health -= uses.water;
		
		if(s.minerals < 0)
			health -= uses.minerals;
		
		if(s.sugar < 0)
			health -= uses.sugar;

		if(health <= 0){
			die();
			return -1;
		}
		return 1;
	}else if(health < 1000){
		health += uses.water + uses.minerals + uses.sugar;
	}else if(health > 1000.01){
		health = 1000;
	}
	return 0;
}

double part::effect(hormones& h){
	horm = h;
	return health > 0.001;
}

double part::effect(hormones& h, hormones& effect, hormones& produce){
	if(health < 999.99)
		return 0;

	double growthFactor = h.auxin / effect.auxin;
	if(growthFactor > 1.01)
		growthFactor = -h.auxin/effect.auxin + 2;
	if(growthFactor < 0)
		growthFactor = 0;
	
	horm = h;
	h += produce * growthFactor;
	
	// TODO: a tmp measure to emulate winter
	if(h.abscisicAcid > 0)
		growthFactor = 0;

	if(temperature < 5)
		growthFactor = 0;

	return growthFactor;
}

supplies part::getConductivity(){
	return conductivity;
}

void part::die(){
	std::cout << "dieing" << std::endl;
	state = 0;
}

void part::print(){}
