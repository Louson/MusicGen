#ifndef _MIDI_H
#define _MIDI_H

#define HEAD_ID "MThd"
#define HEAD_SIZE 6
#define TRACK_ID "MTrk"

typedef struct head {
	/* int id; */
	/* int size; */
	short format;
	short tracks_n;
	short time_div;
} head_struct;

typedef struct track {
	int num;
	int size;
	unsigned char data[1000000];
} track_struct;

typedef struct event {
	int delta_time;
	int type;
	int channel;
	char p1;
	char p2;
} event_struct;

typedef struct midi {
	head_struct *head;
	track_struct *tracks;
} midi_struct;

#endif // _MIDI_H
