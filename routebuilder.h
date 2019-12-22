#ifndef ROUTEBUILDER_H
#define ROUTEBUILDER_H
#include <queue>
#define SIDE 9
#define SIZE 81
#define FREE '\xff'
#define INVALID '\xfe'
class RouteBuilder{
	std::queue<short> visited;
	char *const field;
	const short dest;
	RouteBuilder(const RouteBuilder &) = delete;
	void operator =(const RouteBuilder &) = delete;
	inline bool performStep(short into, short step);
public:
	RouteBuilder(short from, short to, char *balls):
		field(new char[SIZE]), dest(to){
		char *const end = field + SIZE;
		for(char *cell = field; cell != end; cell++, balls++) *cell = *balls == FREE ? FREE : INVALID;
		field[from] = 0;
		visited.push(from);
	}
	~RouteBuilder(){
		delete[] field;
	}
	bool build();
};
#endif
