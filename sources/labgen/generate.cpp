#include "labgen.h"

#include <cage-core/math.h>
#include <cage-core/pointerRangeHolder.h>

namespace
{
	struct Connection
	{
		uint32 x1, y1, x2, y2;
	};

	enum class Cell : char
	{
		None = 0, // outside of playable map
		Empty = 1, // walkable
		Wall = 2,
	};

	struct Generator
	{
		std::vector<Cell> cells;
		uint32 width = 0, height = 0;

		void generate()
		{
			init();
			rooms();
			filling();
		}

		Cell &cell(uint32 x, uint32 y)
		{
			return cells[y * width + x];
		}

	private:
		std::vector<Connection> walls;

		void init()
		{
			width = randomRange(20, 40);
			height = randomRange(20, 40);
			cells.resize(width * height, Cell::None);
		}

		void rooms()
		{
			// todo
			// temporary random char generator
			for (uint32 y = 1; y < height - 1; y++)
			{
				for (uint32 x = 1; x < width - 1; x++)
				{
					if (randomChance() < 0.3)
						cell(x, y) = Cell::Wall;
				}
			}
		}

		void filling()
		{
			// todo
		}
	};
}

Labyrinth generate()
{
	Generator gen;
	gen.generate();
	Labyrinth lab;
	lab.width = gen.width;
	lab.height = gen.height;
	std::vector<char> cells = (std::vector<char> &&)templates::move(gen.cells);
	lab.cells = PointerRangeHolder<char>(templates::move(cells));
	return lab;
}
