#include <cage-core/logger.h>

using namespace cage;

int main()
{
	Holder<Logger> log = newLogger();
	log->format.bind<logFormatConsole>();
	log->output.bind<logOutputStdOut>();

	// todo

	return 0;
}
