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

			Status being(Chimera::Serial::BaudRate baud = Chimera::Serial::BaudRate::SERIAL_BAUD_115200, Modes tx_mode = Chimera::Serial::Modes::BLOCKING, Modes rx_mode = Chimera::Serial::Modes::BLOCKING)
			{
				return CHIMERA_INHERITED_SERIAL::cbegin(baud, tx_mode, rx_mode);
			}

			Status setMode(SubPeripheral sp, Modes mode)
			{
				return CHIMERA_INHERITED_SERIAL::csetMode(sp, mode);
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

		CLASS_METHOD_CHECKER(has_cbegin, CHIMERA_INHERITED_SERIAL, cbegin, Status, BaudRate, Modes, Modes);

		CLASS_METHOD_CHECKER(has_csetMode, CHIMERA_INHERITED_SERIAL, csetMode, Status, SubPeripheral, Modes);

		CLASS_METHOD_CHECKER(has_cwrite1, CHIMERA_INHERITED_SERIAL, cwrite, Status, uint8_t*, size_t);

		CLASS_METHOD_CHECKER(has_cwrite2, CHIMERA_INHERITED_SERIAL, cwrite, Status, char*, size_t);

		CLASS_METHOD_CHECKER(has_cwrite3, CHIMERA_INHERITED_SERIAL, cwrite, Status, const char*);

		CLASS_METHOD_CHECKER(has_cwrite4, CHIMERA_INHERITED_SERIAL, cwrite, Status, const char*, size_t);

		CLASS_METHOD_CHECKER(has_creadPacket, CHIMERA_INHERITED_SERIAL, creadPacket, Status, uint8_t*, size_t);

		CLASS_METHOD_CHECKER(has_availPkts, CHIMERA_INHERITED_SERIAL, availablePackets, uint32_t, void);

		CLASS_METHOD_CHECKER(has_end, CHIMERA_INHERITED_SERIAL, end, void, void);
	}
}

#endif 