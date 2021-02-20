#include "labgen.h"

#include <cage-core/files.h>
#include <cage-core/string.h>
#include <cage-core/pointerRangeHolder.h>

namespace
{
	char connectedWall(uint32 neighbors)
	{
		switch (neighbors)
		{
		case 0: return '+';
		case 1: return 196;
		case 2: return 179;
		case 3: return 217;
		case 4: return 196;
		case 5: return 196;
		case 6: return 192;
		case 7: return 193;
		case 8: return 179;
		case 9: return 191;
		case 10: return 179;
		case 11: return 180;
		case 12: return 218;
		case 13: return 194;
		case 14: return 195;
		case 15: return 197;
		default:
			CAGE_THROW_CRITICAL(Exception, "invalid connected walls code");
		}
	}

	string convertToAscii(const Labyrinth &lab, uint32 y)
	{
		string res;
		for (uint32 x = 1; x < lab.width - 1; x++)
		{
			if (lab.cell(x, y) == 2)
			{
				uint32 neighbors = 0;
				if (lab.cell(x - 1, y) == 2)
					neighbors += 1; // left
				if (lab.cell(x, y - 1) == 2)
					neighbors += 2; // top
				if (lab.cell(x + 1, y) == 2)
					neighbors += 4; // right
				if (lab.cell(x, y + 1) == 2)
					neighbors += 8; // bottom
				res += string(connectedWall(neighbors));
			}
			else
				res += " ";
		}
		return res;
	}
}

void outputs(const Labyrinth &lab)
{
	CAGE_LOG(SeverityEnum::Info, "output", "generating the outputs");

	Holder<File> p = writeFile("plain.txt");
	Holder<File> c = writeFile("cypher.txt");

	for (uint32 y = 1; y < lab.height - 1; y++)
	{
		string s = convertToAscii(lab, y);
		p->writeLine(s);
		CAGE_LOG_CONTINUE(SeverityEnum::Info, "layout", s);
		s = trim(s);
		c->write(s);
	}

	p->close();
	c->writeLine("");
	c->close();
}
