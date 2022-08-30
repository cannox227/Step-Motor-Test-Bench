#ifndef SLAVE_CMD_UTILS_H__
#define SLAVE_CMD_UTILS_H__

#include "main.h"

#include <inttypes.h>
#include <stdbool.h>

#define MOTOR_CMD_SIZE       2
#define MOTOR_CMD_PARAM_SIZE 6

static const char MOTOR_GO_FOREVER[]   = "fw+999999\r\n";
static const char MOTOR_STOP_FOREVER[] = "st+000000\r\n";

typedef enum {
    MOTOR_STOP = 0U,
    MOTOR_HOLD,
    MOTOR_MOVE_FW,
    MOTOR_MOVE_BW,
    MOTOR_GET_POS,
    MOTOR_GO_TO,
    MOTOR_GO_HOME,
    NONE
} motor_cmd_code;

typedef struct {
    motor_cmd_code cmd;
    uint32_t val;
} motor_cmd_msg;

extern motor_cmd_msg motor_cmd_message;

bool parse_motor_cmd();
void build_motor_cmd(char *raw_msg);

#endif