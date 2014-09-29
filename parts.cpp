#include "parts.h"

supplies::supplies(): water(0), minerals(0), sugar(0) {}
supplies::supplies(int w, int m, int s): water(w), minerals(m), sugar(s) {}
std::ostream& operator<<(std::ostream& output, const supplies& s){
	output << "(" << s.water << "," <<  s.minerals << "," <<  s.sugar << ")" << std::endl;
	return output;
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
bool supplies::operator>(const supplies& s){
	return (water > s.water && minerals > s.minerals && sugar > s.sugar);
}

bool supplies::operator<(const supplies& s){
	return (water < s.water && minerals < s.minerals && sugar < s.sugar);
}

hormones::hormones(): auxin(0), growth(0){}
hormones::hormones(double a, double g): auxin(a), growth(g){}

hormones& hormones::operator+(const hormones& h){
	auxin += h.auxin;
	growth += h.growth;
}

hormones& hormones::operator-(const hormones& h){
	auxin -= h.auxin;
	growth -= h.growth;
}

bool hormones::operator<(const hormones& h){
	return (auxin < h.auxin && growth < h.growth);
}

bool hormones::operator>(const hormones& h){
	return (auxin > h.auxin && growth > h.growth);
}

part::part(part* p, supplies u): parent(p), uses(u), horm(hormones(0,0)), togrow(0), state(1){
	devAngle = 15 - ((double)(random()%1000))/33.3f;
}

part* part::transport(supplies& s, hormones& h){
	if(feed(s) == -1)
		return NULL;
	effect(h);

	return this;
}

/* each plant part uses a certain amount of supplies to survive. if there isn't enough 
 * (or maybe too much) of a certain supply then this part dies off. 
 */
int part::feed(supplies& s){
	s -= uses;
	if(s.water < 0 or s.minerals < 0 or s.sugar < 0){
		die();
		return -1;
	}
	return 0;
}

int part::effect(hormones& h){
	horm = h;
}

void part::die(){
	state = 0;
}

void part::print(){}
