#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"

typedef struct {
    int cmd1;
    int cmd2;
    int cmd3;
    int cmd4;
    int cmd6;
    int data8;
    int data9;
    int data10;
    int data11;
}Command;

int parseCommand(const char* buffer, Command *cmd);
void executeCommand(const Command *cmd);
void Bluetooth_ProcessReceivedData(const char* data);
void BlueToothInit();
#endif // __BLUETOOTH_H