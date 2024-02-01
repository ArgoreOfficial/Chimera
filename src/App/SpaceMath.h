#pragma once

#include <Core/Math/Vector3.h>

namespace SpaceMath
{
	/* formula derived from https://beltoforion.de/en/barnes-hut-galaxy-simulator/ */
	static wv::cVector3d computeForce( wv::cVector3d& _1p, wv::cVector3d& _2p, double& _1m, double& _2m )
	{
		double mm = _1m * _2m;
		wv::cVector3d rirj = _1p - _2p;
		double magnitude = rirj.length();
		return ( rirj / pow( magnitude, 3 ) ) * mm;
	}
};