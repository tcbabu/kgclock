#ifndef THREADTOOL_H
#define THREADTOOL_H
int getCores();
void init_threads(int cores);
void do_it_any_thread(void *(*threadFunc)(void *),void *arg);
void wait_threads();
#endif
