#ifndef MARCHING_SQUARES_H
#define MARCHING_SQUARES_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <functional>

void marchingSquares(const Eigen::AlignedBox2d& bounds,
					 double cellSize,
					 const std::function<double (const Eigen::Vector2d& p)>& computeIso,
					 double isoValue,
					 std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign> >& line_segments,
					 std::vector<Eigen::Matrix<double,2,1,Eigen::DontAlign>  >& triangles);

#endif
