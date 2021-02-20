#include "labgen.h"

#include <vector>
#include <utility> // std::swap

namespace
{
	struct Paths
	{
		Labyrinth &lab;

		std::vector<uint32> dists;
		std::vector<ivec2> prevs;

		Paths(Labyrinth &lab) : lab(lab)
		{}

		Cell &cell(ivec2 p)
		{
			CAGE_ASSERT(p[0] >= 0 && (uint32)p[0] < lab.width && p[1] >= 0 && (uint32)p[1] < lab.height);
			return lab.cells[p[1] * lab.width + p[0]];
		}

		bool walkable(ivec2 p)
		{
			switch (cell(p))
			{
			case Cell::Empty:
			case Cell::Start:
			case Cell::Goal:
			case Cell::Path:
				return true;
			default:
				return false;
			}
		}

		uint32 &dist(ivec2 p)
		{
			return dists[p[1] * lab.width + p[0]];
		}

		ivec2 &prev(ivec2 p)
		{
			return prevs[p[1] * lab.width + p[0]];
		}

		void distances(ivec2 start)
		{
			CAGE_ASSERT(walkable(start));

			dists.clear();
			prevs.clear();
			dists.resize(lab.cells.size(), m);
			prevs.resize(lab.cells.size(), ivec2(-1));

			std::vector<ivec2> q;
			q.push_back(start);
			dist(start) = 0;
			while (!q.empty())
			{
				const ivec2 p = q.front();
				q.erase(q.begin());
				const uint32 pd = dist(p);
				CAGE_ASSERT(pd != m);
				CAGE_ASSERT(walkable(p));
				for (ivec2 it : { p + ivec2(-1, 0), p + ivec2(1, 0), p + ivec2(0, -1), p + ivec2(0, 1) })
				{
					if (!walkable(it))
						continue;
					if (dist(it) <= pd + 1)
						continue;
					dist(it) = pd + 1;
					prev(it) = p;
					q.push_back(it);
				}
			}
		}

		ivec2 pickRandom()
		{
			while (true)
			{
				const ivec2 p = ivec2(randomRange(1u, lab.width - 1), randomRange(1u, lab.height - 1));
				if (walkable(p))
					return p;
			}
		}

		ivec2 pickFurthest()
		{
			uint32 d = 0;
			ivec2 p = ivec2(-1);
			for (uint32 i = 0; i < dists.size(); i++)
			{
				uint32 k = dists[i];
				if (k == m)
					continue;
				if (k > d)
				{
					d = k;
					p = ivec2(i % lab.width, i / lab.width);
				}
			}
			return p;
		}

		void markPath(ivec2 start, ivec2 goal)
		{
			uint32 cnt = 1;
			start = prev(start);
			while (start != goal)
			{
				cell(start) = Cell::Path;
				start = prev(start);
				cnt++;
			}
			CAGE_LOG(SeverityEnum::Info, "paths", stringizer() + "the path has " + cnt + " steps");
		}

		void paths()
		{
			CAGE_LOG(SeverityEnum::Info, "paths", "determining start and goal positions");
			distances(pickRandom());
			ivec2 start = pickFurthest();
			distances(start);
			ivec2 goal = pickFurthest();
			std::swap(start, goal);
			cell(start) = Cell::Start;
			cell(goal) = Cell::Goal;
			markPath(start, goal);
		}
	};
}

void paths(Labyrinth &lab)
{
	Paths paths(lab);
	paths.paths();
}
