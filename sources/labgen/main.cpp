#include "labgen.h"

#include <cage-core/logger.h>

int main()
{
	Holder<Logger> log = newLogger();
	log->format.bind<logFormatConsole>();
	log->output.bind<logOutputStdOut>();

	Labyrinth lab = generate();
	paths(lab);
	outputs(lab);

	return 0;
}
