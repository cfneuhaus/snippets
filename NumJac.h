#ifndef NUMJAC_H
#define NUMJAC_H

#include <Eigen/Core>
#include <stdexcept>

template<typename T, typename P>
auto numJacobian(T fn, const P& x) -> Eigen::Matrix<double,decltype(fn(x))::RowsAtCompileTime,P::RowsAtCompileTime>
{
	const int Rows=decltype(fn(x))::RowsAtCompileTime;
	Eigen::Matrix<double,Rows,P::RowsAtCompileTime> jac;

	const double eps=0.0001;
	auto xh=x;
	for (int i=0;i<x.rows();i++)
	{
		xh(i)=x(i)+eps;
		const auto a=fn(xh);
		xh(i)=x(i)-eps;
		const auto b=fn(xh);
		xh(i)=x(i);

		if (Rows==Eigen::Dynamic)
		{
			if (i==0)
				jac.resize(a.rows(),x.rows());
			if ((a.rows()!=jac.rows())||(b.rows()!=jac.rows()))
				throw std::runtime_error("Dynamic Dimensions do not match!");
		}

		jac.col(i)=(a-b)/(2.0*eps);
	}
	return jac;
}

#endif
