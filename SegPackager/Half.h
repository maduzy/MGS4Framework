#ifndef HALF_H
#define HALF_H

#include <math.h>

class Half {
public:
	Half() {}
	Half(const double d) { load(d); }
	operator double() const { return _d.i + ((double)_d.f / 1000); }
private:
	struct Data {
		unsigned short i : 6;
		unsigned short f : 10;
	};
	void load(const double d) {
		int i = d;
		_d.i = i;
		_d.f = round((d - i) * 1000);
	}
	Data _d;
};

#endif //HALF_H