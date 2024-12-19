#include "../common.h"
#include "packet.hpp"
#include "define.h"
#include "network.hpp"
//#include "../config.hpp"
#include "../patch/client.hpp"


using namespace client;

////////////////////////////////////////////////////////////////////////////////


void CPacket::loadConfig()
{
	m_blockSKUp = (::GetPrivateProfileInt(_T("packet"), _T("block_skup"), 0, _T("./ijl15.ini"))!=0);
	m_blockSTUp = (::GetPrivateProfileInt(_T("packet"), _T("block_stup"), 0, _T("./ijl15.ini"))!=0);
}

////////////////////////////////////////////////////////////////////////////////


inline bool CPacket::isOwnAID(u_long sid)
{
	u_long own = _mkU32(client::address(AD_AID));
	return own==sid;
}


inline bool CPacket::isSHIFT()
{
	return (::GetAsyncKeyState(VK_SHIFT)&0x8000)!=0;
}

////////////////////////////////////////////////////////////////////////////////


void CPacket::cl_change_option(u_char* buf)
{
	if (isOwnAID(_mkU32(buf+2)))
	{
		cl_change_option_own(buf);
		return;
	}
}


void CPacket::cl_change_option_own(u_char* buf)
{
	u_short param1 = _mkU16(buf+6);
	u_short param2 = _mkU16(buf+8);
	u_short param3 = _mkU16(buf+10);

	dbgprintf(0, "ZC_CHANGE_OPTION p1=%04X p2=%04X p3=%04X\n", param1, param2, param3);

	switch (param1)
	{
		case 0x0003:
			cl_ctprintf("CONDITION_TO_STUN", CL_TEXT_NOTICE2);
			break;
		case 0x0004:
			cl_ctprintf("CONDITION_TO_CURSE_SLEEP", CL_TEXT_NOTICE2);
			break;
	}

	if (param2 & 0x0001)
		cl_ctprintf("CONDITION_TO_POISON", CL_TEXT_NOTICE2);
	if (param2 & 0x0010)
	{
		cl_ctprintf("CONDITION_TO_BLIND", CL_TEXT_NOTICE2);
		*reinterpret_cast<u_short*>(buf+8) ^= 0x0010;
	}
}

////////////////////////////////////////////////////////////////////////////////


void CPacket::push_changeCondition(u_short type, u_char flag)
{
	u_long own = _mkU32(client::address(client::AD_AID));

	u_char* p = new u_char[9];
	memset(p, 0, 9);
	*reinterpret_cast<u_short*>(p) = PACKET_ZC_CHANGE_CONDITION;
	*reinterpret_cast<u_short*>(p+2) = type;
	*reinterpret_cast<u_long*>(p+4) = own;
	*reinterpret_cast<u_char*>(p+8) = flag;
	network::rbuf_push(p, 9);
}


void CPacket::changeElement(u_char flag, char* element)
{
	if (flag & 0x01)
		cl_ctprintf(CL_TEXT_NOTICE1, " �����%s�������t�^����܂����B", element);
	else
		cl_ctprintf(" ����̑��������ɖ߂�܂����B", CL_TEXT_NOTICE2);
}


void CPacket::changeState(u_char flag, char* state)
{
	if (flag & 0x01)
		cl_ctprintf(CL_TEXT_NOTICE1, "%s��ԂɂȂ�܂����B", state);
	else
		cl_ctprintf(CL_TEXT_NOTICE2, "%s��Ԃ���������܂����B", state);
}


