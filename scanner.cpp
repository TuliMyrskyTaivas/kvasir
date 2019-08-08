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
{}

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
std::vector<std::string> SplitString(const std::string& str)
{
	static std::regex delimiter(",");
	std::sregex_token_iterator i(str.cbegin(), str.cend(), delimiter, -1);
	std::sregex_token_iterator end;

	std::vector<std::string> result;
	for (; i != end; ++i)
		result.push_back(*i);

	return result;
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
std::string Scanner::IssueCommand(const std::string& command) const
{
	// All commands are 3 letters sequences
	const std::string_view cmdName(command.data(), 3);

	m_impl->port.write(command.c_str());
	m_impl->port.waitForReadyRead();
	const auto response = m_impl->port.readAll();
	if (response.size() < 4 || std::string_view(response.data(), 3) != cmdName)
		throw std::runtime_error("invalid " + std::string(cmdName) + " response");
	return std::string(std::next(response.cbegin(), 4), response.cend());
}

//////////////////////////////////////////////////////////////////////////
std::string Scanner::GetModel() const
{
	return IssueCommand("MDL\r");	
}

//////////////////////////////////////////////////////////////////////////
std::string Scanner::GetFirmwareVersion() const
{
	return IssueCommand("VER\r");	
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
	const std::string cmdResult = IssueCommand("GLG\r");
	Logger::GetInstance().Info() << "Scanner result: " << cmdResult;
	const auto result = SplitString(cmdResult);	

	ReceptionStatus status;
	if (result.front().empty())
		return status;

	status.freq = result.front();
	if (result.size() > 1)
		status.mod = ModFromString(result[1]);
	if (result.size() > 2)
		status.att = static_cast<bool>(std::stoi(result[2]));
	if (result.size() > 3)
		status.code = static_cast<CtcssDcsCode>(std::stoi(result[3]));
	if (result.size() > 4)
		status.site = result[4];
	if (result.size() > 5)
		status.group = result[5];
	if (result.size() > 6)
		status.channel = result[6];
	if (result.size() > 7)
		status.squelch = static_cast<bool>(std::stoi(result[7]));
	if (result.size() > 8)
		status.mute = static_cast<bool>(std::stoi(result[8]));
	if (result.size() > 9)
	{
		const auto& systemTag = result[9];
		if ("NONE" != systemTag)
			status.systemTag = std::stoi(systemTag);
	}
	if (result.size() > 10)
	{
		const auto& channelTag = result[10];
		if ("NONE" != channelTag)
			status.channelTag = std::stoi(channelTag);
	}
	if (result.size() > 11)
	{
		const auto& p25nac = result[11];
		if ("NONE" != p25nac)
			status.p25Nac = std::stoi(p25nac);
	}

	return status;
}


} // namespace kvasir