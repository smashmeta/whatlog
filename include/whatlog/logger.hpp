/*
 *
 */

#pragma once

#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>
#include <string>


namespace whatlog
{
	class logger
	{
	public:
		static void initialize_console_logger();
		static void initialize_file_logger(const boost::filesystem::path& log_directory, std::string log_file_name);
	public:
		logger(const std::string& location);

		void info(const std::string& message);
		void warning(const std::string& message);
		void error(const std::string& message);
	private:
		typedef boost::log::sources::severity_logger<boost::log::trivial::severity_level> trivial_severity_logger;
		trivial_severity_logger m_logger;
	};

	void rename_thread(const boost::thread::native_handle_type& handle_id, const std::string& name);
	std::string thread_name();
}