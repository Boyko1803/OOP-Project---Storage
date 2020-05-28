#include "Commands.h"

using namespace std;

int main()
{
	StorageFile file;

	while (Commands::menu(file));

	return 0;
}