#ifndef PLANT_PART
#define PLANT_PART
#include <ostream>
#include <stdlib.h>

/* used to represent various supplies */
struct supplies{
	int water;
	int minerals;
	int sugar;
	supplies();
	supplies(int w, int m, int s);
	friend std::ostream& operator<<(std::ostream& output, const supplies& s);
	supplies operator+(const supplies& s);
	supplies& operator+=(const supplies& s);
	supplies operator-(const supplies& s);
	supplies& operator-=(const supplies& s);
	bool operator>(const supplies& s);
	bool operator<(const supplies& s);
};

/* basic hormone structure */
struct hormones{
	double auxin;
	double growth;
	hormones();
	hormones(double a, double g);
	hormones& operator+(const hormones& h);
	hormones& operator+=(const hormones& h);
	hormones& operator-(const hormones& h);
	hormones& operator-=(const hormones& h);
	bool operator<(const hormones& h);
	bool operator>(const hormones& h);
};

/* basic plant part. retains a pointer back to its parent branch. */
class part{
protected:
	int state; 	// this parts state. currently 1 is growing, 2 grown, 0 dead. it really should be done properly
	supplies uses; 	// defines how much supplies this part needs to survive - if it gets less it dies off
	part* parent; 	// pointer to the parent branch
	hormones horm;
	double togrow; 	// decides whether by how much  this part may grow - if it's > 1 then it grows (really just a handy var.)
	double devAngle;
	double conductivity; // how well this part can transmit supplies. not really sure how to use it properly
public:
	part(part* p,supplies u);
	virtual part* transport(supplies& s, hormones& h);
	virtual int feed(supplies& s); 	// sends supplies to this part
	virtual part* grow(supplies& s, hormones& h)=0; 	// causes this part to grow
	virtual int effect(hormones& h); // used to change this part with a hormone
	virtual void die();
	virtual void drop()=0;
	virtual void draw()=0;
	virtual void print();

	bool isDead(){return state ? false : true; }
	bool isGrown(){return state == 2;}
	void grown(){state = 2;}
	double getConductivity(){return conductivity;}
};
#endif //PLANT_PART
