

typedef struct {
  int request;
  unsigned char data[MAX_REQ_SIZE];
} req_data;

typedef void (*server_listener)(req_data *req);

int init_coin_server();
int register_server_listener(server_listener listener);
int unregister_server_listener(server_listener listener);
