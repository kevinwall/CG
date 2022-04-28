#include "Geometry.hpp"

class ColorMatrix
{
	public:
		Vector3<int>** image;
		int height;
		int width;

		ColorMatrix()
		{
			this->height = 0;
			this->width = 0;
		}

		ColorMatrix(int height, int width)
		{
			this->height = height; // i
			this->width = width; // j

			image = new Vector3<int>*[height];

			for(int i{0}; i < height; i++)
			{
				image[i] = new Vector3<int>[width];
			}
		}

		inline void operator=(const ColorMatrix other)
		{
			if(this->height == 0 and this->width == 0)
			{
				this->height = other.height;
				this->width = other.width;

				image = new Vector3<int>*[height];

				for(int i{0}; i < height; i++)
				{
					image[i] = new Vector3<int>[width];
				}
			}
		}

		~ColorMatrix()
		{
			for(int i{0}; i < height; i++)
			{
				delete[] image[i];
			}

			delete[] image;
		}

		inline void insertColor(int i, int j, Vector3<int>& color)
		{
			image[i][j] = color;
		}

		inline int* convertIntoArray() const
		{
			int* returnVec = new int[this->height*this->width*3];

			int pos{0};
			for(int i{0}; i < this->height; i++)
			{
				for(int j{0}; j < this->width; j++)
				{ 
					returnVec[pos++] = image[i][j].x;
					returnVec[pos++] = image[i][j].y;
					returnVec[pos++] = image[i][j].z;
				}
			}

			return returnVec;
		}
};

inline std::ostream& operator<<(std::ostream &os, const ColorMatrix &t) 
{
    for(int i{0}; i < t.height; i++)
    {
    	os << "[ ";
    	for(int j{0}; j < t.width; j++)
    	{
    		os << "{" << t.image[i][j] << "} ";
    	}
    	os << "]" << std::endl;
    }

    return os;
}