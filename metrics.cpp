#include "metrics.h"
Metrics::Metrics(const short width, const short height){
	fpBase = width / 16.0F;
	float verticalBase = height / 12.0F;
	if(verticalBase < fpBase) fpBase = verticalBase;
	base = short(fpBase);
	object = short(fpBase * 3);
	field = short(fpBase * 9);
	margin = short(fpBase / 4);
	bottomBase = height - base - margin;
	rightObject = width - object - margin;
	columnLeft = (width - field) / 2;
	columnCenter = columnLeft + object;
	columnRight = columnCenter + object;
	rowCenter = (height - field) / 2;
}
