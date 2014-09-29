#ifndef PLANT_PART
#define PLANT_PART
#include <ostream>
#include <stdlib.h>

extern double temperature;
/* used to represent various supplies */
struct supplies{
	double water;
	double minerals;
	double sugar;
	supplies();
	supplies(double w, double m, double s);
	friend std::ostream& operator<<(std::ostream& output, const supplies& s);
	supplies normalise(const supplies& s);
	supplies operator+(const supplies& s);
	supplies& operator+=(const supplies& s);
	supplies operator-(const supplies& s);
	supplies& operator-=(const supplies& s);
	supplies operator*(const double d);
	supplies operator/(const double d);
	supplies operator/(const supplies& s);
	bool operator>(const supplies& s);
	bool operator<(const supplies& s);
};

/* basic hormone structure */
struct hormones{
	double auxin;
	double growth;
	double abscisicAcid;
	hormones();
	hormones(double a, double g, double aA);
	friend std::ostream& operator<<(std::ostream& output, const hormones& s);
	hormones operator+(const hormones& h);
	hormones& operator+=(const hormones& h);
	hormones operator-(const hormones& h);
	hormones& operator-=(const hormones& h);
	hormones operator*(const double d);
	hormones operator/(const double d);
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
	double health; 	// the health of this part, in percentiles. 0 is dead, 1000 means that this part is 
			// healthy enough to grow
	double devAngle;
	supplies conductivity; // how much supplies can be transported by this part. not really sure how to use it properly

	// GL stuff
	/* I'd prefer it to be without this - it's ugly to say the least. maybe someday... */
	static int renderRound; // says what round of rendering this is
public:
	part(part* p,supplies u, supplies g);
	virtual part* transport(supplies& s, hormones& h);
	virtual int feed(supplies& s); 	// sends supplies to this part
	virtual part* grow(supplies& s, hormones& h)=0; 	// causes this part to grow
	virtual double effect(hormones& h); // used to change this part with a hormone
	virtual double effect(hormones& h, hormones& effect, hormones& produce);
	virtual supplies getConductivity();
	virtual void die();
	virtual void drop()=0;
	virtual void draw()=0;
	virtual void print();

	bool isDead(){return state == 0; }
	bool isGrown(){return state == 2;}
	double getHealth(){return health;}
	void grown(){state = 2;}

	static int getRenderRound(){return part::renderRound;}
	static void setRenderRound(int r){part::renderRound = r;}
};
#endif //PLANT_PART
