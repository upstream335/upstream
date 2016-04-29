#ifndef _LOG_H_
#define _LOG_H_

extern void logOpen(void);
extern void logClose(void);
extern void logwrite(const char *fmt, ...);

#endif
