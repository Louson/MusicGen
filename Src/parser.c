#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parser.h"
#include "read.h"

int
parse(char* file_name, midi_struct *res)
{
	FILE* file = fopen(file_name, "r");
	if ( file == NULL ) {
		printf("Unable to open file %s", file_name);
		return -1;
	}

	head_struct *head = malloc(sizeof(*head));
	if ( parse_head(file, head) <0 )
		return -1;

	track_struct *tracks = malloc((head->tracks_n+1) * sizeof(*tracks));
	int track_it;
	for (track_it = 1 ; track_it <= head->tracks_n ; track_it++) {
		tracks[track_it].num = track_it;
		parse_track(file, &tracks[track_it]);
	}

	fclose(file);	

	res->head = head;
	res->tracks = tracks;

	/* Read events in each track*/
	event_struct event;
	for (track_it = 1 ; track_it <= head->tracks_n ; track_it++) {
		printf("\nTrack n°%d\n", track_it);
		unsigned char* ptr = tracks[track_it].data;
		while (ptr-tracks[track_it].data < tracks[track_it].size) {
			ptr = parse_event(ptr, &event);
		}
	}
	return 0;
}

int
parse_head(FILE* file, head_struct* head)
{
	char head_id[4];
	fread((void*)head_id, sizeof(char), 4, file);
	if ( strncmp(head_id, "MThd", 4) != 0 ) {
		printf("Error: Not a midi file\n");
		return -1;
	}

	int head_size = read_int(file);
	if ( head_size != HEAD_SIZE ) {
		printf("Error: header is corrupted");
		return -1;
	}

	head->format = read_short(file);
	head->tracks_n = read_short(file);
	head->time_div = read_short(file);

	printf ("Format: %d\nNumber of tracks: %d\nTime division: %d\n\n",
		head->format, head->tracks_n, head->time_div);

	return 0;
}

int
parse_track(FILE* file, track_struct *track)
{
	char track_id[5];
	track_id[4] ='\0';
	fread((void*)track_id, sizeof(char), 4, file);
	if ( strncmp(track_id, "MTrk", 4) != 0 ) {
		printf("Error: the track %d is corrupted\n",
			track->num);
		return -1;
	}
	track->size = read_int(file);

	int i;
	for (i=0 ; i<track->size ; i++) {
	        fread((track->data)+i, 1, sizeof(char), file);
	}
	return 0;
}


unsigned char*
parse_event(unsigned char* data, event_struct *res)
{
	unsigned char *c = data;

	/* delta_time */
	c = read_vlv(data, &(res->delta_time));

	if ( *c == 255 ) { // Meta event
		return parse_meta_event(c+1, res);
	} else if ( *c == 240 || *c == 247 ) {
		puts("SYSTEM EVENT");
	}
	printf("%x\n", *c);
	res->type = *c >> 4; // first 4 bits
	res->channel = *c & 15; // last 4 bits
	c++;

	switch(res->type) {
	case 8:
		res->p1 = *c; c++;
		res->p2 = *c; c++;
		break;
	case 9:
		res->p1 = *c; c++;
		res->p2 = *c; c++;
		break;
	case 10:
		res->p1 = *c; c++;
		res->p2 = *c; c++;
		break;
	case 11:
		res->p1 = *c; c++;
		res->p2 = *c; c++;
		break;
	case 12:
		res->p1 = *c; c++;
		break;
	case 13:
		res->p1 = *c; c++;
		break;
	case 14:
		res->p1 = *c; c++;
		res->p2 = *c; c++;
		break;
	default:
		printf("Uncorrect event type: %d\n", res->type);
		assert(0);
		break;
	}
	
	return c;
}

unsigned char*
parse_meta_event(unsigned char* data, event_struct *res)
{
	int length, i;
	unsigned char *c = data;
	switch(*c) {
	case 0: 
		c++;
		res->p1 = *c; c++;
		res->p2 = *c; c++;
		return c;
	case 1:
		printf("Text      :");
		break;
	case 2:
		printf("Copyright :");
		break;
	case 3:
		printf("Title     :");
		break;
	case 4:
		printf("Instrument:");
		break;
	case 5:
		printf("Lyrics    :");
		break;
	case 6:
		printf("Marker    :");
		break;
	case 7:
		printf("Cue point :");
		break;
	case 32:
		c++;
		assert(*c == 1); c++;
		printf("channel %d\n", *c); c++;
		return c;
	case 33:
		c++;
		assert(*c == 1); c++;
		printf ("port %d\n", *c); c++;
		return c;
	case 47:
		c++;
		assert(*c == 0); c++;
		printf("EOT\n");
		return c;
	case 81:
		c++;
		assert(*c == 3); c++;
		c += 3;
		return c;
	case 84:
		c++;
		assert(*c == 5); c++;
		printf("SMPTE\n");
		c += 5;
		return c;
	case 88:
		c++;
		assert(*c == 4); c++;
		printf("Time signature\n");
		c += 4;
		return c;
	case 89:
		c++;
		assert(*c == 2); c++;
		printf("Key signature\n");
		c += 2;
		return c;
	case 127:
		c++;
		length = *c; c++;
		printf("Sequencer\n");
		c += length;
		return c;
	default:
		printf("%x\n", *c);
		c++;
		return c;
	}
	c++;
	length = *c; c++;
	for (i=0 ; i<length ; i++) {
		printf("%c", *c);
		c++;
	} puts("");
	return c;
}

int
write(const midi_struct* song, const char* file_name)
{
	FILE* file = fopen(file_name, "w+");
	if ( file == NULL ) {
		printf("Unable to open file %s", file_name);
		return -1;
	}

	fclose(file);
	return 0;
}

void
destroy(midi_struct *mid)
{
	free(mid->tracks);
	free(mid->head);
}


unsigned char*
read_vlv(unsigned char* stream, int* res)
{
	unsigned char *c = stream;;
	*res = *c & 127; // 0b 0111 1111
	while ( *c >> 7 ) { // first bit
		c++;
		*res = (*res << 7) + (*c & 127);
	}
	return c+1;
}
