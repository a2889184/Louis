#include "Common.h"
#include "Math/Vector3f.h"

std::ostream& operator<<(std::ostream &stream, const math::Vector3f &r)
{
	return stream << r.toStringFormal();
}