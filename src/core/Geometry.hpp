// All source code bellow is based on the material provided by Peter Shirley in
// "Ray tracing in one weekend", avaliable in: https://github.com/RayTracing/InOneWeekend/releases

#ifndef GEOMETRYH
#define GEOMETRYH

#include <math.h>
#include <stdlib.h>
#include <iostream>

template <typename T>
class Vector3
{
	public:
		T x;
		T y;
		T z;

		Vector3(){};

		Vector3(T x, T y, T z)
		{
			this->x = x; 
			this->y = y;
			this->z = z;
		}

		//inline T x() const
		//{
		//	return this->x;
		//}

		//inline T y() const 
		//{
		//	return this->y;
		//}

		//inline T z() const
		//{
		//	return this->z;
		//}

		inline const Vector3<T>& operator+() const
		{ 
			return *this; 
		}

		inline Vector3<T> operator-() const
		{ 
			return Vector3(-this->x, -this->y, -this->z); 
		}

		inline T operator[](int index) const
		{ 
			if(index == 0)
			{
				return this->x;
			}
			else if(index == 1)
			{
				return this->y;
			}
			else if(index == 2)
			{
				return this->z;
			}
			else
			{
				std::cout << "Index out of bounds" << std::endl;
				// Returning a deafult value
				return -1;
			}
		}

		inline T& operator[](int index)
		{ 
			if(index == 0)
			{
				return this->x;
			}
			else if(index == 1)
			{
				return this->y;
			}
			else if(index == 2)
			{
				return this->z;
			}
			else
			{
				std::cout << "Index out of bounds" << std::endl;
				// Returning a default reference (couldn't think of anything better)
				return this->x;
			} 
		}

		inline void operator=(const Vector3<T> other)
		{
			this->x = other.x;
		    this->y = other.y;
		    this->z = other.z;
		}

		inline Vector3<T>& operator+=(const Vector3<T> &other)
		{
		    this->x += other.x;
		    this->y += other.y;
		    this->z += other.z;

		    return *this;
		}

		inline Vector3<T>& operator*=(const Vector3<T> &other)
		{
		    this->x *= other.x;
		    this->y *= other.y;
		    this->z *= other.z;

		    return *this;
		}

		inline Vector3<T>& operator/=(const Vector3<T> &other)
		{
		    this->x /= other.x;
		    this->y /= other.y;
		    this->z /= other.z;

		    return *this;
		}

		inline Vector3<T>& operator-=(const Vector3<T> &other) 
		{
		    this->x -= other.x;
		    this->y -= other.y;
		    this->z -= other.z;

		    return *this;
		}

		inline Vector3<T>& operator*=(const T t) 
		{
		    this->x *= t;
		    this->y *= t;
		    this->z *= t;

		    return *this;
		}

		inline Vector3<T>& operator/=(const T t) 
		{
			if(t == 0)
			{
				std::cout << "Division by zero!!!" << std::endl;
				return *this;
			}

		    float k = 1.0/t;
		    
		    this->x *= k;
		    this->y *= k;
		    this->z *= k;

		    return *this;
		}

		inline T length() const
		{ 
			return sqrt(this->x*this->x + this->y*this->y + this->z*this->z); 
		}

		inline T squared_length() const
		{ 
			return this->x*this->x + this->y*this->y + this->z*this->z; 
		}

		inline void make_unit_vector() 
		{
		    float k = 1.0 / length();
		    
		    this->x *= k;
		    this->y *= k;
		    this->z *= k;
		}
};

template <typename T>
inline Vector3<T> unit_vector(Vector3<T> v) 
{
    return v / v.length();
}

template <typename T>
inline std::istream& operator>>(std::istream &is, Vector3<T> &t) 
{
    is >> t.x >> t.y >> t.z;

    return is;
}

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const Vector3<T> &t) 
{
    os << t.x << " " << t.y << " " << t.z;

    return os;
}

