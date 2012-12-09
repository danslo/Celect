#ifndef NETWORK_PEER_TCPIP_WIN32_H
#define NETWORK_PEER_TCPIP_WIN32_H

#include <WinSock2.h>

class NetworkPeerTCPIPWin32
{

public:

	int a[5];
	SOCKET s;

	virtual void unk0();
	virtual void unk1();
	virtual void shutdown();									// +8
	virtual void unk2();
	virtual void unk3();
	virtual void unk4();
	virtual void unk5();
	virtual void sendv(int a2, int a3, int a4);					// +28
	virtual void unk6();
	virtual void unk7();
	virtual void recievBuffer();								// +40
	virtual void unk8();
	virtual void unk9();
	virtual void unk10();
	virtual void unk11();
	virtual void setFastPathListener();							// +60
	virtual void unk12();
	virtual void unk13();
	virtual void unk14();
	virtual void unk15();
	virtual void unk16();
	virtual void unk17();
	virtual void unk18();
	virtual void unk19();
	virtual void gotNewBuffer();			// +96
	virtual void unk20();
	virtual void invalidate();				// +104

};

#endif