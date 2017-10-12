#include <glm/glm.hpp>
using namespace glm;
class Bounds {
public:
	Bounds(vec3 min, vec3 max) {
		_max = max;
		_min = min;
	}

	Bounds() {
		_max = vec3(0.0f);
		_min = vec3(0.0f);
	}


	vec3 getCenter() { return 0.5f * (_max + _min); }
	vec3 getMax() { return _max; }
	vec3 getMin() { return _min; }

	vec3 getEntents() { return 0.5f * (_max - _min); };
	vec3 getSize() { return _max - _min; };



	Bounds(Bounds& other) {
		_max = other.getMax();
		_min = other.getMin();
	}

	Bounds & Bounds :: operator= (Bounds & other) {
		_max = other.getMax();
		_min = other.getMin();
		return *this;
	}

	//std::ostream& operator<< (ostream& os, const Bounds& bounds);
	
private:
	vec3 _max;
	vec3 _min;

};

//
//std::ostream& Bounds::operator<< (ostream& os, const Bounds& bounds) {
//	os << "Bounds: max(" << bounds.getMax[0] << ", " << bounds.getMax[1] << ", " << bounds.getMax[2] << "), ";
//	os << "min(" << bounds.getMin[0] << ", " << bounds.getMin[1] << ", " << bounds.getMin[2] << ")." << endl;
//	return os;
//}