template <typename T>
inline Vector3<T> operator+(const Vector3<T> &v1, const Vector3<T> &v2) 
{
    return Vector3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T> &v1, const Vector3<T> &v2) 
{
    return Vector3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template <typename T>
inline Vector3<T> operator*(const Vector3<T> &v1, const Vector3<T> &v2) 
{
    return Vector3<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

template <typename T>
inline Vector3<T> operator/(const Vector3<T> &v1, const Vector3<T> &v2) 
{
    return Vector3<T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

template <typename T>
inline Vector3<T> operator*(T t, const Vector3<T> &v) 
{
    return Vector3<T>(t*v.x, t*v.y, t*v.z);
}

template <typename T>
inline Vector3<T> operator/(Vector3<T> v, T t) 
{
    return Vector3<T>(v.x/t, v.y/t, v.z/t);
}

template <typename T>
inline Vector3<T> operator*(const Vector3<T> &v, T t) 
{
    return Vector3<T>(t*v.x, t*v.y, t*v.z);
}

template <typename T>
inline T dot(const Vector3<T> &v1, const Vector3<T> &v2) 
{
    return v1.x * v2.x + v1.y * v2.y  + v1.z * v2.z;
}

template <typename T>
inline Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2) 
{
    return -Vector3<T>( (v1.y*v2.z - v1.z*v2.y),
                (-(v1.x*v2.z - v1.z*v2.x)),
                (v1.x*v2.y - v1.y*v2.x));
}


//================================================================================================================================================
//===================================This line marks the beggining of the two-dimensional vector implementation===================================
//================================================================================================================================================

template <typename T>
class Vector2
{
	public:
		T x;
		T y;

		Vector2(){};

		Vector2(T x, T y)
		{
			this->x = x; 
			this->y = y;
		}

		//inline T x() const
		//{
		//	return this->x;
		//}

		//inline T y() const 
		//{
		//	return this->y;
		//}

		//inline T z() const
		//{
		//	return this->z;
		//}

		inline const Vector2<T>& operator+() const
		{ 
			return *this; 
		}

		inline Vector2<T> operator-() const
		{ 
			return Vector2(-this->x, -this->y); 
		}

		inline T operator[](int index) const
		{ 
			if(index == 0)
			{
				return this->x;
			}
			else if(index == 1)
			{
				return this->y;
			}
			else
			{
				std::cout << "Index out of bounds" << std::endl;
				// Returning a deafult value
				return -1;
			}
		}

		inline T& operator[](int index)
		{ 
			if(index == 0)
			{
				return this->x;
			}
			else if(index == 1)
			{
				return this->y;
			}
			else
			{
				std::cout << "Index out of bounds" << std::endl;
				// Returning a default reference (couldn't think of anything better)
				return this->x;
			} 
		}

		inline void operator=(const Vector2<T> other)
		{
			this->x = other.x;
		    this->y = other.y;
		}

		inline Vector2<T>& operator+=(const Vector2<T> &other)
		{
		    this->x += other.x;
		    this->y += other.y;

		    return *this;
		}

		inline Vector2<T>& operator*=(const Vector2<T> &other)
		{
		    this->x *= other.x;
		    this->y *= other.y;

		    return *this;
		}

		inline Vector2<T>& operator/=(const Vector2<T> &other)
		{
		    this->x /= other.x;
		    this->y /= other.y;

		    return *this;
		}

		inline Vector2<T>& operator-=(const Vector2<T> &other) 
		{
		    this->x -= other.x;
		    this->y -= other.y;

		    return *this;
		}

		inline Vector2<T>& operator*=(const T t) 
		{
		    this->x *= t;
		    this->y *= t;

		    return *this;
		}

		inline Vector2<T>& operator/=(const T t) 
		{
			if(t == 0)
			{
				std::cout << "Division by zero!!!" << std::endl;
				return *this;
			}

		    float k = 1.0/t;
		    
		    this->x *= k;
		    this->y *= k;

		    return *this;
		}

		inline T length() const
		{ 
			return sqrt(this->x*this->x + this->y*this->y); 
		}

		inline T squared_length() const
		{ 
			return this->x*this->x + this->y*this->y; 
		}

		inline void make_unit_vector() 
		{
		    float k = 1.0 / length();
		    
		    this->x *= k;
		    this->y *= k;
		}
};

template <typename T>
inline Vector2<T> unit_vector(Vector2<T> v) 
{
    return v / v.length();
}

template <typename T>
inline std::istream& operator>>(std::istream &is, Vector2<T> &t) 
{
    is >> t.x >> t.y;

    return is;
}

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const Vector2<T> &t) 
{
    os << t.x << " " << t.y;

    return os;
}

template <typename T>
inline Vector2<T> operator+(const Vector2<T> &v1, const Vector2<T> &v2) 
{
    return Vector2<T>(v1.x + v2.x, v1.y + v2.y);
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T> &v1, const Vector2<T> &v2) 
{
    return Vector2<T>(v1.x - v2.x, v1.y - v2.y);
}

template <typename T>
inline Vector2<T> operator*(const Vector2<T> &v1, const Vector2<T> &v2) 
{
    return Vector2<T>(v1.x * v2.x, v1.y * v2.y);
}

template <typename T>
inline Vector2<T> operator/(const Vector2<T> &v1, const Vector2<T> &v2) 
{
    return Vector2<T>(v1.x / v2.x, v1.y / v2.y);
}

template <typename T>
inline Vector2<T> operator*(T t, const Vector2<T> &v) 
{
    return Vector2<T>(t*v.x, t*v.y);
}

template <typename T>
inline Vector2<T> operator/(Vector2<T> v, T t) 
{
    return Vector2<T>(v.x/t, v.y/t);
}

template <typename T>
inline Vector2<T> operator*(const Vector2<T> &v, T t) 
{
    return Vector3<T>(t*v.x, t*v.y);
}

template <typename T>
inline T dot(const Vector2<T> &v1, const Vector2<T> &v2) 
{
    return v1.x * v2.x + v1.y * v2.y;
}

// Is there a definition of a cross product between two-dimensional vectors?

//template <typename T>
//inline Vector2<T> cross(const Vector2<T> &v1, const Vector2<T> &v2) 
//{
//    return Vector2<T>( (v1.x * v2.y) - (v1.y * v2.x) );
//}

//=================================================================================================================================================
//===================================This line marks the beggining of the four-dimensional vector implementation===================================
//=================================================================================================================================================

template <typename T>
class Vector4
{
	public:
		T x;
		T y;
		T z;
		T w;

		Vector4(){};

		// We might delete this later
		Vector4(T x, T y, T z)
		{
			this->x = x; 
			this->y = y;
			this->z = z;
			this->w = 1;
		}

		Vector4(T x, T y, T z, T w)
		{
			this->x = x; 
			this->y = y;
			this->z = z;
			this->w = w;
		}

		//inline T x() const
		//{
		//	return this->x;
		//}

		//inline T y() const 
		//{
		//	return this->y;
		//}

		//inline T z() const
		//{
		//	return this->z;
		//}

		inline const Vector4<T>& operator+() const
		{ 
			return *this; 
		}

		inline Vector4<T> operator-() const
		{ 
			return Vector4(-this->x, -this->y, -this->z, this->w); 
		}

		inline T operator[](int index) const
		{ 
			if(index == 0)
			{
				return this->x;
			}
			else if(index == 1)
			{
				return this->y;
			}
			else if(index == 2)
			{
				return this->z;
			}
			else if(index == 3)
			{
				return this->w;
			}
			else
			{
				std::cout << "Index out of bounds" << std::endl;
				// Returning a deafult value
				return -1;
			}
		}

		inline T& operator[](int index)
		{ 
			if(index == 0)
			{
				return this->x;
			}
			else if(index == 1)
			{
				return this->y;
			}
			else if(index == 2)
			{
				return this->z;
			}
			else if(index == 3)
			{
				return this->w;
			}
			else
			{
				std::cout << "Index out of bounds" << std::endl;
				// Returning a default reference (couldn't think of anything better)
				return this->x;
			} 
		}

		inline void operator=(const Vector4<T> other)
		{
			this->x = other.x;
		    this->y = other.y;
		    this->z = other.z;
		    this->w = other.w;
		}

		inline Vector4<T>& operator+=(const Vector4<T> &other)
		{
		    this->x += other.x;
		    this->y += other.y;
		    this->z += other.z;
		    this->w += other.w;

		    return *this;
		}

		inline Vector4<T>& operator*=(const Vector4<T> &other)
		{
		    this->x *= other.x;
		    this->y *= other.y;
		    this->z *= other.z;
		    this->w *= other.w;

		    return *this;
		}

		inline Vector4<T>& operator/=(const Vector4<T> &other)
		{
		    this->x /= other.x;
		    this->y /= other.y;
		    this->z /= other.z;
		    this->w /= other.w;

		    return *this;
		}

		inline Vector4<T>& operator-=(const Vector4<T> &other) 
		{
		    this->x -= other.x;
		    this->y -= other.y;
		    this->z -= other.z;
		    this->w -= other.w;

		    return *this;
		}

		inline Vector4<T>& operator*=(const T t) 
		{
		    this->x *= t;
		    this->y *= t;
		    this->z *= t;
		    this->w *= t;

		    return *this;
		}

		inline Vector4<T>& operator/=(const T t) 
		{
			if(t == 0)
			{
				std::cout << "Division by zero!!!" << std::endl;
				return *this;
			}

		    float k = 1.0/t;
		    
		    this->x *= k;
		    this->y *= k;
		    this->z *= k;
		    this->w *= k;

		    return *this;
		}

		inline T length() const
		{ 
			return sqrt(this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w); 
		}

		inline T squared_length() const
		{ 
			return this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w; 
		}

		inline void make_unit_vector() 
		{
		    float k = 1.0 / length();
		    
		    this->x *= k;
		    this->y *= k;
		    this->z *= k;
		    this->w *= k;
		}
};

template <typename T>
inline Vector4<T> unit_vector(Vector4<T> v) 
{
    return v / v.length();
}

template <typename T>
inline std::istream& operator>>(std::istream &is, Vector4<T> &t) 
{
    is >> t.x >> t.y >> t.z >> t.w;

    return is;
}

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const Vector4<T> &t) 
{
    os << t.x << " " << t.y << " " << t.z << " " << t.w;

    return os;
}

