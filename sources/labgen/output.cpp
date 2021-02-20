#include "labgen.h"

#include <cage-core/files.h>
#include <cage-core/string.h>
#include <cage-core/pointerRangeHolder.h>

namespace
{
#ifdef CAGE_DEBUG
	const string Start = u8"S";
	const string Goal = u8"G";
	const string Path = u8"+";
#else
	const string Start = u8"\U0001F415"; // dog
	const string Goal = u8"\U0001F9B4"; // bone
	const string Path = u8"\U0001F43E"; // paw prints
#endif // CAGE_DEBUG

	string connectedWall(uint32 neighbors)
	{
		switch (neighbors)
		{
		case 0: return "O";
		case 1: return u8"\u2500";
		case 2: return u8"\u2502";
		case 3: return u8"\u2518";
		case 4: return u8"\u2500";
		case 5: return u8"\u2500";
		case 6: return u8"\u2514";
		case 7: return u8"\u2534";
		case 8: return u8"\u2502";
		case 9: return u8"\u2510";
		case 10: return u8"\u2502";
		case 11: return u8"\u2524";
		case 12: return u8"\u250C";
		case 13: return u8"\u252C";
		case 14: return u8"\u251C";
		case 15: return u8"\u253C";
		default:
			CAGE_THROW_CRITICAL(Exception, "invalid connected walls code");
		}
	}

	string convertToAscii(Labyrinth &lab, uint32 y)
	{
		string res;
		for (uint32 x = 1; x < lab.width - 1; x++)
		{
			switch (lab.cell(ivec2(x, y)))
			{
			case Cell::None:
				res += u8" ";
				break;
			case Cell::Wall:
			{
				uint32 neighbors = 0;
				if (lab.cell(ivec2(x - 1, y)) == Cell::Wall)
					neighbors += 1; // left
				if (lab.cell(ivec2(x, y - 1)) == Cell::Wall)
					neighbors += 2; // top
				if (lab.cell(ivec2(x + 1, y)) == Cell::Wall)
					neighbors += 4; // right
				if (lab.cell(ivec2(x, y + 1)) == Cell::Wall)
					neighbors += 8; // bottom
				res += connectedWall(neighbors);
			} break;
			case Cell::Empty:
				res += u8" ";
				break;
			case Cell::Start:
				res += Start;
				break;
			case Cell::Goal:
				res += Goal;
				break;
			case Cell::Path:
				res += Path;
				break;
			default:
				res += " ";
				break;
			}
		}
		return res;
	}
}

void outputs(Labyrinth &lab)
{
	CAGE_LOG(SeverityEnum::Info, "output", "generating outputs");

	Holder<File> p = writeFile("plain.txt");
	Holder<File> c = writeFile("cypher.txt");

	for (uint32 y = 1; y < lab.height - 1; y++)
	{
		string s = convertToAscii(lab, y);
		p->writeLine(s);
		CAGE_LOG_CONTINUE(SeverityEnum::Info, "layout", s);
		s = replace(s, Path, " ");
		s = trim(s);
		c->write(s);
	}

	p->close();
	c->writeLine("");
	c->close();
}
