#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void handler_1(void){
	sleep(1);
	printf("handler_1\n");
}
void handler_2(void){
	sleep(1);
	printf("handler_2\n");
}
void handler_3(void){
	sleep(1);
	printf("handler_3\n");
}
void handler_4(void){
	sleep(1);
	printf("handler_4\n");
}
void handler_5(void){
	sleep(1);
	printf("handler_5\n");
}
void handler_6(void){
	sleep(1);
	printf("handler_6\n");
}
void handler_7(void){
	sleep(1);
	printf("handler_7\n");
}
void handler_8(void){
	sleep(1);
	printf("handler_8\n");
}
void handler_9(void){
	sleep(1);
	printf("handler_9\n");
}
void handler_10(void){
	sleep(1);
	printf("handler_10\n");
}
void handler_11(void){
	sleep(1);
	printf("handler_11\n");
}
void handler_12(void){
	sleep(1);
	printf("handler_12\n");
}
void handler_13(void){
	sleep(1);
	printf("handler_13\n");
}
void handler_14(void){
	sleep(1);
	printf("handler_14\n");
}
void handler_15(void){
	sleep(1);
	printf("handler_15\n");
}
void handler_16(void){
	sleep(1);
	printf("handler_16\n");
}
void handler_17(void){
	sleep(1);
	printf("handler_17\n");
}
void handler_18(void){
	sleep(1);
	printf("handler_18\n");
}
void handler_19(void){
	sleep(1);
	printf("handler_19\n");
}
void handler_20(void){
	sleep(1);
	printf("handler_20\n");
}
void handler_21(void){
	sleep(1);
	printf("handler_21\n");
}
void handler_22(void){
	sleep(1);
	printf("handler_22\n");
}
void handler_23(void){
	sleep(1);
	printf("handler_23\n");
}
void handler_24(void){
	sleep(1);
	printf("handler_24\n");
}
void handler_25(void){
	sleep(1);
	printf("handler_25\n");
}
void handler_26(void){
	sleep(1);
	printf("handler_26\n");
}
void handler_27(void){
	sleep(1);
	printf("handler_27\n");
}
void handler_28(void){
	sleep(1);
	printf("handler_28\n");
}
void handler_29(void){
	sleep(1);
	printf("handler_29\n");
}
void handler_30(void){
	sleep(1);
	printf("handler_30\n");
}
void handler_31(void){
	sleep(1);
	printf("handler_31\n");
}
void handler_32(void){
	sleep(1);
	printf("handler_32\n");
}

void (*handler_arr[32])(void) = {handler_1,handler_2,handler_3,handler_4,handler_5,handler_6,
								handler_7,handler_8,handler_9,handler_10,handler_11,handler_12,
								handler_13,handler_14,handler_15,handler_16,handler_17,handler_18,
								handler_19,handler_20,handler_21,handler_22,handler_23,handler_24,
								handler_25,handler_26,handler_27,handler_28,handler_29,handler_30,
								handler_31,handler_32};