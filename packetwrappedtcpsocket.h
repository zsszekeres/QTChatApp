#ifndef PACKETWRAPPEDTCPSOCKET_H
#define PACKETWRAPPEDTCPSOCKET_H

#include <QTcpSocket>
#include <QList>

/*!
 * \brief The PacketWrappedTcpSocket class is a specialized QTcpSocket for sending packets rather than just streaming the data.
 *
 * The sender extends the packets with their size.
 * The receiver splits the stream into packets and waits for them to completely arrive.
 *
 * The class collects the incoming packets into a FIFO buffer, from there they can be queried.
 */
class PacketWrappedTcpSocket : public QTcpSocket
{
	Q_OBJECT

	static constexpr int bytesOfPacketLengthData=8;

public:
	explicit PacketWrappedTcpSocket(QObject *parent = nullptr);

	/*!
	 * \brief Checks if there are any packets in the internal buffer.
	 * \return Returns true if the internal buffer has at least one packet.
	 */
	bool hasPendingPackets() const;

	/*! \brief Returns the number of packets in the internal buffer. */
	unsigned int numberOfPendingPackets() const;

	/*! \brief Returns the oldest packet in the buffer without removing it. Does not check if the buffer is empty. */
	const QByteArray &getNextPacket() const;

	/*! \brief Removes the oldest packet in the buffer. Does not check if the buffer is empty. */
	void removeNextPacket();

	/*! \brief Returns a constant reference to the buffer. */
	const QList<QByteArray> &getAllPackets() const;

	/*! \brief Clears the buffer. */
	void removeAllPackets();

	// Overwriting or deleting base methods
	/*!
	 * \brief Wraps the data into a packet, sending its size before the data itself.
	 * \param data The byte array to be sent.
	 * \return Returns the number of bytes written (as defined in QIODevice::write).
	 */
	qint64 write(const QByteArray &data);
	qint64 write(const char *data, qint64 maxSize) = delete;
	qint64 write(const char *data) = delete;

	QByteArray readAll() = delete;
	qint64 read(char *data, qint64 maxSize) = delete;
	QByteArray read(qint64 maxSize) = delete;

signals:
	/*! \brief Emitted when one or more packets have fully arrived, and are ready for processing. */
	void packetsReady();

private:
	// For size encoding an decoding into text (hexa format)
	static inline QByteArray transformPacketSizeIntoByteArray(int32_t value)
	{
		return QByteArray::number(value, 16).rightJustified(bytesOfPacketLengthData, '0');
	}
	static inline int32_t getPacketSizeFromByteArray(const QByteArray &bytearray)
	{
		return bytearray.left(bytesOfPacketLengthData).toInt(nullptr,16);
	}

private slots:
	void dataArrived();
	void parseDataInBuffer();
	bool parseOnePacketInBuffer();

private:
	QByteArray processedDataByteArray;
	QList<QByteArray> parsedPackageBuffer;
};

#endif // PACKETWRAPPEDTCPSOCKET_H
