#include "bluetooth.h"
#include "usart.h"
#include "Wheel.h"


// 解析接收到的数据
void BlueToothInit(){
	USART2_RegisterCallback(Bluetooth_ProcessReceivedData);
}

int parseCommand(const char* buffer, Command *cmd) {
    if (buffer[0] != '$' || buffer[strlen(buffer) - 1] != '#') {
        return -1;  // 检查包头和包尾
    }

    // 使用sscanf从字符串解析命令和数据位
    int parsed = sscanf(buffer, "$%d,%d,%d,%d,%d,%d,%d,%d,%d#",
                        &cmd->cmd1, &cmd->cmd2, &cmd->cmd3, &cmd->cmd4,
                        &cmd->cmd6, &cmd->data8, &cmd->data9, &cmd->data10, &cmd->data11);

    if (parsed != 9) {  // 确保正确解析了所有位
        return -1;
    }

    return 0;
}

// 执行解析出的命令
void executeCommand(const Command *cmd) {
    // 根据命令值执行动作，这里只是打印出来
   switch(cmd->cmd1){
		 case 1:Car_Run(2000);break;
		 case 2:Car_Back(1500);break;
		 case 3:Car_Left(2000);break;
		 case 4:Car_Right(2000);break;
		 default:Car_Stop();break;
	 }
}

void Bluetooth_ProcessReceivedData(const char* data) {
    Command cmd;
    if (parseCommand(data, &cmd) == 0) {
        executeCommand(&cmd);
    } else {
        printf("Command parsing failed.\n");
    }
}