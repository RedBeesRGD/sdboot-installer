/* RGD SDBoot Installer */

#include "tools.h"

u32 WaitForPad() {
	int wpadButtons = 0;
	while(1) {
		WPAD_ScanPads();
		for(int i = 0; i < 4; i++) {
				wpadButtons += WPAD_ButtonsDown(i);
		}
		if (wpadButtons) break;
		VIDEO_WaitVSync();
	}
	return wpadButtons;
}

u8 IsWiiU( void ) {
	if(*(vu16*)0xcd8005a0 == 0xCAFE) return 1;
	return 0;
}

u8 IsDolphin( void ) {
	#ifdef DOLPHIN_CHECK
	int fd = IOS_Open("/dev/dolphin", 1);
	if(fd >= 0) {
		IOS_Close(fd);
		return 1;
	}
	IOS_Close(fd);
	#endif
	return 0;
}

void WaitExit( void ) {
	u8 i = 0;
	u32 wpadButtons = 0;
	u32 padButtons = 0;
	printf("\n\nPress any controller button to exit.");
	while(1) {
		WPAD_ScanPads();
		PAD_ScanPads();
		for(i = 0; i < 4; i++) {
			wpadButtons += WPAD_ButtonsDown(i);
			padButtons += PAD_ButtonsDown(i);
		}
		if (wpadButtons || padButtons) exit(0);

		VIDEO_WaitVSync();
	}
}

void *alloc(u32 size){
	return memalign(32, (size+31)&(~31));
}

u32 filesize(FILE* fp){
	fseek(fp, 0L, SEEK_END);
	u32 size = ftell(fp);
	rewind(fp);
	return size;
}

int CheckFile(FILE* fp, const char *filename){
	if(fp == NULL){
		printf("Error: cannot read file %s\n", filename);
		return 0;
	}
	return 1;
}
