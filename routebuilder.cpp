#include "routebuilder.h"
bool RouteBuilder::performStep(short into, short step){
	if(into == dest) return true;
	if(field[into] == FREE){
		field[into] = char(step);
		visited.push(into);
	}
	return false;
}
bool RouteBuilder::build(){
	while(!visited.empty()){
		const short index = visited.front();
		visited.pop();
		const char x = index % SIDE, y = char(index) / SIDE;
		const short step = field[index] + 1;
		if(x){
			if(performStep(index - 1, step)) return true;
		}
		if(x != SIDE - 1){
			if(performStep(index + 1, step)) return true;
		}
		if(y){
			if(performStep(index - SIDE, step)) return true;
		}
		if(y != SIDE - 1){
			if(performStep(index + SIDE, step)) return true;
		}
	}
	return false;
}
