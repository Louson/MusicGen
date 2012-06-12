#include <stdio.h>
#include <stdlib.h>

#include "midi.h"
#include "parser.h"

int main(int a, char **b) {
	char* file_name = "chpn_op25_e12.mid";
	midi_struct song;

	parse(file_name, &song);

	destroy(&song);
	return 0;
}
