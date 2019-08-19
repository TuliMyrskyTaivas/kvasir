//////////////////////////////////////////////////////////////////////////
/// file: scanner.h
///
/// summary: Scanner device controller.h
//////////////////////////////////////////////////////////////////////////

#include <QtSerialPort/QSerialPort>
#include <string_view>
#include <cassert>
#include <regex>

#include "scanner.h"
#include "config.h"
#include "logger.h"

namespace kvasir
{

//////////////////////////////////////////////////////////////////////////
struct Scanner::Impl
{
	QSerialPort port;

	~Impl()
	{
		if (port.isOpen())
		{
			port.close();
		}
	}
};

//////////////////////////////////////////////////////////////////////////
Scanner::~Scanner() = default;

//////////////////////////////////////////////////////////////////////////
Scanner::Scanner()
	: m_impl(std::make_unique<Impl>())
	, m_inProgrammingMode(false)
{	
}

//////////////////////////////////////////////////////////////////////////
std::vector<std::string> SplitString(const std::string& str)
{
	static std::regex delimiter(",");
	std::sregex_token_iterator i(str.cbegin(), str.cend(), delimiter, -1);
	std::sregex_token_iterator end;

	std::vector<std::string> result;

	for (; i != end; ++i)
	{
		result.push_back(*i);
	}

	// Correction for std::sregex_token_iterator behavior
	if (',' == str.back())
	{
		result.emplace_back(std::string{});
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
QSerialPort::DataBits ToDataBits(const unsigned int bits)
{
	switch (bits)
	{
	case 8:
		return QSerialPort::Data8;
	case 7:
		return QSerialPort::Data7;
	case 6:
		return QSerialPort::Data6;
	case 5:
		return QSerialPort::Data5;
	default:
		throw std::logic_error("invalid number of data bits: " + std::to_string(bits));
	}
}

//////////////////////////////////////////////////////////////////////////
QSerialPort::StopBits ToStopBits(const unsigned int bits)
{
	switch (bits)
	{
	case 1:
		return QSerialPort::OneStop;
	case 2:
		return QSerialPort::TwoStop;
	case 3:
		return QSerialPort::OneAndHalfStop;
	default:
		throw std::logic_error("invalid number of stop bits: " + std::to_string(bits));
	}
}

//////////////////////////////////////////////////////////////////////////
QSerialPort::Parity ToParity(const bool parity)
{
	return parity ? QSerialPort::EvenParity : QSerialPort::NoParity;
}

//////////////////////////////////////////////////////////////////////////
void Scanner::Connect(const Device& device)
try
{
	assert(!m_impl->port.isOpen() && "serial port already open");
	auto& port = m_impl->port;

	port.setPortName(QString::fromStdString(device.port));
	port.setBaudRate(device.baudRate);
	port.setDataBits(ToDataBits(device.dataBits));
	port.setStopBits(ToStopBits(device.stopBits));
	port.setParity(ToParity(device.parityCheck));
	port.setFlowControl(QSerialPort::NoFlowControl);
	if (!port.open(QIODevice::ReadWrite))
	{
		throw std::runtime_error(port.errorString().toStdString());
	}

	Logger::GetInstance().Debug() << "connected to port " << device.port;
}
catch (const std::exception& e)
{
	throw std::runtime_error("failed to connect to port " + device.port + ": " + std::string(e.what()));
}

//////////////////////////////////////////////////////////////////////////
void Scanner::Disconnect()
{
	assert(m_impl->port.isOpen() && "serial port is not open");
}

//////////////////////////////////////////////////////////////////////////
Scanner::Response Scanner::IssueCommand(const std::string& command, size_t responseSize) const
{
	// All commands are 3 letters sequences
	const std::string_view cmdName(command.data(), 3);

	m_impl->port.write(command.c_str());		

	// Wait for the end of data (all responses are finished with '\r')
	QByteArray buf;
	do
	{
		m_impl->port.waitForReadyRead();		
		buf += m_impl->port.readAll();
	} while (buf.back() != '\r');

	if (Logger::GetInstance().IsVerbose())
	{
		Logger::GetInstance().Debug() << "scanner response: "
			<< std::string(buf.cbegin(), std::prev(buf.cend(), 1));
	}
	
	// Check response format: it should be suffixed with the command's name
	if (buf.size() < 4 || std::string_view(buf.data(), 3) != cmdName)
		throw std::runtime_error("invalid " + std::string(cmdName) + " response: wrong prefix");

	// Build the list of response values
	const std::string response(std::next(buf.cbegin(), 4), std::prev(buf.cend()));
	const Response result = SplitString(response);
	if (responseSize != result.size())
		throw std::runtime_error("invalid " + std::string(cmdName) +
			" response length: " + std::to_string(result.size()));

	return result;
}

//////////////////////////////////////////////////////////////////////////
bool Scanner::InProgrammingMode() const noexcept
{
	return m_inProgrammingMode;
}

//////////////////////////////////////////////////////////////////////////
void Scanner::EnterProgrammingMode() const
{
	assert(m_inProgrammingMode == false);
	Logger::GetInstance().Debug() << "Entering programming mode";

	const auto result = IssueCommand("PRG\r", 1);
	if ("OK" != result.front())
		throw std::runtime_error("failed to enter programming mode: " + result.front());
	
	const_cast<bool&>(m_inProgrammingMode) = true;
}

//////////////////////////////////////////////////////////////////////////
void Scanner::ExitProgrammingMode() const
{
	assert(m_inProgrammingMode == true);
	Logger::GetInstance().Debug() << "Leaving programming mode";

	const auto result = IssueCommand("EPG\r", 1);
	if ("OK" != result.front())
		throw std::runtime_error("failed to exit programming mode: " + result.front());

	const_cast<bool&>(m_inProgrammingMode) = false;
}

//////////////////////////////////////////////////////////////////////////
std::string Scanner::GetModel() const
{
	return IssueCommand("MDL\r", 1).front();
}

//////////////////////////////////////////////////////////////////////////
std::string Scanner::GetFirmwareVersion() const
{
	return IssueCommand("VER\r", 1).front();
}

//////////////////////////////////////////////////////////////////////////
Modulation ModFromString(const std::string& mod)
{
	if ("AM" == mod)
		return Modulation::AM;
	else if ("FM" == mod)
		return Modulation::FM;
	else if ("NFM" == mod)
		return Modulation::NFM;
	else if ("WFM" == mod)
		return Modulation::WFM;
	else if ("FMB" == mod)
		return Modulation::FMB;

	Logger::GetInstance().Debug() << "unknown modulation: " << mod;
	return Modulation::None;
}

//////////////////////////////////////////////////////////////////////////
ReceptionStatus Scanner::GetReceptionStatus() const
{
	const auto result = IssueCommand("GLG\r", 12);		

	ReceptionStatus status{};
	if (result.front().empty())
		return status;

	const auto& systemTag = result[9];
	const auto& channelTag = result[10];
	const auto& p25nac = result[11];

	status.freq = result.front();	
	status.mod = ModFromString(result[1]);
	status.att = static_cast<bool>(std::stoi(result[2]));
	status.code = static_cast<CtcssDcsCode>(std::stoi(result[3]));
	status.site = result[4];
	status.group = result[5];
	status.channel = result[6];
	status.squelch = static_cast<bool>(std::stoi(result[7]));
	status.mute = static_cast<bool>(std::stoi(result[8]));
	
	if ("NONE" != systemTag)
		status.systemTag = std::stoi(systemTag);
		
	if ("NONE" != channelTag)
		status.channelTag = std::stoi(channelTag);
		
	if ("NONE" != p25nac)
		status.p25Nac = std::stoi(p25nac);
	
	return status;
}

} // namespace kvasir