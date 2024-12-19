#include "packetTable.hpp"


#pragma warning(disable: 4312)	// greater size

////////////////////////////////////////////////////////////////////////////////


CPacketTable::CPacketTable()
{
	m_table = 0;
	m_lastOp = 0;
}

////////////////////////////////////////////////////////////////////////////////

// PacketLength table�\�z

void CPacketTable::_build(DWORD address, int pos)
{
	if (m_table != 0)	// ���ɍ\�z�ς�
		return;

	if (address == 0)	// �A�h���X����
		return;


	PTBL_NODE_PTR root;

	if (pos == 0)
		root = searchRoot(address);
	else
		root = reinterpret_cast<PTBL_NODE_PTR>(*reinterpret_cast<DWORD*>(address+pos+0x04));

	if (root == 0)	// rootNode��������Ȃ�
		return;

	PTBL_NODE_PTR last = reinterpret_cast<PTBL_NODE_PTR>(root->r);
	if (last == 0)	// lastNode���Ȃ�
		return;

	m_lastOp = last->op;
	m_table = new int[m_lastOp+1];
	::ZeroMemory(m_table, sizeof(int)*(m_lastOp+1));


	searchTree(reinterpret_cast<PTBL_NODE_PTR>(root->p));
}

// 2���ؒT��

void CPacketTable::searchTree(PTBL_NODE_PTR current)
{
	if (current == 0)
		return;

	if (current->op <= m_lastOp)
		m_table[current->op] = current->length;

	searchTree(reinterpret_cast<PTBL_NODE_PTR>(current->l));
	searchTree(reinterpret_cast<PTBL_NODE_PTR>(current->r));
}

////////////////////////////////////////////////////////////////////////////////


CPacketTable::PTBL_NODE_PTR CPacketTable::searchRoot(DWORD address)
{
	//for (DWORD d=address; d<address+0xFF; d+=0x04)
	// 2005-09-15bSakexe����͈͊g��
	for (DWORD d=address; d<address+0x02FFFF; d+=0x04)
	{
		if (*reinterpret_cast<DWORD*>(d) != 0x00004000)
			continue;

		PTBL_NODE_PTR root = reinterpret_cast<PTBL_NODE_PTR>(*reinterpret_cast<DWORD*>(d+4));
		if (IsBadReadPtr(reinterpret_cast<void*>(root->r), sizeof(PTBL_NODE)) == TRUE)
			continue;

		return root;
	}

	return NULL;
}
