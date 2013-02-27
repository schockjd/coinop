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
int is_active();
void force_on();
void force_off();

//gpio functions
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio, unsigned int *value);
int gpio_poll(unsigned int gpio, unsigned int lvl, unsigned int timeout);

//switcher functions
int set_switcher(unsigned int select);
int get_switcher(unsigned int *select);

