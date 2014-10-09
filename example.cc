#include "atom.hh"

typedef atom::atom_t atom_t;
using atom::literal;

int main()
{
	atom_t str = literal("Atom 0");
	atom_t str2 = literal("Atom 1");

	return str != str2;
}
