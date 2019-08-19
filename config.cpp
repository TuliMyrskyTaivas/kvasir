//////////////////////////////////////////////////////////////////////////
/// file: config.cpp
///
/// summary: configuration data types for Kvasir
//////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "logger.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QVariant>

#include <stdexcept>

namespace kvasir
{

struct Config::Impl
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

	Impl(const std::string& configPath)
	{
		Logger::GetInstance().Debug() << "reading configuration file " << configPath;
		db.setDatabaseName(QString(configPath.c_str()));
		if (!db.open())
		{
			throw std::runtime_error("failed to open configuration file " + configPath + ": " + db.lastError().text().toStdString());
		}
	}

	~Impl()
	{		
		db.close();
	}
};

Config::~Config() = default;

//////////////////////////////////////////////////////////////////////////
Config::Config(const std::string& path)
	:m_impl(std::make_unique<Impl>(path))
{
	ReadDevices();
}

//////////////////////////////////////////////////////////////////////////
const std::vector<Device>& Config::GetDevices() const noexcept
{
	return m_devices;
}

//////////////////////////////////////////////////////////////////////////
void Config::ReadDevices()
{
	QSqlQuery query("select * from devices");
	while (query.next())
	{
		const auto name = query.value(0).toString().toStdString();
		const auto port = query.value(1).toString().toStdString();
		const unsigned int bauds = query.value(2).toUInt();
		const unsigned int dataBits = query.value(3).toUInt();
		const unsigned int stopBits = query.value(4).toUInt();
		const bool parityCheck = query.value(5).toBool();
		m_devices.emplace_back(Device{ name, port, bauds, dataBits, stopBits, parityCheck });
	}
	Logger::GetInstance().Debug() << m_devices.size() << " devices are configured";
}

} // namespace kvasir