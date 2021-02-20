#include <cage-core/math.h>

using namespace cage;

enum class Cell : char
{
	None = 0, // outside of playable map
	Wall = 1,
	Empty = 2, // walkable
	Start = 3,
	Goal = 4,
	Path = 5,
	Tmp = (char)m,
};

struct Labyrinth
{
	Holder<PointerRange<Cell>> cells;
	uint32 width = 0, height = 0;

	Cell &cell(ivec2 p)
	{
		CAGE_ASSERT(p[0] >= 0 && (uint32)p[0] < width && p[1] >= 0 && (uint32)p[1] < height);
		return cells[p[1] * width + p[0]];
	}
};

Labyrinth generate();
void paths(Labyrinth &lab);
void outputs(Labyrinth &lab);


