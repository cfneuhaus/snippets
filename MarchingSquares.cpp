#include "MarchingSquares.h"

namespace
{
Eigen::Vector2d lerpVertex(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, double v1, double v2, double isovalue)
{
	double mu = (isovalue - v1)/(v2 - v1);
	return p1+mu*(p2-p1);
}

class Square
{
public:
	Eigen::Vector2d vertex[4];
	double value[4];

	void genGeometry(std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign> > &line_segments, std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign>  >& triangles, double isovalue)
	{
		// Identify which of the 4 corners of the square are within the isosurface.
		// Max 16 cases. Only 14 cases produce triangles and image edge line segments.

		// Corner vertex order: 03
		//                      12
		// e.g.: clockwise, as in OpenGL

		unsigned short int mask = 0;

		if(value[0] >= isovalue)
			mask |= 1;
		if(value[1] >= isovalue)
			mask |= 2;
		if(value[2] >= isovalue)
			mask |= 4;
		if(value[3] >= isovalue)
			mask |= 8;

		// Handle the 16 cases manually.
		switch(mask)
		{
		case 1:
		{
			//1:
			//10
			//00
			Eigen::Vector2d a = vertex[0];
			Eigen::Vector2d b = lerpVertex(vertex[0], vertex[1], value[0], value[1], isovalue);
			Eigen::Vector2d c = lerpVertex(vertex[0], vertex[3], value[0], value[3], isovalue);
			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(c);
			line_segments.push_back(b);
			line_segments.push_back(c);
			break;
		}
		case 2:
		{
			//2:
			//00
			//10
			Eigen::Vector2d a = lerpVertex(vertex[1], vertex[0], value[1], value[0], isovalue);
			Eigen::Vector2d b = vertex[1];
			Eigen::Vector2d c = lerpVertex(vertex[1], vertex[2], value[1], value[2], isovalue);
			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(c);
			line_segments.push_back(a);
			line_segments.push_back(c);
			break;
		}
		case 4:
		{
			//4:
			//00
			//01
			Eigen::Vector2d a = lerpVertex(vertex[2], vertex[1], value[2], value[1], isovalue);
			Eigen::Vector2d b = vertex[2];
			Eigen::Vector2d c = lerpVertex(vertex[2], vertex[3], value[2], value[3], isovalue);
			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(c);
			line_segments.push_back(a);
			line_segments.push_back(c);
			break;
		}
		case 8:
		{
			//8:
			//01
			//00
			Eigen::Vector2d a = lerpVertex(vertex[3], vertex[0], value[3], value[0], isovalue);
			Eigen::Vector2d b = lerpVertex(vertex[3], vertex[2], value[3], value[2], isovalue);
			Eigen::Vector2d c = vertex[3];
			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(c);
			line_segments.push_back(a);
			line_segments.push_back(b);
			break;
		}


		case 3:
		{
			//3:
			//10
			//10
			Eigen::Vector2d a = vertex[0];
			Eigen::Vector2d b = vertex[1];
			Eigen::Vector2d c = lerpVertex(vertex[1], vertex[2], value[1], value[2], isovalue);
			Eigen::Vector2d d = lerpVertex(vertex[0], vertex[3], value[0], value[3], isovalue);

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(d);

			triangles.push_back(d);
			triangles.push_back(b);
			triangles.push_back(c);

			line_segments.push_back(d);
			line_segments.push_back(c);
			break;
		}
		case 6:
		{
			//6:
			//00
			//11
			Eigen::Vector2d a = lerpVertex(vertex[1], vertex[0], value[1], value[0], isovalue);
			Eigen::Vector2d b = vertex[1];
			Eigen::Vector2d c = vertex[2];
			Eigen::Vector2d d = lerpVertex(vertex[2], vertex[3], value[2], value[3], isovalue);

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(d);

			triangles.push_back(d);
			triangles.push_back(b);
			triangles.push_back(c);

			line_segments.push_back(a);
			line_segments.push_back(d);
			break;
		}
		case 9:
		{
			//9:
			//11
			//00
			Eigen::Vector2d a = vertex[0];
			Eigen::Vector2d b = lerpVertex(vertex[0], vertex[1], value[0], value[1], isovalue);
			Eigen::Vector2d c = lerpVertex(vertex[3], vertex[2], value[3], value[2], isovalue);
			Eigen::Vector2d d = vertex[3];

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(d);

			triangles.push_back(d);
			triangles.push_back(b);
			triangles.push_back(c);

			line_segments.push_back(b);
			line_segments.push_back(c);
			break;
		}
		case 12:
		{
			//12:
			//01
			//01

			Eigen::Vector2d a = lerpVertex(vertex[3], vertex[0], value[3], value[0], isovalue);
			Eigen::Vector2d b = lerpVertex(vertex[2], vertex[1], value[2], value[1], isovalue);
			Eigen::Vector2d c = vertex[2];
			Eigen::Vector2d d = vertex[3];

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(d);

			triangles.push_back(d);
			triangles.push_back(b);
			triangles.push_back(c);

			line_segments.push_back(a);
			line_segments.push_back(b);

			break;
		}


		case 5:
		{
			//5:
			//10
			//01

			Eigen::Vector2d a = vertex[0];
			Eigen::Vector2d b = lerpVertex(vertex[0], vertex[1], value[0], value[1], isovalue);
			Eigen::Vector2d c = lerpVertex(vertex[0], vertex[3], value[0], value[3], isovalue);
			Eigen::Vector2d d = lerpVertex(vertex[2], vertex[1], value[2], value[1], isovalue);
			Eigen::Vector2d e = vertex[2];
			Eigen::Vector2d f = lerpVertex(vertex[2], vertex[3], value[2], value[3], isovalue);

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(c);

			triangles.push_back(d);
			triangles.push_back(e);
			triangles.push_back(f);

			line_segments.push_back(b);
			line_segments.push_back(c);
			line_segments.push_back(d);
			line_segments.push_back(f);
			break;
		}
		case 10:
		{
			//10:
			//01
			//10

			Eigen::Vector2d a = lerpVertex(vertex[1], vertex[0], value[1], value[0], isovalue);
			Eigen::Vector2d b = vertex[1];
			Eigen::Vector2d c = lerpVertex(vertex[1], vertex[2], value[1], value[2], isovalue);
			Eigen::Vector2d d = lerpVertex(vertex[3], vertex[0], value[3], value[0], isovalue);
			Eigen::Vector2d e = lerpVertex(vertex[3], vertex[2], value[3], value[2], isovalue);
			Eigen::Vector2d f = vertex[3];

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(c);

			triangles.push_back(d);
			triangles.push_back(e);
			triangles.push_back(f);

			line_segments.push_back(a);
			line_segments.push_back(c);
			line_segments.push_back(d);
			line_segments.push_back(e);
			break;
		}


		case 7:
		{
			//7:
			//10
			//11

			Eigen::Vector2d a = vertex[0];
			Eigen::Vector2d b = vertex[1];
			Eigen::Vector2d c = vertex[2];
			Eigen::Vector2d d = lerpVertex(vertex[2], vertex[3], value[2], value[3], isovalue);
			Eigen::Vector2d e = lerpVertex(vertex[0], vertex[3], value[0], value[3], isovalue);

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(e);

			triangles.push_back(e);
			triangles.push_back(b);
			triangles.push_back(d);

			triangles.push_back(d);
			triangles.push_back(b);
			triangles.push_back(c);

			line_segments.push_back(e);
			line_segments.push_back(d);
			break;
		}
		case 11:
		{
			//11:
			//11
			//10

			Eigen::Vector2d a = vertex[0];
			Eigen::Vector2d b = vertex[1];
			Eigen::Vector2d c = lerpVertex(vertex[1], vertex[2], value[1], value[2], isovalue);
			Eigen::Vector2d d = lerpVertex(vertex[3], vertex[2], value[3], value[2], isovalue);
			Eigen::Vector2d e = vertex[3];

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(c);

			triangles.push_back(a);
			triangles.push_back(c);
			triangles.push_back(d);

			triangles.push_back(a);
			triangles.push_back(d);
			triangles.push_back(e);

			line_segments.push_back(c);
			line_segments.push_back(d);
			break;
		}
		case 13:
		{
			//13:
			//11
			//01

			Eigen::Vector2d a = vertex[0];
			Eigen::Vector2d b = lerpVertex(vertex[0], vertex[1], value[0], value[1], isovalue);
			Eigen::Vector2d c = lerpVertex(vertex[2], vertex[1], value[2], value[1], isovalue);
			Eigen::Vector2d d = vertex[2];
			Eigen::Vector2d e = vertex[3];

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(e);

			triangles.push_back(e);
			triangles.push_back(b);
			triangles.push_back(c);

			triangles.push_back(e);
			triangles.push_back(c);
			triangles.push_back(d);

			line_segments.push_back(b);
			line_segments.push_back(c);
			break;
		}
		case 14:
		{
			//14:
			//01
			//11

			Eigen::Vector2d a = lerpVertex(vertex[1], vertex[0], value[1], value[0], isovalue);
			Eigen::Vector2d b = vertex[1];
			Eigen::Vector2d c = vertex[2];
			Eigen::Vector2d d = vertex[3];
			Eigen::Vector2d e = lerpVertex(vertex[3], vertex[0], value[3], value[0], isovalue);

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(c);

			triangles.push_back(a);
			triangles.push_back(c);
			triangles.push_back(e);

			triangles.push_back(e);
			triangles.push_back(c);
			triangles.push_back(d);

			line_segments.push_back(a);
			line_segments.push_back(e);
			break;
		}

		case 15:
		{
			// Case 15 (all inside of image area) produces no outlines.

			//15:
			//11
			//11

			triangles.push_back(vertex[0]);
			triangles.push_back(vertex[1]);
			triangles.push_back(vertex[3]);

			triangles.push_back(vertex[3]);
			triangles.push_back(vertex[1]);
			triangles.push_back(vertex[2]);

			break;
		}


		default:
		{
			// Case 0 (all outside of image area) produces no geometry.

			//0:
			//00
			//00

			break;
		}
		}
	}
};
}


void marchingSquares(const Eigen::AlignedBox2d& bounds, double cellSize, const std::function<double (const Eigen::Vector2d& p)>& computeIso, double isoValue, std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign> > &line_segments, std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign>  >& triangles)
{
	for (double x=bounds.min().x();x<bounds.max().x();x+=cellSize)
	{
		for (double y=bounds.min().y();y<bounds.max().y();y+=cellSize)
		{
			Square gs;
			gs.vertex[0]=Eigen::Vector2d(x,y);
			gs.vertex[1]=Eigen::Vector2d(x+cellSize,y);
			gs.vertex[2]=Eigen::Vector2d(x+cellSize,y+cellSize);
			gs.vertex[3]=Eigen::Vector2d(x,y+cellSize);
			for (int i=0;i<4;i++)
				gs.value[i]=computeIso(gs.vertex[i]);
			gs.genGeometry(line_segments,triangles,isoValue);
		}
	}
}
