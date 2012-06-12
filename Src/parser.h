#ifndef _PARSER_H_
#define _PARSER_H_

#include "midi.h"

int parse(char* file_name, midi_struct* res);
int parse_head(FILE* file, head_struct* res);
int parse_track(FILE* file, track_struct* res);
unsigned char* parse_event(unsigned char* data, event_struct *res);
unsigned char* parse_meta_event(unsigned char* data, event_struct *res);

int write(const midi_struct* song, const char* file_name);
void destroy(midi_struct* mid);
unsigned char* read_vlv(unsigned char* stream, int *res);

#endif // _PARSER_H_