template <typename T>
inline Vector4<T> operator+(const Vector4<T> &v1, const Vector4<T> &v2) 
{
    return Vector4<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

template <typename T>
inline Vector4<T> operator-(const Vector4<T> &v1, const Vector4<T> &v2) 
{
    return Vector4<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

template <typename T>
inline Vector4<T> operator*(const Vector4<T> &v1, const Vector4<T> &v2) 
{
    return Vector4<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

template <typename T>
inline Vector4<T> operator/(const Vector4<T> &v1, const Vector4<T> &v2) 
{
    return Vector4<T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

template <typename T>
inline Vector4<T> operator*(T t, const Vector4<T> &v) 
{
    return Vector4<T>(t*v.x, t*v.y, t*v.z, t*v.w);
}

template <typename T>
inline Vector4<T> operator/(Vector4<T> v, T t) 
{
    return Vector4<T>(v.x/t, v.y/t, v.z/t, v.w/t);
}

template <typename T>
inline Vector4<T> operator*(const Vector4<T> &v, T t) 
{
    return Vector4<T>(t*v.x, t*v.y, t*v.z, t*v.w);
}

template <typename T>
inline T dot(const Vector4<T> &v1, const Vector4<T> &v2) 
{
    return v1.x * v2.x + v1.y * v2.y  + v1.z * v2.z + v1.w * v2.w;
}

// Is there a definition for a four-dimensional cross product?

//template <typename T>
//inline Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2) 
//{
//    return Vector3<T>( (v1.y*v2.z - v1.z*v2.y),
//                (-(v1.x*v2.z - v1.z*v2.x)),
//                (v1.x*v2.y - v1.y*v2.x));
//}

#endif