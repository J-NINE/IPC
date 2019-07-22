// EthernetLayer.cpp: implementation of the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ipc.h"
#include "EthernetLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEthernetLayer::CEthernetLayer( char* pName )
: CBaseLayer( pName )
{
	ResetHeader( ) ;
}

CEthernetLayer::~CEthernetLayer()
{
}

void CEthernetLayer::ResetHeader()
{
	memset( m_sHeader.enet_dstaddr, 0, 6 ) ;
	memset( m_sHeader.enet_srcaddr, 0, 6 ) ;
	memset( m_sHeader.enet_data, ETHER_MAX_DATA_SIZE, 6 ) ;
	m_sHeader.enet_type = 0 ;
}

unsigned char* CEthernetLayer::GetSourceAddress()
{
	return m_sHeader.enet_srcaddr ;
}

unsigned char* CEthernetLayer::GetDestinAddress()
{
//////////////////////// fill the blank ///////////////////////////////
	return m_sHeader.enet_dstaddr;

	/* p.29
	EthernetLayer 헤더파일에 정의되어 있는 GetDestinAddress()함수는
	역시 EthernetLayer 헤더파일에 구조체로 정의되어 있는 _ETHERNET_HEADER의
	enet_dstaddr을 리턴하게끔 되어있다.
	(EthernetLayer 헤더파일을 보면 _ETHERNET_HEADER는 m_sHeader로 자료형을 바꿔서 사용한다.)
	Ethernet 계층에 도착한 데이터는 여러 값을 가지고 있는 데, 그 중에서 목적지에 대한
	값을 가지고 있는 array 값을 반환하게끔 한다.
	*/
///////////////////////////////////////////////////////////////////////
}

void CEthernetLayer::SetSourceAddress(unsigned char *pAddress)
{
//////////////////////// fill the blank ///////////////////////////////
	memcpy(m_sHeader.enet_srcaddr, pAddress, 6);

///////////////////////////////////////////////////////////////////////
}

void CEthernetLayer::SetDestinAddress(unsigned char *pAddress)
{
	memcpy( m_sHeader.enet_dstaddr, pAddress, 6 ) ;
}

BOOL CEthernetLayer::Send(unsigned char *ppayload, int nlength)
{
	memcpy( m_sHeader.enet_data, ppayload, nlength ) ;

	BOOL bSuccess = FALSE ;
//////////////////////// fill the blank ///////////////////////////////
	bSuccess = mp_UnderLayer->Send( (unsigned char*)&m_sHeader, nlength*ETHER_HEADER_SIZE);

///////////////////////////////////////////////////////////////////////

	return bSuccess ;
}

BOOL CEthernetLayer::Receive( unsigned char* ppayload )
{
	PETHERNET_HEADER pFrame = (PETHERNET_HEADER) ppayload ;

	BOOL bSuccess = FALSE ;
//////////////////////// fill the blank ///////////////////////////////
	bSuccess = mp_aUpperLayer[0]->Receive( (unsigned char*)pFrame->enet_data);

///////////////////////////////////////////////////////////////////////

	return bSuccess ;
}