#include "StreamReader.h"

NAMESPACE_IO

//~~~Properties~~~//

const size_t StreamReader::Length() 
{ 
	return m_streamData.Length(); 
}

const size_t StreamReader::Position() 
{ 
	return m_streamData.Position(); 
}

//~~~Constructor~~~//

StreamReader::StreamReader(const MemoryStream &DataStream)
	:
	m_streamData(DataStream)
{
}

StreamReader::~StreamReader()
{
}

//~~~Public Functions~~~//

byte StreamReader::ReadByte()
{
	CexAssert(m_streamData.Position() + sizeof(byte) <= m_streamData.Length(), "Exceeds stream length");

	std::vector<byte> data(1);
	m_streamData.Read(data, 0, 1);
	return data[0];
}

std::vector<byte> StreamReader::ReadBytes(size_t Length)
{
	CexAssert(m_streamData.Position() + Length <= m_streamData.Length(), "Exceeds stream length");

	std::vector<byte> data(Length);
	m_streamData.Read(data, 0, Length);
	return data;
}

NAMESPACE_IOEND
