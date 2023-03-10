/* RGD SDBoot Installer */

#include "tools.h"
#define MISSING_FILE     -1
#define BOOT2_DOLPHIN    -4
#define HASH_MISMATCH    -1022
#define CANNOT_DOWNGRADE -1031

#ifndef BOOT2_H_
#define BOOT2_H_

typedef struct {
	signed_blob *ca_cert;
	signed_blob *cp_cert;
	signed_blob *xs_cert;
	
	signed_blob *tik_cert;
	signed_blob *TMD_cert;
	
} certificates;

typedef struct {
	u32 headerLen;
	u32 dataOffset;
	u32 certsLen;
	u32 tikLen;
	u32 TMDLen;
	
	certificates *certs;
	signed_blob *tik;
	signed_blob *TMD;
	u8 *content;
	u32 contentSize;
} boot2;

typedef struct {
	u32 headerLen;
	u16 wadType;
	u16 wadVersion;
	u32 certsLen;
	u8  reserved[4];
	u32 tikLen;
	u32 TMDLen;
	u32 contentSize;
	u32 footerSize;
	u8  padding[0x20];
	
	certificates *certs;
	signed_blob *tik;
	signed_blob *TMD;
	u8 *content;
} WAD;

WAD *ReadWAD(const char *filename);
boot2 *ReadBoot2(const char *filename);
s32 InstallRawBoot2(const char* filename);
s32 InstallWADBoot2(const char* filename);
s32 InstallSDBoot(const char* filename);
s32 InstallNANDBoot(const char* filename, const char* payload);
s32 BackupBoot2Blocks(const char* filename);
s32 RestoreBoot2Blocks(const char* filename);

#endif