void CPacket::cl_change_condition(u_char* buf)
{
	if (isOwnAID(_mkU32(buf+4)) == false)
		return;

	u_short type = _mkU16(buf+2);
	u_char flag = _mkU8(buf+8);

	dbgprintf(0, "ZC_CHANGE_CONDITION type=%04X flag=%02X\n", type, flag);

	switch (type)
	{
#ifdef _EXTENTION
		case CI_HALLUCINATION:
			if (flag & 0x01)
			{
				cl_ctprintf("CONDITION_TO_HALLUCINATION", CL_TEXT_NOTICE2);
				*reinterpret_cast<u_char*>(buf+8) = 0;
			}
			break;
#endif

		case CI_ELEMENTALCONV:
			if (flag == 0x00)
				releaseElementalConv(buf);
			break;

		case CI_ELEMENT_FIRE:
			changeElement(flag, "��");
			break;
		case CI_ELEMENT_WATER:
			changeElement(flag, "��");
			break;
		case CI_ELEMENT_WIND:
			changeElement(flag, "��");
			break;
		case CI_ELEMENT_GROUND:
			changeElement(flag, "�n");
			break;

		case CI_BERSERK:
			changeState(flag, "�o�[�T�N");
			break;
		case CI_ASSUMPTIO:
			changeState(flag, "�A�X���v�e�B�I");
			break;
#ifdef _EXTENTION
		case CI_MAGICPOWER:
			changeState(flag, "���@�͑���");
			break;
#endif

		case CI_ELEMENT_SHADOW:
			changeElement(flag, "��");
			break;
		case CI_ELEMENT_GHOST:
			changeElement(flag, "�O");
			break;

		case CI_SOULLINK:
			changeState(flag, "�����N");
			break;

		case CI_KAITE:
			changeState(flag, "�J�C�g");
			break;
		case CI_KAUPE:
			changeState(flag, "�J�E�v");
			break;

		case CI_TRUESIGHT:
			changeState(flag, "True Sight");
			break;

#ifdef _EXTENTION
		case CI_CLOSECONFINE_OWN:
		case CI_CLOSECONFINE_TGT:
			changeState(flag, "�N���[�Y�R���t�@�C��");
			break;
#endif

	}
}


void CPacket::releaseElementalConv(u_char* buf)
{
	switch (m_currentCI)
	{
		case CI_ELEMENT_SHADOW:
			changeElement(0x00, "��");
			break;
		case CI_ELEMENT_FIRE:
			changeElement(0x00, "��");
			break;
		case CI_ELEMENT_WATER:
			changeElement(0x00, "��");
			break;
		case CI_ELEMENT_GROUND:
			changeElement(0x00, "�n");
			break;
		case CI_ELEMENT_WIND:
			changeElement(0x00, "��");
			break;
	}

	*reinterpret_cast<u_short*>(buf+2) = m_currentCI;
	m_currentCI = CI_NONE;
}

////////////////////////////////////////////////////////////////////////////////


void CPacket::cl_notice_actor(u_char* buf)
{
//	u_short param1 = _mkU16(buf+8);
//	u_short param2 = _mkU16(buf+10);
//	u_short param3 = _mkU16(buf+12);
}

////////////////////////////////////////////////////////////////////////////////


void CPacket::cl_notice_useitem(u_char* buf)
{
	if (isOwnAID(_mkU32(buf+6)) == false)
		return;
	if (_mkU8(buf+12) != 0x01)
		return;

	// 12020#���ꂽ��#
	// 12114#�Α����R���o�[�^�[#
	// 12115#�������R���o�[�^�[#
	// 12116#�n�����R���o�[�^�[#
	// 12117#�������R���o�[�^�[#

	switch (_mkU16(buf+4))
	{
		case 12020:
			m_currentCI = CI_ELEMENT_SHADOW;
			changeElement(0x01, "��");
			push_changeCondition(m_currentCI, 0x01);
			break;
		case 12114:
			m_currentCI = CI_ELEMENT_FIRE;
			changeElement(0x01, "��");
			push_changeCondition(m_currentCI, 0x01);
			break;
		case 12115:
			m_currentCI = CI_ELEMENT_WATER;
			changeElement(0x01, "��");
			push_changeCondition(m_currentCI, 0x01);
			break;
		case 12116:
			m_currentCI = CI_ELEMENT_GROUND;
			changeElement(0x01, "�n");
			push_changeCondition(m_currentCI, 0x01);
			break;
		case 12117:
			m_currentCI = CI_ELEMENT_WIND;
			changeElement(0x01, "��");
			push_changeCondition(m_currentCI, 0x01);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////


void CPacket::local_debugMsg()
{
	cl_ctprintf(CL_TEXT_NOTICE5, "[DEBUG] AID=%08X", _mkU32(client::address(AD_AID)));
	cl_ctprintf(CL_TEXT_NOTICE5, "[DEBUG] TRUESIGHTE=%02X", m_truesight);
}


bool CPacket::local_trueSight()
{
	if (isSHIFT())
		return false;

	m_truesight ^= 0x01;
	changeState(m_truesight, "True Sight");

	push_changeCondition(CI_TRUESIGHT, m_truesight);

	return true;
}


bool CPacket::local_0x0112()
{
	if (isSHIFT())
		return false;

	cl_ctprintf(CL_TEXT_NOTICE5, "PACKET_CZ_SKILLUP_REQ BLOCKED");

	return true;
}


bool CPacket::local_0x00BB()
{
	if (isSHIFT())
		return false;

	cl_ctprintf(CL_TEXT_NOTICE5, "PACKET_CZ_STATUSUP_REQ BLOCKED");

	return true;
}