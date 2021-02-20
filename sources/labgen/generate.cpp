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
		}

		Cell &cell(uint32 x, uint32 y)
		{
			return cells[y * width + x];
		}

	private:
		std::vector<Connection> walls;

		void init()
		{
			width = randomRange(60, 70);
			height = randomRange(30, 40);
			cells.resize(width * height, Cell::None);
		}

		uint32 countCells(Cell cell) const
		{
			uint32 cnt = 0;
			for (Cell c : cells)
				if (c == cell)
					cnt++;
			return cnt;
		}

		void replace(uint32 x1, uint32 y1, uint32 x2, uint32 y2, Cell what, Cell with)
		{
			for (uint32 y = y1; y < y2; y++)
			{
				for (uint32 x = x1; x < x2; x++)
				{
					Cell &c = cell(x, y);
					if (c == what)
						c = with;
				}
			}
		}

		void room(uint32 x1, uint32 y1, uint32 x2, uint32 y2)
		{
			CAGE_ASSERT(x1 > 0);
			CAGE_ASSERT(y1 > 0);
			CAGE_ASSERT(x2 > x1);
			CAGE_ASSERT(y2 > y1);
			CAGE_ASSERT(x2 < width);
			CAGE_ASSERT(y2 < height);
			replace(x1, y1, x2, y2, Cell::None, Cell::Wall);
			replace(x1 + 1, y1 + 1, x2 - 1, y2 - 1, Cell::Wall, Cell::Empty);
		}

		void rooms()
		{
			while (countCells(Cell::Empty) < width * height / 3)
			{
				constexpr uint32 S = 15;
				uint32 a = randomRange(1u, width - S);
				uint32 b = randomRange(1u, height - S);
				room(a, b, a + randomRange(3u, S + 1), b + randomRange(3u, S + 1));
			}
		}
	};
}

Labyrinth generate()
{
	CAGE_LOG(SeverityEnum::Info, "generate", "generating the labyrinth");
	Generator gen;
	gen.generate();
	Labyrinth lab;
	lab.width = gen.width;
	lab.height = gen.height;
	std::vector<char> cells = (std::vector<char> &&)templates::move(gen.cells);
	lab.cells = PointerRangeHolder<char>(templates::move(cells));
	return lab;
}
