#include <cage-core/core.h>

using namespace cage;

struct Labyrinth
{
	Holder<PointerRange<char>> cells;
	uint32 width = 0, height = 0;

	char cell(uint32 x, uint32 y) const
	{
		return cells[y * width + x];
	}
};

Labyrinth generate();
void paths(Labyrinth &lab);
void outputs(const Labyrinth &lab);


