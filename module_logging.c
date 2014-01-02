#include "events.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

// Consts
#define HL_LOG_TAG "LIB_HOOK"
#define HL_LOG_INDENT 4

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
__attribute__((visibility ("hidden"))) char *nameWithTime()
{
    time_t logtime;
    struct tm *ptime;
    char * logname = (char *)malloc(25);

    logtime = time(NULL);
    ptime = localtime(&logtime);
    sprintf(logname, "log-%04d%02d%02d%02d%02d.txt", 1900+ptime->tm_year, 1+ptime->tm_mon, ptime->tm_mday, ptime->tm_hour, ptime->tm_min);
    return logname;
}

__attribute__((visibility ("hidden"))) char *setfinalStr(int hlStackDepth, EVENT event, const char *state, const char *arg_strs)
{
	//prepare strings: level, fclass, name, type, value
	char level[2] = "";
	level[0] = (char)(hlStackDepth%10 + 48); /*cycle from 0 to 9, and lStackDepth is always positive, right?*/
	const char *fclass = ArrayEvent[event][0];
	const char *name = ArrayEvent[event][1];
	char type[7] = "";
	if (strcmp(state, "=>") == 0)
		strcpy(type, "call");
	else if (strcmp(state, "<=") == 0)
		strcpy(type, "return");
	else if (strcmp(state, "--") == 0)
		strcpy(type, "repeat");
	else
		strcpy(type, "other");
	const char *value = arg_strs;

	//combined strings into rows
	char indentSpace[HL_LOG_INDENT + 1] = "";
	memset(indentSpace, ' ', HL_LOG_INDENT);
	char *row1 = (char *)malloc(strlen("<log level=\"") + strlen(level) + strlen("\">\n") + 1);
	sprintf(row1, "%s%s%s", "<log level=\"", level, "\">\n");
	char *row2;
	if (event != JavaScript_Execute)
	{
		row2 = (char *)malloc(strlen(indentSpace) + strlen("<func class=\"") + strlen(fclass) + strlen("\" name=\"") + strlen(name) + strlen("\" type=\"") + strlen(type) + strlen("\">\n") + 1);
		sprintf(row2, "%s%s%s%s%s%s%s%s", indentSpace, "<func class=\"", fclass, "\" name=\"", name, "\" type=\"", type, "\">\n");
	}
	else
	{
		row2 = (char *)malloc(strlen(indentSpace) + strlen("<code name=\"") + strlen(name) + strlen("\" type=\"") + strlen(type) + strlen("\">\n") + 1);
		sprintf(row2, "%s%s%s%s%s%s", indentSpace, "<code name=\"", name, "\" type=\"", type, "\">\n");
	}
	char *row3 = (char *)malloc(strlen(indentSpace) + strlen(indentSpace) + strlen(value) + strlen("\n") + 1);
	sprintf(row3, "%s%s%s%s", indentSpace, indentSpace, value, "\n");
	char *row4;
	if (event != JavaScript_Execute)
	{
		row4 = (char *)malloc(strlen(indentSpace) + strlen("</func>\n") + 1);
		sprintf(row4, "%s%s", indentSpace, "</func>\n");
	}
	else
	{
		row4 = (char *)malloc(strlen(indentSpace) + strlen("</code>\n") + 1);
		sprintf(row4, "%s%s", indentSpace, "</code>\n");
	}
	char row5[8] = "</log>\n";

	//set finalStr
	char *finalStr = (char *)malloc(strlen(row1) + strlen(row2) + strlen(row3) + strlen(row4) + strlen(row5) + 1);
    sprintf(finalStr, "%s%s%s%s%s", row1, row2, row3, row4, row5);

	//free memory
	free(row1);
	free(row2);
	free(row3);
	free(row4);

	return finalStr;
}

__attribute__((visibility ("hidden"))) void my_event_logger(EVENT event, const char *state, void **func_pointers, const char *arg_strs, void **raw_args)
{
    static int Exec_flag = 0;  /* 0 Not Executed  1 Be Executed */
    static FILE *logfile_pointer = NULL;
    static int hlStackDepth = 0;

    if(strcmp(state, "<=") == 0)
        hlStackDepth --;
    
    char *finalStr = setfinalStr(hlStackDepth, event, state, arg_strs);

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
            sprintf(logfile_name, "%s/%s", log_folder, logname);
            logfile_pointer = fopen(logfile_name, "a+");
            free(logname);
            Exec_flag = 1;
        }
        if(logfile_pointer)
            fprintf(logfile_pointer, "%s\t%s\n", HL_LOG_TAG, finalStr);
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
            mkdir("webkit_logs", 0775);
            char *logname = nameWithTime();
            sprintf(logfile_name, "%s/%s", "webkit_logs", logname);
            logfile_pointer = fopen(logfile_name, "a+");
            free(logname);
        }
        Exec_flag = 1;
    }
    if(logfile_pointer)
        fprintf(logfile_pointer, "%s\t%s\n", HL_LOG_TAG, finalStr);
