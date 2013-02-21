

typedef void (*acceptor_listener)();

int init_coin_acceptor(unsigned int gpio);
int register_accept_listener(acceptor_listener listener);
int unregister_accept_listener(acceptor_listener listener);

