#pragma once

typedef struct {
	int num, notes_max, knd, roll_id, branch_id;
	float create_time, judge_time, pop_time, bpm, scroll, x_ini;
	bool flag, isThrough;
	float x;

} NOTES_T;

typedef struct {

	int level[7],x,y,tmp,genre;
	char title[256],subtitle[256],path[256],tja[256],wave[256];
	bool course[7],course_exist[5];
} LIST_T;