#endif

    free(finalStr);
    
    if(strcmp(state, "=>") == 0)
        hlStackDepth ++;
}

// Initialize event_hander Function
__attribute__((visibility ("default"))) int security_module_init(void **event_handler)
{
    event_handler[EVENT_DEFAULT] = (void *)(&my_event_logger);
    event_handler[HTMLDocument_createElement] = (void *)(&my_event_logger);
    event_handler[HTMLElement_create] = (void *)(&my_event_logger);
    event_handler[HTMLElement_setInnerHTML] = (void *)(&my_event_logger);
    event_handler[HTMLElement_setOuterHTML] = (void *)(&my_event_logger);
    event_handler[HTMLElement_setInnerText] = (void *)(&my_event_logger);
    event_handler[HTMLElement_setOuterText] = (void *)(&my_event_logger);
    event_handler[DOMWindow_alert] = (void *)(&my_event_logger);
    event_handler[DOMWindow_confirm] = (void *)(&my_event_logger);
    event_handler[DOMWindow_prompt] = (void *)(&my_event_logger);
    event_handler[WebCore_setCookies] = (void *)(&my_event_logger);
    event_handler[JSDocument_setLocation] = (void *)(&my_event_logger);
    event_handler[JSLocation_setHref] = (void *)(&my_event_logger);
    event_handler[JSLocation_setProtocol] = (void *)(&my_event_logger);
    event_handler[JSLocation_setHost] = (void *)(&my_event_logger);
    event_handler[JSLocation_setHostname] = (void *)(&my_event_logger);
    event_handler[JSLocation_setPort] = (void *)(&my_event_logger);
    event_handler[JSLocation_setPathname] = (void *)(&my_event_logger);
    event_handler[JSLocation_setSearch] = (void *)(&my_event_logger);
    event_handler[JSLocation_setHash] = (void *)(&my_event_logger);
    event_handler[JSLocation_replace] = (void *)(&my_event_logger);
    event_handler[JSLocation_reload] = (void *)(&my_event_logger);
    event_handler[JSLocation_assign] = (void *)(&my_event_logger);
    event_handler[JSEventListener_handleEvent] = (void *)(&my_event_logger);
    event_handler[Element_getAttribute] = (void *)(&my_event_logger);
    event_handler[Element_setAttribute] = (void *)(&my_event_logger);
    event_handler[Element_focus] = (void *)(&my_event_logger);
    event_handler[Element_blur] = (void *)(&my_event_logger);
    event_handler[Document_getElementById] = (void *)(&my_event_logger);
    event_handler[Document_createElement] = (void *)(&my_event_logger);
    event_handler[Document_createDocumentFragment] = (void *)(&my_event_logger);
    event_handler[Document_createTextNode] = (void *)(&my_event_logger);
    event_handler[Document_setDocumentURI] = (void *)(&my_event_logger);
    event_handler[Document_setTitle] = (void *)(&my_event_logger);
    event_handler[Document_open] = (void *)(&my_event_logger);
    event_handler[Document_close] = (void *)(&my_event_logger);
    event_handler[Document_write] = (void *)(&my_event_logger);
    event_handler[Document_cookie] = (void *)(&my_event_logger);
    event_handler[Document_setCookie] = (void *)(&my_event_logger);
    event_handler[Document_setDomain] = (void *)(&my_event_logger);
    event_handler[Document_execCommand] = (void *)(&my_event_logger);
    event_handler[Document_createAttribute] = (void *)(&my_event_logger);
    event_handler[Document_setWindowAttributeEventListener] = (void *)(&my_event_logger);
    event_handler[Node_addEventListener] = (void *)(&my_event_logger);
    event_handler[Node_appendChild] = (void *)(&my_event_logger);
    event_handler[Node_insertBefore] = (void *)(&my_event_logger);
    event_handler[Storage_getItem] = (void *)(&my_event_logger);
    event_handler[Storage_setItem] = (void *)(&my_event_logger);
    event_handler[Storage_removeItem] = (void *)(&my_event_logger);
    event_handler[Storage_clear] = (void *)(&my_event_logger);
    event_handler[JavaScript_Execute] = (void *)(&my_event_logger);

    return 0;
}
