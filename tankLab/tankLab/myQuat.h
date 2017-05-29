//Introductory quaternion class
#include <math.h>

#ifndef MYVECTOR_AND_MYMATRIX_H_INCLUDED
#define MYVECTOR_AND_MYMATRIX_H_INCLUDED

#include "myVector.h"
#include "mymatrix.h"
//
//namespace MyMathLibrary
//{

class MyQuat
{
	public:
		MyQuat(void);
		MyQuat(float angleDeg, MyVector &axis);	//create from axis, angle
		MyQuat(Position &p);	//create from position
		
		~MyQuat(void){;}

		MyQuat addTo(const MyQuat &other) const;
		MyQuat multiplyBy(const MyQuat &other) const;

		float getMagnitude(void) const;	
		void normalise(void);
		MyQuat getConjugate(void) const;	
		MyQuat getInverse(void) const;

		MyMatrix convertToRotationMatrix(void) const; 

		float w;
		MyVector v;
};


//}

#endif