/*
 *
 */

#pragma once

#include <format>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>
#include <string>

#ifdef WHATLOG_DLL
#  define WHATLOG_EXPORT __declspec(dllexport)
#else
#  define WHATLOG_EXPORT __declspec(dllimport)
#endif

namespace whatlog
{
	class logger
	{
	public:
		static void disable_logging();
		static void initialize_console_logger();
		static void initialize_file_logger(const boost::filesystem::path& log_directory, std::string log_file_name);
	public:
		explicit logger(const std::string& location);
		logger(const std::string& location, const std::string& fileFilter);

		template<typename ...args_t>
		void info(std::format_string<args_t...> text, args_t ... args)
		{
			info_internal(std::format(text, args...));
		}

		template<typename ...args_t>
		void warning(std::format_string<args_t...> text, args_t ... args)
		{
			warning_internal(std::format(text, args...));
		}

		template<typename ...args_t>
		void error(std::format_string<args_t...> text, args_t ... args)
		{
			error_internal(std::format(text, args...));
		}

		
	private:
		WHATLOG_EXPORT void info_internal(const std::string& message);
		WHATLOG_EXPORT void warning_internal(const std::string& message);
		WHATLOG_EXPORT void error_internal(const std::string& message);

		typedef boost::log::sources::severity_logger<boost::log::trivial::severity_level> trivial_severity_logger;
		trivial_severity_logger m_logger;
	};

	void WHATLOG_EXPORT rename_thread(const boost::thread::native_handle_type& handle_id, const std::string& name);
	std::string WHATLOG_EXPORT thread_name();
}
