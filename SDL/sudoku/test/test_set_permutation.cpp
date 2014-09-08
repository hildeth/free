#include "Set.h"

int main()
{
	for (Set bit_perm = Set::perm_begin(3);
		 bit_perm.perm_more();
		 bit_perm.perm_next())
		std::cout << bit_perm << std::endl;
	return 0;
}
