/*
 *
 */

#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>
#include <boost/dll.hpp>
#include <thread>
#include <shared_mutex>
#include <windows.h>
#include "whatlog/logger.hpp"


namespace whatlog
{
	namespace 
	{
		// map containing name alias for different threads spawned and renamed
		static std::map<std::thread::id, std::string> thread_map;

		static std::shared_mutex thread_map_mutex;
	}

	void rename_thread(const boost::thread::native_handle_type& handle_id, const std::string& name)
	{
		{
			std::unique_lock<std::shared_mutex> lock(thread_map_mutex);
			thread_map[std::this_thread::get_id()] = name;
		}
		
		HANDLE native_handle_id = (HANDLE)handle_id;
		std::wstring wide_name(name.begin(), name.end());
		SetThreadDescription(native_handle_id, wide_name.c_str());
	}

	std::string thread_name()
	{
		{
			std::shared_lock<std::shared_mutex> lock(thread_map_mutex);
			auto itr_thread_name = thread_map.find(std::this_thread::get_id());
			if (itr_thread_name != thread_map.end())
			{
				return itr_thread_name->second;
			}
		}
		
		std::stringstream thread_id_as_string;
		thread_id_as_string << std::this_thread::get_id();
		return thread_id_as_string.str();
	}

	std::string log_thread_id_and_name()
	{
		static const size_t thread_id_width = 5;
		static const size_t thread_name_width = 16;
		
		// adding thread name
		std::string name;
		{
			std::shared_lock<std::shared_mutex> lock(thread_map_mutex);
			auto itr_thread_name = thread_map.find(std::this_thread::get_id());
			if (itr_thread_name != thread_map.end())
			{
				name = itr_thread_name->second;
			}
		}
		

		// truncating thread name to fit column in log
		if (name.size() < thread_name_width)
		{
			int padding_bytes = thread_name_width - name.size();
			name += std::string(padding_bytes, ' ');
		}
		else
		{
			name.resize(thread_name_width);
		}

		std::stringstream result;
		result << std::setw(thread_id_width) << std::this_thread::get_id() << " - " << std::setw(thread_name_width) << name;
		return result.str();
	}

	void logger::initialize_console_logger()
	{
		namespace lg = boost::log;
		namespace exp = lg::expressions; 

		// what does this do?
		lg::add_common_attributes();

		// what does this do?
		lg::register_simple_formatter_factory<lg::trivial::severity_level, char>("Severity");

		// format string tell boost log how each log entry shall be saved and presented to the user
		const std::string output_format = "[%TimeStamp%] [%Severity%]: %ThreadName% ::> [%LogLocation%] %Message%";

		// adding log to console
		auto console_log = lg::add_console_log(
			std::clog, 
			lg::keywords::format = output_format
		);

		console_log->set_filter(exp::attr<std::string>("FileFilter") == std::string());
	}

	void logger::initialize_file_logger(const boost::filesystem::path& log_directory, std::string log_file_name)
	{
		namespace lg = boost::log;
		namespace exp = lg::expressions; 

		whatlog::logger log("initialize_file_logger");
		
		boost::filesystem::path log_file_path = log_directory / log_file_name;
		std::string log_file_path_str = log_file_path.string();

		boost::shared_ptr<lg::core> core = lg::core::get();
		auto backend = boost::make_shared<lg::sinks::text_multifile_backend>();

		// setting up matching pattern to the different log files
		auto pattern = (exp::stream << log_file_path_str << exp::attr<std::string>("FileFilter") << ".log");
		backend->set_file_name_composer(lg::sinks::file::as_file_name_composer(pattern));

		typedef lg::sinks::synchronous_sink<lg::sinks::text_multifile_backend> mf_sink_t;
		boost::shared_ptr<mf_sink_t> sink(new mf_sink_t(backend));

		// log entry format
		auto format = (exp::stream << 
			exp::format_date_time< boost::posix_time::ptime >("TimeStamp", "[%Y-%m-%d %H:%M:%S.%f]") << " " <<
			std::setw(8) << lg::trivial::severity << " " << exp::attr<std::string>("ThreadName") << 
			" ::> [" <<  exp::attr<std::string>("LogLocation") << "] " << exp::smessage);

		sink->set_formatter(format);
		core->add_sink(sink);

		log.info("file logger initialized. saving log to directory: {}, with name: {}.", 
			log_directory.string(), log_file_name);
	}

	logger::logger(const std::string& location)
		: logger(location, std::string())
	{
		// nothing for now
	}

	logger::logger(const std::string& location, const std::string& fileFilter)
	{
		std::string log_file_ext;
		if (!fileFilter.empty())
		{
			log_file_ext = fileFilter;
			log_file_ext.insert(0, 1, '_');
		}

		m_logger.add_attribute("LogLocation", boost::log::attributes::constant<std::string>(location));
		m_logger.add_attribute("ThreadName", boost::log::attributes::constant<std::string>(log_thread_id_and_name()));
		m_logger.add_attribute("FileFilter", boost::log::attributes::constant<std::string>(log_file_ext));
	}

	void logger::info_internal(const std::string& message)
	{
		BOOST_LOG_SEV(m_logger, boost::log::trivial::info) << message;
	}

	void logger::warning_internal(const std::string& message)
	{
		BOOST_LOG_SEV(m_logger, boost::log::trivial::warning) << message;
	}

	void logger::error_internal(const std::string& message)
	{
		BOOST_LOG_SEV(m_logger, boost::log::trivial::error) << message;
	}
}
