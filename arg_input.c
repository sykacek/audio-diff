#include "arg_input.h"

/* valid program arguments table */
const char argv_match[ST_MAX][24] = { "--third_octave", "--octave", "--ignore_silence", "--channel",
"--start", "--force_freq", "--output", "--help"};

const char argv_match_short[ST_MAX][3] = { "", "", "", "-c", "-s", "-f", "-o", "-h"};

/* program arguments description */
const char argv_description[ST_MAX][128] = {"\tdata in third octave band format, used be default",
"\tdata in octave band format", "ignores silence at the beggining of track",
"\tspecifies which channel which will be used", "\tstarts computation at specified time in ms",
"forces program to use specified sample rate", "\tspecifies output file, by default out.txt",
"\tprints this help"};

const int arg_size[ST_MAX] = {0, 0, 0, 1, 1, 1, 1, 0};

int empty(settings_t *__set, char *__argv)
{
    return 0;
}

int arg_channel(settings_t *__set, char *__argv)
{
    int ret = atoi(__argv);
    if(!ret)
        return 1;
    
    __set->channel = ret;
    return 0;
}

int arg_start(settings_t *__set, char *__argv)
{
    int ret = atoi(__argv);
    if(!ret)
        return 1;
    
    __set->start = atoi(__argv);
    return 0;
}

int arg_frequency(settings_t *__set, char *__argv)
{
    int ret = atoi(__argv);
    if(!ret)
        return 1;

    __set->frequency = atoi(__argv);
    return 0;
}

int arg_output(settings_t *__set, char *__argv)
{
    if(strcpy(__set->output, __argv) == NULL)
        return 1;
    
    return 0;
}

settings_t *settings_init(int __argc, char **__argv)
{
    if(__argc < 2)
        return 0;
    /* if help is specified, abort */
    if(!strcmp(__argv[1], "--help") || !strcmp(__argv[1], "-h"))
        return NULL;
    
    settings_t *__set = (settings_t *)malloc(sizeof(settings_t));
    return __set;
}

int settings_apply(settings_t *__settings, int __argc, char **__argv)
{
    static arg_handler_t arg_handler;

    int (*functions[ST_MAX])(settings_t *, char *) = {empty, empty, empty,
    arg_channel, arg_start, arg_frequency, arg_output, empty};
    
    memcpy(arg_handler.fun, functions, sizeof(functions));
    
    int i = 0, j = 1, ret;
    while(j < __argc - 2){
        i = 0;
        while(i < ST_MAX){
            __settings->error = 0xFF;
            if(!strcmp(__argv[j], argv_match[i]) || !strcmp(__argv[j], argv_match_short[i])){
                set_flag(__settings->param, (1 << i));
                ret = arg_handler.fun[i](__settings, __argv[++j]);

                if(ret)
                    return ret;

                __settings->error = 0;
                break;
            }
            i++;
        }
        if(__settings->error)
            return EINVAL;
        j++;
    }

    return 0;
}

void settings_free(settings_t *__settings)
{
    free(__settings);
}
void settings_help(void)
{
    puts("Usage: audio-diff [options] reference.wav comparison.wav");
    puts("Options:");
    for(int i = 0; i < ST_MAX; ++i)
        if(strcmp(argv_match_short[i], ""))
            printf("%s (%s)\t\t%s\n", argv_match[i], argv_match_short[i], argv_description[i]);
        else
            printf("%s\t\t%s\n", argv_match[i], argv_description[i]);
}

void settings_einval(void)
{
    puts("Error: invalid number of arguments\nplease see audio-diff --help for help");
}
