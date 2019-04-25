#ifndef _LOGGER_H_
#define _LOGGER_H_

void LogWriterInit();
int sync_all_logs();

int logprint(const char *format, ...);
int logPrintWithSystemID(const char *format, ...);

#endif //


