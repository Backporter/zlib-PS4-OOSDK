// requires OrbisUtil
// SAVEDATA.DAT is a rando save i extracted from skyrim

#include "../OrbisUtil/include/MessageHandler.h"
#include "../OrbisUtil/include/SystemWrapper.h"
#include "../OrbisUtil/include/WIP/Streams/IFIleStream.h"

#include <stdlib.h>
#include <stdint.h>

#if __ORBIS__
unsigned int sceLibcHeapExtendedAlloc = 1;  /* Switch to dynamic allocation */
size_t    sceLibcHeapSize = SCE_LIBC_HEAP_SIZE_EXTENDED_ALLOC_NO_LIMIT; /* no upper limit for heap area */
#include <system_service.h>
#elif __OPENORBIS__
#include <orbis/SystemService.h>
#endif

int32_t handle = -1;
int(*compress)(void*, unsigned long*, void*, unsigned long);

void Quit()
{
	sceSystemServiceLoadExec("exit", 0);
	SystemWrapper::UnmountUSB(0);
}

void LoadZLIB()
{
	int ret = 0;

	handle = sceKernelLoadStartModule("/app0/sce_module/zlib.prx", 0, 0, 0, 0, 0);
	if (handle > 0)
	{
		if ((ret = sceKernelDlsym(handle, "compress", (void**)&compress)) != 0)
		{
			MessageHandler::Notify("Failed to get function address for sceKernelSendNotificationRequest");
		}
	}
	else
	{
		MessageHandler::Notify("Faild to load PRX");
	}

}


int main()
{
	SystemWrapper::ImportSymbols();
	LoadZLIB();

	SystemWrapper::MountUSB(0);

	unsigned long srclen = 0x293F89;
	unsigned char* src = (unsigned char*)malloc(srclen);

	unsigned long dstlen = (0x293F89 * 1.1) + 12;
	unsigned char* dst = (unsigned char*)malloc(dstlen);

	if (!src)
	{
		MessageHandler::Notify("Failed to malloc src buffer");
		Quit();
	}
	
	if (!dst)
	{
		MessageHandler::Notify("Failed to malloc dst buffer");
		Quit();
	}

	IFileStream F("/app0/SAVEDATA.DAT", IFileStream::kFlags::kFlagReadMode);
	F.Read(src, srclen);
	F.Dispose();

	int ret = compress(dst, &dstlen, src, srclen);

	IFileStream FF("/usb_0/SAVEDATA.DAT", IFileStream::kFlags::kFlagReadWriteMode | IFileStream::kFlags::kFlagCreate);
	FF.Write(dst, dstlen);
	FF.Dispose();

	SystemWrapper::UnmountUSB(0);

	while (true) { }
}
