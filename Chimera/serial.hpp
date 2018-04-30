#pragma once
#ifndef CHIMERA_SERIAL_HPP
#define CHIMERA_SERIAL_HPP

/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

namespace Chimera
{
	namespace Serial
	{
		class SerialClass : public CHIMERA_INHERITED_SERIAL
		{
		public:

			Status being(BaudRate baud = SERIAL_BAUD_115200, Modes tx_mode = TX_MODE_BLOCKING, Modes rx_mode = RX_MODE_BLOCKING)
			{
				return CHIMERA_INHERITED_SERIAL::begin(baud, tx_mode, rx_mode);
			}

			Status setMode(SubPeripheral sp, Modes mode)
			{
				return CHIMERA_INHERITED_SERIAL::setMode(sp, mode);
			}

			Status write(uint8_t* data, size_t length)
			{
				return CHIMERA_INHERITED_SERIAL::cwrite(data, length);
			}

			Status write(char* string, size_t length)
			{
				return CHIMERA_INHERITED_SERIAL::cwrite(string, length);
			}

			Status write(const char* string)
			{
				return CHIMERA_INHERITED_SERIAL::cwrite(string);
			}

			Status write(const char* string, size_t length)
			{
				return CHIMERA_INHERITED_SERIAL::cwrite(string, length);
			}

			Status readPacket(uint8_t* buff, size_t buffLength)
			{
				return CHIMERA_INHERITED_SERIAL::creadPacket(buff, buffLength);
			}

			int availablePackets()
			{
				return CHIMERA_INHERITED_SERIAL::availablePackets();
			}

			void end()
			{
				CHIMERA_INHERITED_SERIAL::end();
			}

			SerialClass(const int& channel) : CHIMERA_INHERITED_SERIAL(channel)
			{
				this->serial_channel = channel;
			}

			~SerialClass() = default;
			
		private:
			int serial_channel = 0;
		};

		typedef boost::shared_ptr<SerialClass> Serial_sPtr;

		CLASS_METHOD_CHECKER(has_begin, CHIMERA_INHERITED_SERIAL, begin, Status, BaudRate, Modes, Modes);

		CLASS_METHOD_CHECKER(has_setMode, CHIMERA_INHERITED_SERIAL, setMode, Status, SubPeripheral, Modes);

		CLASS_METHOD_CHECKER(has_cwrite1, CHIMERA_INHERITED_SERIAL, cwrite, Status, uint8_t*, size_t);

		CLASS_METHOD_CHECKER(has_cwrite2, CHIMERA_INHERITED_SERIAL, cwrite, Status, char*, size_t);

		CLASS_METHOD_CHECKER(has_cwrite3, CHIMERA_INHERITED_SERIAL, cwrite, Status, const char*);

		CLASS_METHOD_CHECKER(has_cwrite4, CHIMERA_INHERITED_SERIAL, cwrite, Status, const char*, size_t);

		CLASS_METHOD_CHECKER(has_creadPacket, CHIMERA_INHERITED_SERIAL, creadPacket, Status, uint8_t*, size_t);

		CLASS_METHOD_CHECKER(has_availPkts, CHIMERA_INHERITED_SERIAL, availablePackets, int, void);

		CLASS_METHOD_CHECKER(has_end, CHIMERA_INHERITED_SERIAL, end, void, void);
	}
}

#endif 