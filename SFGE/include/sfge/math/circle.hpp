#ifndef SFGE_MATH_SOLVERS_HPP
#define SFGE_MATH_SOLVERS_HPP

#include <SFML/System/Vector2.hpp>

namespace sfge
{
	template <typename VectorStorageT>
	struct Circle
	{
        typedef VectorStorageT                  type_precision;
		typedef Circle<type_precision>			self_type;
		typedef sf::Vector2<type_precision>		vector_type;

		Circle() : mRadius(0)														{ ; }
		Circle(const vector_type &p, float r) : mPos(p), mRadius(r)					{ ; }
		Circle(const self_type &other) : mPos(other.mPos), mRadius(other.mRadius)	{ ; }

		vector_type		mPos;
		type_precision	mRadius;
	};
}

#endif
