//////////////////////////////////////////////////////////////////////////
/// file: config.h
///
/// summary: configuration data types for Kvasir
//////////////////////////////////////////////////////////////////////////

#ifndef KVASIR_CONFIG_H_INCLUDED
#define KVASIR_CONFIG_H_INCLUDED

#include <string>
#include <vector>
#include <memory>

namespace kvasir
{

struct Device
{
	std::string name;
	std::string port;
	unsigned int baudRate;
	unsigned int dataBits;
	unsigned int stopBits;
	bool parityCheck;
};

class Config
{
	struct Impl;
	std::unique_ptr<Impl> m_impl;
	std::vector<Device> m_devices;

	void ReadDevices();

public:
	explicit Config(const std::string& path);
	~Config();

	const std::vector<Device>& GetDevices() const noexcept;	
};

#endif // KVASIR_CONFIG_H_INCLUDED

} // namespace kvasir