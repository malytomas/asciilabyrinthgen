#include "labgen.h"

#include <cage-core/math.h>
#include <cage-core/pointerRangeHolder.h>

#include <initializer_list>

namespace
{
	enum class Cell : char
	{
		None = 0, // outside of playable map
		Empty = 1, // walkable
		Wall = 2,
		Tmp = 3,
	};

	struct Generator
	{
		std::vector<Cell> cells;
		uint32 width = 0, height = 0;

		void generate()
		{
			rooms();
			corridors();
		}

		Cell &cell(ivec2 p)
		{
			CAGE_ASSERT(p[0] >= 0 && (uint32)p[0] < width && p[1] >= 0 && (uint32)p[1] < height);
			return cells[p[1] * width + p[0]];
		}

	private:
		uint32 countCells(Cell cell) const
		{
			uint32 cnt = 0;
			for (Cell c : cells)
				if (c == cell)
					cnt++;
			return cnt;
		}

		void rectReplace(ivec2 a, ivec2 b, Cell what, Cell with)
		{
			for (sint32 y = a[1]; y < b[1]; y++)
			{
				for (sint32 x = a[0]; x < b[0]; x++)
				{
					Cell &c = cell(ivec2(x, y));
					if (c == what)
						c = with;
				}
			}
		}

		ivec2 findAny(Cell c)
		{
			for (uint32 i = 0; i < cells.size(); i++)
				if (cells[i] == c)
					return ivec2(i % width, i / width);
			return ivec2(-1);
		}

		void seedReplace(ivec2 seed, Cell what, Cell with)
		{
			CAGE_ASSERT(what != with);
			std::vector<ivec2> q;
			q.push_back(seed);
			while (!q.empty())
			{
				const ivec2 p = q.back();
				q.pop_back();
				Cell &c = cell(p);
				if (c != what)
					continue;
				c = with;
				for (ivec2 it : { p + ivec2(-1, 0), p + ivec2(1, 0), p + ivec2(0, -1), p + ivec2(0, 1) })
					if (cell(it) == what)
						q.push_back(it);
			}
		}

		bool isConnected()
		{
			seedReplace(findAny(Cell::Empty), Cell::Empty, Cell::Tmp);
			bool res = countCells(Cell::Empty) == 0;
			rectReplace(ivec2(), ivec2(width, height), Cell::Tmp, Cell::Empty); // restore back
			return res;
		}

		void rooms()
		{
			CAGE_LOG(SeverityEnum::Info, "generate", "generating rooms");

			while (true)
			{
				width = randomRange(60, 70);
				height = randomRange(30, 40);
				cells.clear();
				cells.resize(width * height, Cell::None);

				// generate random rectangular rooms
				while (countCells(Cell::Empty) < width * height * 3 / 5)
				{
					constexpr uint32 S = 6;
					ivec2 a = ivec2(randomRange(1u, width - S), randomRange(1u, height - S));
					ivec2 b = a + randomRange2i(3, S + 1);
					rectReplace(a, b, Cell::None, Cell::Empty);
				}

				// find outline
				for (uint32 y = 1; y < height - 1; y++)
				{
					for (uint32 x = 1; x < width; x++)
					{
						const ivec2 p = ivec2(x, y);
						Cell &c = cell(p);
						if (c != Cell::Empty)
							continue;
						uint32 neighs = 0;
						for (sint32 j = -1; j < 2; j++)
							for (sint32 i = -1; i < 2; i++)
								neighs += cell(p + ivec2(i, j)) == Cell::None;
						if (neighs > 0)
							c = Cell::Wall;
					}
				}

				// detect disconnected areas
				if (isConnected())
					break;
			}
		}

		void corridors()
		{
			CAGE_LOG(SeverityEnum::Info, "generate", "generating corridors");

			for (uint32 round = 0; round < 1000000; round++)
			{
				const ivec2 p = ivec2(randomRange(2u, width - 2), randomRange(2u, height - 2));
				Cell &c = cell(p);
				if (c != Cell::Empty)
					continue;
				bool neighs[8];
				{
					bool *n = neighs;
					for (ivec2 it : { ivec2(-1, 0), ivec2(-1, -1), ivec2(0, -1), ivec2(1, -1), ivec2(1, 0), ivec2(1, 1), ivec2(0, 1), ivec2(-1, 1) })
						*n++ = cell(p + it) == Cell::Wall;
				}
				if (!neighs[0] && !neighs[2] && !neighs[4] && !neighs[6])
					continue; // no 4-connected neighbor
				uint32 sum = 0;
				for (bool b : neighs)
					sum += b;
				if (sum > 2)
					continue;
				uint32 changes = 0;
				for (uint32 i = 0; i < 8; i++)
					changes += neighs[i] != neighs[(i + 1) % 8];
				if (changes != 2)
					continue;
				c = Cell::Wall;
			}

			CAGE_ASSERT(isConnected());
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
