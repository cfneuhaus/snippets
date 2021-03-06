#include "MarchingSquares.h"

namespace
{
Eigen::Vector2d lerpVertex(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, double v1, double v2, double isovalue)
{
	double mu = (isovalue - v1)/(v2 - v1);
	return p1+mu*(p2-p1);
}

struct GridCell
{
	Eigen::Vector2d vertex[4];
	double value[4];
};

void genGeometry(const GridCell& gridCell, std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign> > &line_segments, std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign>  >& triangles, double isovalue)
{
	// Identify which of the 4 corners of the square are within the isosurface.
	// Max 16 cases. Only 14 cases produce triangles and image edge line segments.

	// Corner vertex order: 03
	//                      12
	// e.g.: clockwise, as in OpenGL

	unsigned short int mask = 0;

	if(gridCell.value[0] >= isovalue)
		mask |= 1;
	if(gridCell.value[1] >= isovalue)
		mask |= 2;
	if(gridCell.value[2] >= isovalue)
		mask |= 4;
	if(gridCell.value[3] >= isovalue)
		mask |= 8;

	switch(mask)
	{
	case 1:
	{
		//1:
		//10
		//00
		Eigen::Vector2d a = gridCell.vertex[0];
		Eigen::Vector2d b = lerpVertex(gridCell.vertex[0], gridCell.vertex[1], gridCell.value[0], gridCell.value[1], isovalue);
		Eigen::Vector2d c = lerpVertex(gridCell.vertex[0], gridCell.vertex[3], gridCell.value[0], gridCell.value[3], isovalue);
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
		Eigen::Vector2d a = lerpVertex(gridCell.vertex[1], gridCell.vertex[0], gridCell.value[1], gridCell.value[0], isovalue);
		Eigen::Vector2d b = gridCell.vertex[1];
		Eigen::Vector2d c = lerpVertex(gridCell.vertex[1], gridCell.vertex[2], gridCell.value[1], gridCell.value[2], isovalue);
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
		Eigen::Vector2d a = lerpVertex(gridCell.vertex[2], gridCell.vertex[1], gridCell.value[2], gridCell.value[1], isovalue);
		Eigen::Vector2d b = gridCell.vertex[2];
		Eigen::Vector2d c = lerpVertex(gridCell.vertex[2], gridCell.vertex[3], gridCell.value[2], gridCell.value[3], isovalue);
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
		Eigen::Vector2d a = lerpVertex(gridCell.vertex[3], gridCell.vertex[0], gridCell.value[3], gridCell.value[0], isovalue);
		Eigen::Vector2d b = lerpVertex(gridCell.vertex[3], gridCell.vertex[2], gridCell.value[3], gridCell.value[2], isovalue);
		Eigen::Vector2d c = gridCell.vertex[3];
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
		Eigen::Vector2d a = gridCell.vertex[0];
		Eigen::Vector2d b = gridCell.vertex[1];
		Eigen::Vector2d c = lerpVertex(gridCell.vertex[1], gridCell.vertex[2], gridCell.value[1], gridCell.value[2], isovalue);
		Eigen::Vector2d d = lerpVertex(gridCell.vertex[0], gridCell.vertex[3], gridCell.value[0], gridCell.value[3], isovalue);

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
		Eigen::Vector2d a = lerpVertex(gridCell.vertex[1], gridCell.vertex[0], gridCell.value[1], gridCell.value[0], isovalue);
		Eigen::Vector2d b = gridCell.vertex[1];
		Eigen::Vector2d c = gridCell.vertex[2];
		Eigen::Vector2d d = lerpVertex(gridCell.vertex[2], gridCell.vertex[3], gridCell.value[2], gridCell.value[3], isovalue);

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
		Eigen::Vector2d a = gridCell.vertex[0];
		Eigen::Vector2d b = lerpVertex(gridCell.vertex[0], gridCell.vertex[1], gridCell.value[0], gridCell.value[1], isovalue);
		Eigen::Vector2d c = lerpVertex(gridCell.vertex[3], gridCell.vertex[2], gridCell.value[3], gridCell.value[2], isovalue);
		Eigen::Vector2d d = gridCell.vertex[3];

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

		Eigen::Vector2d a = lerpVertex(gridCell.vertex[3], gridCell.vertex[0], gridCell.value[3], gridCell.value[0], isovalue);
		Eigen::Vector2d b = lerpVertex(gridCell.vertex[2], gridCell.vertex[1], gridCell.value[2], gridCell.value[1], isovalue);
		Eigen::Vector2d c = gridCell.vertex[2];
		Eigen::Vector2d d = gridCell.vertex[3];

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

		Eigen::Vector2d a = gridCell.vertex[0];
		Eigen::Vector2d b = lerpVertex(gridCell.vertex[0], gridCell.vertex[1], gridCell.value[0], gridCell.value[1], isovalue);
		Eigen::Vector2d c = lerpVertex(gridCell.vertex[0], gridCell.vertex[3], gridCell.value[0], gridCell.value[3], isovalue);
		Eigen::Vector2d d = lerpVertex(gridCell.vertex[2], gridCell.vertex[1], gridCell.value[2], gridCell.value[1], isovalue);
		Eigen::Vector2d e = gridCell.vertex[2];
		Eigen::Vector2d f = lerpVertex(gridCell.vertex[2], gridCell.vertex[3], gridCell.value[2], gridCell.value[3], isovalue);

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

		Eigen::Vector2d a = lerpVertex(gridCell.vertex[1], gridCell.vertex[0], gridCell.value[1], gridCell.value[0], isovalue);
		Eigen::Vector2d b = gridCell.vertex[1];
		Eigen::Vector2d c = lerpVertex(gridCell.vertex[1], gridCell.vertex[2], gridCell.value[1], gridCell.value[2], isovalue);
		Eigen::Vector2d d = lerpVertex(gridCell.vertex[3], gridCell.vertex[0], gridCell.value[3], gridCell.value[0], isovalue);
		Eigen::Vector2d e = lerpVertex(gridCell.vertex[3], gridCell.vertex[2], gridCell.value[3], gridCell.value[2], isovalue);
		Eigen::Vector2d f = gridCell.vertex[3];

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

		Eigen::Vector2d a = gridCell.vertex[0];
		Eigen::Vector2d b = gridCell.vertex[1];
		Eigen::Vector2d c = gridCell.vertex[2];
		Eigen::Vector2d d = lerpVertex(gridCell.vertex[2], gridCell.vertex[3], gridCell.value[2], gridCell.value[3], isovalue);
		Eigen::Vector2d e = lerpVertex(gridCell.vertex[0], gridCell.vertex[3], gridCell.value[0], gridCell.value[3], isovalue);

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

		Eigen::Vector2d a = gridCell.vertex[0];
		Eigen::Vector2d b = gridCell.vertex[1];
		Eigen::Vector2d c = lerpVertex(gridCell.vertex[1], gridCell.vertex[2], gridCell.value[1], gridCell.value[2], isovalue);
		Eigen::Vector2d d = lerpVertex(gridCell.vertex[3], gridCell.vertex[2], gridCell.value[3], gridCell.value[2], isovalue);
		Eigen::Vector2d e = gridCell.vertex[3];

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

		Eigen::Vector2d a = gridCell.vertex[0];
		Eigen::Vector2d b = lerpVertex(gridCell.vertex[0], gridCell.vertex[1], gridCell.value[0], gridCell.value[1], isovalue);
		Eigen::Vector2d c = lerpVertex(gridCell.vertex[2], gridCell.vertex[1], gridCell.value[2], gridCell.value[1], isovalue);
		Eigen::Vector2d d = gridCell.vertex[2];
		Eigen::Vector2d e = gridCell.vertex[3];

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

		Eigen::Vector2d a = lerpVertex(gridCell.vertex[1], gridCell.vertex[0], gridCell.value[1], gridCell.value[0], isovalue);
		Eigen::Vector2d b = gridCell.vertex[1];
		Eigen::Vector2d c = gridCell.vertex[2];
		Eigen::Vector2d d = gridCell.vertex[3];
		Eigen::Vector2d e = lerpVertex(gridCell.vertex[3], gridCell.vertex[0], gridCell.value[3], gridCell.value[0], isovalue);

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

		triangles.push_back(gridCell.vertex[0]);
		triangles.push_back(gridCell.vertex[1]);
		triangles.push_back(gridCell.vertex[3]);

		triangles.push_back(gridCell.vertex[3]);
		triangles.push_back(gridCell.vertex[1]);
		triangles.push_back(gridCell.vertex[2]);

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

}


void marchingSquares(const Eigen::AlignedBox2d& bounds, double cellSize, const std::function<double (const Eigen::Vector2d& p)>& computeIso, double isoValue, std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign> > &line_segments, std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign>  >& triangles)
{
	for (double x=bounds.min().x();x<bounds.max().x();x+=cellSize)
	{
		for (double y=bounds.min().y();y<bounds.max().y();y+=cellSize)
		{
			GridCell gc;
			gc.vertex[0]=Eigen::Vector2d(x,y);
			gc.vertex[1]=Eigen::Vector2d(x+cellSize,y);
			gc.vertex[2]=Eigen::Vector2d(x+cellSize,y+cellSize);
			gc.vertex[3]=Eigen::Vector2d(x,y+cellSize);
			for (int i=0;i<4;i++)
				gc.value[i]=computeIso(gc.vertex[i]);
			genGeometry(gc,line_segments,triangles,isoValue);
		}
	}
}
