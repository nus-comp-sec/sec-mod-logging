#include "events.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

// Consts
#define HL_LOG_PATTERN "%s [%s] %s | %s"
#define HL_LOG_TAG "LIB_HOOK"
#define HL_LOG_INDENT 4
#define HL_LOG_EXTRA_LEN 10

// Set the configuration(for Android only)
#ifdef __arm__
char log_location[50];
__attribute__((visibility ("default"))) int set_Security_ConfValue(const char *location_value)
{
    strcpy(log_location, location_value);
    return 0;
}
#endif

// Example: my_event_logger function
__attribute__((visibility ("hidden"))) const char* eventToString(EVENT event)
{
    return ArrayEvent[event];
}

__attribute__((visibility ("hidden"))) char* stringWithSpace(int count)
{
    char *ans = (char *)malloc(count + 1);
    memset(ans, ' ', count);
    ans[count] = '\0';
    return ans;
}

__attribute__((visibility ("hidden"))) char* nameWithTime()
{
    time_t logtime;
    struct tm *ptime;
    char * logname = (char *)malloc(25);

    logtime = time(NULL);
    ptime = localtime(&logtime);
    sprintf(logname,"log-%04d%02d%02d%02d%02d.txt",1900+ptime->tm_year,1+ptime->tm_mon,ptime->tm_mday,ptime->tm_hour,ptime->tm_min);
    return logname;
}

__attribute__((visibility ("hidden"))) void my_event_logger(EVENT event, const char *state, void **func_pointers, const char *arg_strs, void **raw_args)
{
    static int Exec_flag = 0;  /* 0 Not Executed  1 Be Executed */
    static FILE *logfile_pointer = NULL;
    static int hlStackDepth = 0;

    if(strcmp(state, "<=") == 0)
        hlStackDepth --;
    
    const char *eventStr = eventToString(event);
    char *indentSpace = stringWithSpace(HL_LOG_INDENT * hlStackDepth);
    char *finalStr = (char *)malloc(strlen(eventStr) + strlen(state) + strlen(arg_strs) + HL_LOG_EXTRA_LEN + HL_LOG_INDENT * hlStackDepth);
    sprintf(finalStr,HL_LOG_PATTERN,indentSpace,state,eventStr,arg_strs);
    
#ifdef __arm__
    static int logtype = 0; /* 0:invoking __android_log_write  others:write the log to specific files*/
    if(Exec_flag == 0)
        logtype = strcmp(log_location, "logcat");

    static int (*funcp_log_write)(int, const char*, const char*);
    if(logtype == 0)
    {
        if(Exec_flag == 0)
        {
            *(void **)(&funcp_log_write) = func_pointers[0];
            Exec_flag = 1;
        }
        funcp_log_write(3, HL_LOG_TAG, finalStr);
    }else{
        if(Exec_flag == 0)
        {
            char *logname = nameWithTime();
            char log_folder[50], logfile_name[50];
            sprintf(log_folder, "%s/webkit_logs", log_location);
            mkdir(log_folder, 0775);
            sprintf(logfile_name,"%s/%s", log_folder, logname);
            logfile_pointer = fopen(logfile_name, "a+");
            free(logname);
            Exec_flag = 1;
        }
        if(logfile_pointer)
            fprintf(logfile_pointer,"%s\t%s\n",HL_LOG_TAG,finalStr);
    }
#else
    if(Exec_flag == 0)
    {
        char *logtype = NULL;
        logtype = getenv("LOGTYPE");
        if((logtype != NULL) && (strcmp(logtype, "PRINT") == 0))
            logfile_pointer = stderr;
        else
        {
            char logfile_name[50];
            mkdir("webkit_logs",0775);
            char *logname = nameWithTime();
            sprintf(logfile_name,"%s/%s","webkit_logs",logname);
            logfile_pointer = fopen(logfile_name,"a+");
            free(logname);
        }
        Exec_flag = 1;
    }
    if(logfile_pointer)
        fprintf(logfile_pointer,"%s\t%s\n",HL_LOG_TAG,finalStr);
#endif

    free(finalStr);
    free(indentSpace);
    
    if(strcmp(state, "=>") == 0)
        hlStackDepth ++;
}

// Initialize event_hander Function
__attribute__((visibility ("default"))) int security_module_init(void **event_handler)
{
    event_handler[EVENT_DEFAULT] = (void *)(&my_event_logger);
    event_handler[HTMLDocument_createElement] = (void *)(&my_event_logger);
    event_handler[JavaScript_Execute]= (void *)(&my_event_logger);
    return 0;
}
