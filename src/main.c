/* RGD SDBoot Installer */

#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <gctypes.h>

#include "errorhandler.h"
#include "errorcodes.h"
#include "seeprom.h"
#include "boot2.h"
#include "tools.h"

#define RGDSDB_VER_MAJOR	0
#define RGDSDB_VER_MINOR	3

#define SDBOOT_PATH "/boot2/sdboot.bin"
#define NANDBOOT_PATH "/boot2/nandboot.bin"

#define AHBPROT_DISABLED (*(vu32*)0xcd800064 == 0xFFFFFFFF)

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int main(int argc, char **argv) {

	VIDEO_Init();
	WPAD_Init();
	PAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	
	printf("\x1b[2;0H");
	
	if(IsDolphin()) {
		ThrowError(errorStrings[ErrStr_InDolphin]);
	} else if(IsWiiU()) {
		ThrowError(errorStrings[ErrStr_InCafe]);
	}
	
	printf("RGD SDBoot Installer v%u.%u - by \x1b[32mroot1024\x1b[37m, \x1b[31mRedBees\x1b[37m, \x1b[31mDeadlyFoez\x1b[37m\nraregamingdump.ca", RGDSDB_VER_MAJOR, RGDSDB_VER_MINOR);
	fatInitDefault();

	if(!AHBPROT_DISABLED) { 
		ThrowError(errorStrings[ErrStr_NeedPerms]);
	}
	printf("\n\nPress any controller button to clear the boot2 version.");
	WaitForPad();
	ClearVersion();
	
	u32 choice = 0;
	s32 ret = 0;
	printf("\The boot2 version was cleared successfully!\nPress the A button to install SDboot from /boot2/sdboot.bin, or the B button to install nandboot from /boot2/nandboot.bin.\n");

choice:
	choice = WaitForPad();
	if(choice & WPAD_BUTTON_A) {
		ret = InstallRawBoot2(SDBOOT_PATH);
		goto out;
	} else if(choice & WPAD_BUTTON_B) {
		ret = InstallRawBoot2(NANDBOOT_PATH);
		goto out;
	} else { goto choice; }
	
out:
	switch(ret){
		case 0:
			printf("%s was installed successfully!\n", (choice & WPAD_BUTTON_A) ? "SDBoot" : "NANDBoot"); break;
		case -4:
			ThrowError(errorStrings[ErrStr_InDolphin]); break;
		case -1022:
			ThrowError(errorStrings[ErrStr_BadFile]); break;
		case -1031: // TODO: Find out why this triggers sometimes even after writing SEEPROM
			printf("Error: cannot downgrade boot2\n"); break;
		default:
			ThrowErrorEx(errorStrings[ErrStr_Generic], ret); break;
	}

	WaitExit();
	return 0;	// NOT REACHED HERE
}