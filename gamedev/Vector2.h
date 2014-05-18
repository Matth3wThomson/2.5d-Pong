#pragma once
/*
Class:Vector2
Implements:
Author:Rich Davison
Description:VERY simple Vector2 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#include <iostream>
#include "Common.h"

class Vector2	{
public:
	Vector2(void) {
		ToZero();
	}

	Vector2(const float x, const float y) {
		this->x = x;
		this->y = y;
	}

	~Vector2(void){}

	float x;
	float y;

	void ToZero(){
		x = 0.0f;
		y = 0.0f;
	}

	inline Vector2  operator-(const Vector2  &a) const{
		return Vector2(x - a.x,y - a.y);
	}

	inline Vector2  operator+(const Vector2  &a) const{
		return Vector2(x + a.x,y + a.y);
	}

	inline float GetX() const {return x;};
	inline float GetY() const {return y;};

	inline float GetMagnitude() const { return sqrt( (x * x) + (y * y) ); }

	//Returns the distance from this vector to another
	inline float GetDistance(Vector2& rhs){
		return sqrt( pow( (rhs.x - x), 2 ) + pow( (rhs.y - y), 2 ) );	
	};

	//Translates the current vector by the supplied amounts
	inline void Translate(float x, float y){ 
		x += x;
		y += y;
	}

	//Translates the current vector by the supplied vector
	inline void Translate(const Vector2& rhs){
		x += rhs.x;
		y += rhs.y;
	}

	//Dot Product
	inline float DotProduct(Vector2& rhs){
		return this->x * rhs.x + this->y * rhs.y;
	};

	//Returns the a vector2 perpendicular to the current vector
	inline Vector2 perpVec(){
		Vector2 temp( - this->y, this->x);
		return temp;
	}

	//Returns a normalized version of itself
	inline Vector2 GetNormalized(){
		float magnitude = this->GetMagnitude();
		Vector2 temp(this->x / magnitude, this->y / magnitude);
		return temp;
	}

	//Operator overload for subtraction of vectors
	inline Vector2 operator-(const Vector2& rhs){
		Vector2 temp(this->x - rhs.x, this->y - rhs.y); 
		return temp;
	}

	//Operator overload for addition of vectors
	inline Vector2 operator+(const Vector2& rhs){
		Vector2 temp(this->x + rhs.x, this->y + rhs.y);
		return temp;
	}

	//Operator overload for division by a float
	inline Vector2 operator/(const float& rhs){
		Vector2 temp(this->x / rhs, this->y / rhs);
		return temp;
	}

	//Operator overload for equality
	inline bool operator==(const Vector2& rhs){
		if (abs(this->x - rhs.x) < 0.001){
			if (abs(this->y - rhs.y) < 0.001){
				return true;
			}
		}
		return false;
	}

	//Operator overload for assignment
	inline Vector2 operator=(const Vector2* rhs){
		if (this != rhs){
			x = rhs->x;
			y = rhs->y;
		}
		return *this;
	}

	//The assignment operator for a stack variable
	inline Vector2 operator=(Vector2 rhs){
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	//Creates a vector at the distance and angle supplied
	Vector2* CreateVectorAtDistance(float& distance, float& angle){
		Vector2* newPoint = new Vector2();

		//Add on how much x to the x coordinate (CAH)
		newPoint->x = (float) (this->x + cos(angle * PI/180) * distance);

		//Add on how much y to the y coordinate (SOH)
		newPoint->x = (float) (this->y + sin(angle * PI/180) * distance);

		return newPoint;	
	};

	//Returns the angle between this point and a supplied one.
	inline float FindAngle(Vector2& rhs){
		float deltaX = x - rhs.x;
		float deltaY = y - rhs.y;

		return (float) (atan(deltaY / deltaX) * 180 / PI);
	}

	//OStream method
	inline friend std::ostream& operator<<(std::ostream& o, const Vector2& v){
		o << "Vector2(" << v.x << "," << v.y << ")";
		return o;
	}

	inline Vector2 operator*(const float& rhs) const {
		return Vector2(x * rhs, y * rhs);
	}
};