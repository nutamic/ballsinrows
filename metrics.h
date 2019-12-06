#ifndef METRICS_H
#define METRICS_H
struct Metrics{
	float fpBase;
	short base, object, field;
	short margin, bottomBase, rightObject;
	short columnLeft, columnCenter, columnRight;
	short rowCenter;
	Metrics(const short width, const short height);
};
#endif
