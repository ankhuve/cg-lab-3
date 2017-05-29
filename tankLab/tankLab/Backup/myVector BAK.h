#include <math.h>



const double M_PI = 3.14159265358979323846;
const double M_HALF_PI = 1.57079632679489661923;

const double RAD = (M_PI / 180.0);
const double PIOVER2 = (3.14159265358979323846 / 2);
#define DEG2RAD(x) ((x)*M_PI/180.0)
#define RAD2DEG(x) ((x)*180.0/M_PI)

//namespace MyMathLibrary
//{


struct Position
{
	float x;
	float y;
	float z;
};

class MyVector
{
public:
	MyVector(void);
	MyVector(float x, float y, float z);
	MyVector(Position& start, Position& end);
	~MyVector(void) { ; }

	MyVector addTo(const MyVector &other) const;
	MyVector subtractFrom(const MyVector &other) const;

	float getMagnitude(void) const;
	void setMagnitude(const float m);
	MyVector getCrossProduct(const MyVector &other) const;

	float getDotProduct(const MyVector &other) const;

	void normalise(void);

	// New: Overloaded Operators...
	MyVector operator + (const MyVector &other);
	MyVector operator - (const MyVector &other);
	MyVector operator * (const MyVector &other);
	MyVector operator / (const MyVector &other);

	MyVector operator * (const float scalar);
	friend MyVector operator * (const float scalar, const MyVector &other);

	MyVector& operator = (const MyVector &other);
	MyVector& operator += (const MyVector &other);
	MyVector& operator -= (const MyVector &other);

	MyVector operator + (void) const;
	MyVector operator - (void) const;

	float x;
	float y;
	float z;
};

//}