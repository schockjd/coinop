/*
 * Main coin-op header file. 
 */

//start functions for server an acceptor threads
void *server_thread_start(void *arg);
void *acceptor_thread_start(void *arg);

//threadsafe functions which can be called from any thread
void set_time(unsigned int seconds);
void add_time(unsigned int seconds);
int get_time_remaining();

