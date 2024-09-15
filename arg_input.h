#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>

/* flag definitions */
#define ST_THIRD_OCTAVE     (0)
#define ST_OCTAVE           (1)
#define ST_IGNORE_SILENCE   (2)
#define ST_CHANNEL_SELECT   (3)
#define ST_START_ON_TIME    (4)
#define ST_FORCE_FREQUENCY  (5)
#define ST_OUTPUT           (6)   
#define ST_HELP             (7)   

#define ST_MAX              (ST_HELP + 1)

/* argv string comparison defines */
#define USE_THIRD_OCTAVE_FULL      "--third_octave"
#define USE_IGNORE_SILENCE         "--ignore_silence"

/* store all program variables in flags */
typedef struct {
    uint64_t param;
    uint8_t error;
    uint8_t channel;
    uint16_t start;
    uint16_t frequency;
    char output[24];
} settings_t;


/* valid program arguments table */
extern const char argv_match[ST_MAX][24];
extern const char argv_match_short[ST_MAX][3];

/* program arguments description */
extern const char argv_description[ST_MAX][128];

/* program arguments size */
extern const int argv_size[ST_MAX];

/* handlers for the arguments */
int empty(settings_t *__set, char *__argv);
int arg_channel(settings_t *__set, char *__argv);
int arg_start(settings_t *__set, char *__argv);
int arg_frequency(settings_t *__set, char *__argv);
int arg_output(settings_t *__set, char *__argv);


//static void (*arg_handler[ST_MAX])(settings_t *, char *) = {NULL, NULL, NULL,
//arg_channel, arg_start, arg_frequency, arg_output, NULL};

typedef struct {
    int (* fun[ST_MAX])(settings_t *, char *);
} arg_handler_t;

/* check if __flag bit is set */
#define is_flag(__param, __flag) ((__param & __flag) >> __flag)
#define set_flag(__param, __flag) (__param |= __flag)
#define unset_flag(__param, _flag) (__param &= ~__flag)
#define flag_handle(__param, __flag, __true, __false) (is_flag(__param, __flag) ? __true : __false)

/* allocate settings struct */
settings_t *settings_init(int __argc, char **__argv);

/**/
int settings_apply(settings_t *__settings, int __argc, char **__argv);

/* free allocated memory */
void settings_free(settings_t *__settings);

/* print help */
void settings_help(void);

/* invalid argument print */
void settings_einval(void);