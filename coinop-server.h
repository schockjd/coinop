/*
 * server protocol defns.
 */

//Commands

#define CMD_TURN_ON   1
#define CMD_TURN_OFF  2
#define CMD_SET_TIME  3
#define CMD_ADD_TIME  4
#define CMD_GET_TIME  5

typedef struct {
  char cmd;
  char data[7];
} co_request;

typedef struct {
  char rsp;
  char data[7];
} co_response;

