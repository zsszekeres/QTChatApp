#include "packetwrappedtcpsocket.h"

PacketWrappedTcpSocket::PacketWrappedTcpSocket(QObject *parent) :
	QTcpSocket(parent)
{
	connect(this, &QIODevice::readyRead, this, &PacketWrappedTcpSocket::dataArrived);
}

bool PacketWrappedTcpSocket::hasPendingPackets() const
{
	return !parsedPackageBuffer.isEmpty();
}

const QByteArray &PacketWrappedTcpSocket::getNextPacket() const
{
	return parsedPackageBuffer.front();
}

void PacketWrappedTcpSocket::removeNextPacket()
{
	if (!parsedPackageBuffer.isEmpty())
		parsedPackageBuffer.removeFirst();
}

unsigned int PacketWrappedTcpSocket::numberOfPendingPackets() const
{
	return parsedPackageBuffer.size();
}

const QList<QByteArray> &PacketWrappedTcpSocket::getAllPackets() const
{
	return parsedPackageBuffer;
}

void PacketWrappedTcpSocket::removeAllPackets()
{
	parsedPackageBuffer.clear();
}

qint64 PacketWrappedTcpSocket::write(const QByteArray &data)
{
	if (data.size()==0) return 0;
	QByteArray realData;
	realData.append(transformPacketSizeIntoByteArray(data.size()+bytesOfPacketLengthData));
    realData.append(data);
	return QTcpSocket::write(realData);
}

void PacketWrappedTcpSocket::dataArrived()
{
	QByteArray allData=QTcpSocket::readAll();
	processedDataByteArray.append(allData);
	parseDataInBuffer();
}

void PacketWrappedTcpSocket::parseDataInBuffer()
{
	bool addedNewPackets=false;
	bool parsingFinished=false;
	while (!parsingFinished)
	{
		if (parseOnePacketInBuffer()) addedNewPackets=true;
		else parsingFinished=true;
	}

	if (addedNewPackets) emit packetsReady();
}

bool PacketWrappedTcpSocket::parseOnePacketInBuffer()
{
	int currentBufferSize=processedDataByteArray.size();
	if (currentBufferSize<=bytesOfPacketLengthData) return false;

	int packetSize=getPacketSizeFromByteArray(processedDataByteArray);
	if (packetSize>currentBufferSize) return false; // Full packet did not arrive yet

	processedDataByteArray.remove(0,bytesOfPacketLengthData); // Dump length data
	QByteArray onePacket=processedDataByteArray.left(packetSize-bytesOfPacketLengthData);
	processedDataByteArray.remove(0,packetSize-bytesOfPacketLengthData);

	parsedPackageBuffer.append(onePacket);
	return true;
}
