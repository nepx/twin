
#include "windows.h"
#include "Resources.h"
#include "ModTable.h"
#include "Log.h"

static int
CommTrap(char *s)
{
	int rc = 0;
	LOGSTR((LF_CONSOLE,"COMM: trap %s\n",s));
	return rc;
}

int
trmcom(int cid)
{
	CommTrap("trmcom");
	return -2;
}


int
cclrbrk(int cid)
{
	CommTrap("cclrbrk");
	return -1;
}

typedef struct {
	char	*pqRx;
	int	cbqRx;
	char	*pqTx;
	int	cbqTx;
} qdb;

int
setque(int cid, qdb *lpqdb)
{
	CommTrap("setque");
	return -1;
}


WORD
setcom(DCB *lpdcb)
{
	CommTrap("setcom");
	return -1;
}


int
csetbrk(int cid)
{
	CommTrap("csetbrk");
	return -1;
}


int
CommWriteString(int cid, LPSTR pbuf, int size)
{
	return WriteComm(cid,pbuf,size);
}

void
ReadCommString(void)
{
	CommTrap("ReadCommString");
}



LONG
cextfcn(int cid, int fcn)
{
	CommTrap("cextfcn");
	return -1;
}


WORD
inicom(DCB *lpdcb)
{
	CommTrap("inicom");
	return -1;
}


WORD
cflush(int cid, int q)
{
	CommTrap("cflush");
	return -1;
}



void
ReactivateOpenCommPorts(void)
{
	CommTrap("ReactivateOpenCommPorts");
}


void
SuspendOpenCommPorts(void)
{
	CommTrap("SuspendOpenCommPorts");
}

WORD
ctx(int cid, int ch)
{
	CommTrap("ctx");
	return 0x4000;
}


LPWORD
cevt(int cid, int evtmask)
{
	CommTrap("cevt");
	return 0;
}


WORD
cevtGet(int cid, int evtmask)
{
	CommTrap("cevtGet");
	return 0;
}


int
reccom(int cid)
{
	CommTrap("reccom");
	return -1;
}


WORD
stacom(int cid, COMSTAT *cs)
{
	CommTrap("stacom");
	return -1;
}



WORD
sndcom(int cid, int ch)
{
	CommTrap("sndcom");
	return -1;
}


LPDCB
getdcb(int cid)
{
	CommTrap("getdcb");
	return 0;
}

int
EnableNotification(int cid, HWND hWnd, WORD winTrigger, WORD wOutTrigger)
{
	return FALSE;
}


