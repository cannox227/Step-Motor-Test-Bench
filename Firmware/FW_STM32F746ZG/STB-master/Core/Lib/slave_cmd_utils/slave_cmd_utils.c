#include "slave_cmd_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

motor_cmd_msg motor_cmd_message;
char motor_cmd[MOTOR_CMD_SIZE];
char cmd_params[MOTOR_CMD_PARAM_SIZE];

bool parse_cmd() {
    bool parse_errors = false;
    if (strcmp(motor_cmd, "fw") == 0) {
        motor_cmd_message.cmd = MOTOR_MOVE_FW;
        motor_cmd_message.val = atoi(cmd_params);
    } else if ((strcmp(motor_cmd, "bw") == 0)) {
        motor_cmd_message.cmd = MOTOR_MOVE_BW;
        motor_cmd_message.val = atoi(cmd_params);
    } else if ((strcmp(motor_cmd, "hd") == 0)) {
        motor_cmd_message.cmd = MOTOR_HOLD;
        motor_cmd_message.val = 0;
    } else if ((strcmp(motor_cmd, "st") == 0)) {
        motor_cmd_message.cmd = MOTOR_STOP;
        motor_cmd_message.val = 0;
    } else if (strcmp(motor_cmd, "ps") == 0) {
        motor_cmd_message.cmd = MOTOR_GET_POS;
        motor_cmd_message.val = 0;
    } else if ((strcmp(motor_cmd, "gt") == 0)) {
        motor_cmd_message.cmd = MOTOR_GO_TO;
        motor_cmd_message.val = atoi(cmd_params);
    } else if ((strcmp(motor_cmd, "gh") == 0)) {
        motor_cmd_message.cmd = MOTOR_GO_HOME;
        motor_cmd_message.val = 0;
    } else {
        parse_errors = true;
    }
    return parse_errors;
}

void build_cmd(char *raw_smg) {
    strncpy(motor_cmd, raw_smg, MOTOR_CMD_SIZE);
    strncpy(cmd_params, &raw_smg[MOTOR_CMD_SIZE + 1], MOTOR_CMD_PARAM_SIZE);
}