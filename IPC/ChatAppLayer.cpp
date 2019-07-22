// ChatAppLayer.cpp: implementation of the CChatAppLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ipc.h"
#include "ChatAppLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatAppLayer::CChatAppLayer( char* pName ) 
: CBaseLayer( pName ), 
  mp_Dlg( NULL )
{
	ResetHeader( ) ;
}

CChatAppLayer::~CChatAppLayer()
{

}

void CChatAppLayer::SetSourceAddress(unsigned int src_addr)
{
	m_sHeader.app_srcaddr = src_addr ;
}


void CChatAppLayer::SetDestinAddress(unsigned int dst_addr)
{
	m_sHeader.app_dstaddr = dst_addr ;
}

void CChatAppLayer::ResetHeader( )
{
	m_sHeader.app_srcaddr = 0x00000000 ;
	m_sHeader.app_dstaddr = 0x00000000 ;
	m_sHeader.app_length  = 0x0000 ;
	m_sHeader.app_type    = 0x00 ;
	memset( m_sHeader.app_data, 0, APP_DATA_SIZE ) ;
}

unsigned int CChatAppLayer::GetSourceAddress()
{
	return m_sHeader.app_srcaddr ;
}

unsigned int CChatAppLayer::GetDestinAddress()
{
	return m_sHeader.app_dstaddr ;
}

BOOL CChatAppLayer::Send(unsigned char *ppayload, int nlength)
{
	m_sHeader.app_length = (unsigned short) nlength ;

	BOOL bSuccess = FALSE ;
//////////////////////// fill the blank ///////////////////////////////
	memcpy(m_sHeader.app_data, ppayload, nlength>APP_DATA_SIZE ? APP_DATA_SIZE:nlength);
	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength+APP_HEADER_SIZE);

//////////////////////////////////////////////////////////////////////

	return bSuccess ;
}

BOOL CChatAppLayer::Receive( unsigned char* ppayload )
{
	PCHAT_APP_HEADER app_hdr = (PCHAT_APP_HEADER) ppayload ;

	if ( app_hdr->app_dstaddr == m_sHeader.app_srcaddr || 
	   ( app_hdr->app_srcaddr != m_sHeader.app_srcaddr &&
		 app_hdr->app_dstaddr == (unsigned int) 0xff ) )
	{
//////////////////////// fill the blank ///////////////////////////////
	unsigned char GetBuff[APP_DATA_SIZE];
	memset(GetBuff, '\0', APP_DATA_SIZE);
	memcpy(GetBuff, app_hdr->app_data, app_hdr->app_length>APP_DATA_SIZE? APP_DATA_SIZE: app_hdr->app_length);
	CString Msg;
	
	if(app_hdr->app_dstaddr==(unsigned int)0xff)
	{
		Msg.Format("[%d:BROADCAST] %s", app_hdr->app_srcaddr, (char*)GetBuff);
	}
	else
	{
		Msg.Format("[%d:%d] %s", app_hdr->app_srcaddr, app_hdr->app_dstaddr, (char*)GetBuff);
	}
	mp_aUpperLayer[0] ->Receive((unsigned char*) Msg.GetBuffer(0));

	/*p.27
	매개변수로 받은 ppayload를 강제 casting해서 app_hdr 변수로 할당한다.
	if문 안의 조건을 살펴보면, app_hdr의 목적 주소와 현재 데이터의 출처 주소가 같거나(조건1)
	app_hdr의 출처 주소와 현재 데이터의 출처주소가 같지 않고 app_hdr의 목적 주소가 0xff일 때(조건2),
	blank 안의 구문을 실행하게 된다. 여기서 현재 데이터는 m_sHeader을 말하는데 ResetHeader( ) 함수를
	통해 이 구조체 안의 변수들의 값이 모두 초기화된다.
	blank 안의 구문을 살펴보면 먼저 memset과 memcpy 함수를 통해서 현재 데이터의 크기만큼 초기화를 먼저 해주고,
	현재 데이터의 크기와 app_hdr의 크기를 비교해서 GetBuff 변수에 복사한다.
	그리고 현재 데이터의 목적 주소가 0xff(위의 ResetHeader함수를 통해 초기화해준 값)과 같다면 전송하고 포맷을 출력해준다.
	0xff값과 다르다면 if문을 만족할 때보다 인자를 하나 더 출력해준다.(솔직히 왜 이러는지 잘 모르겠다..)
	그리고 aUpperLayer의 array 첫 번째에 위에서 송신받은 GetBuffer값을 성공적으로 받는다면 true값을 반환한다.
	*/
///////////////////////////////////////////////////////////////////////
		return TRUE ;
	}
	else
		return FALSE ;
}