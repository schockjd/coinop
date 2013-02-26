/*
 * server protocol defns.
 */

//Commands

#define CMD_TURN_ON   1
#define CMD_TURN_OFF  2
#define CMD_SET_TIME  3
#define CMD_ADD_TIME  4
#define CMD_GET_TIME  5

typedef union {
  struct {
    char cmd;
    char rsvd[3];
    unsigned int payload;
  };
  char data[8];
} co_request;

typedef union {
  struct {
    char cmd;
    char rsp;
    char rsvd[2];
    unsigned int payload;
  };
  char data[8];
} co_response;
