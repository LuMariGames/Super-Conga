#pragma once
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>
#include <citro2d.h>
#include <limits.h>
#include <math.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <vector>
#include <iostream>

#define C3D_CMDBUF_SIZE 0x40000
#define VERSION "1.0.0"
#define DEFAULT_DIR "sdmc:/SC_charts/"

#define DEFAULT_JUDGE_RANGE_GREAT	0.050
#define DEFAULT_JUDGE_RANGE_OK	  	0.100
#define DEFAULT_JUDGE_RANGE_MISS	0.150

#define TOP_WIDTH 400
#define TOP_HEIGHT 240
#define BOTTOM_WIDTH 320
#define BOTTOM_HEIGHT 240
#define NOTES_MEASURE_MAX 1024
#define MEASURE_MAX 1024
#define NOTES_MAX 8192

#define LIST_MAX 1024
#define DEFAULT_BUFFER_SIZE 8192

enum COURSE_KND {

	COURSE_EASY = 0,
	COURSE_HARD,
	COURSE_EXPERT,
};
