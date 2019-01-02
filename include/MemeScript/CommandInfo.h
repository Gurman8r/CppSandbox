#ifndef _COMMAND_INFO_H_
#define _COMMAND_INFO_H_

#include <MemeScript/Export.h>
#include <MemeCore/ITrackable.h>
#include <unordered_map>

namespace ml
{
	class ML_SCRIPT_API CommandInfo final
		: public ITrackable
	{
	public:
		enum ArgType
		{
			Args_None = 0,
			Args_List,
			Args_File,
			Args_Name,
			Args_Expr,
			MAX_ARG_TYPE
		};
		const static std::string ArgNames[MAX_ARG_TYPE];
		struct CmdOpt
		{
			std::string name;
			std::string desc;
			ArgType		type;

			CmdOpt()
				: name()
				, desc()
				, type()
			{
			};
			CmdOpt(const std::string& name, const std::string& desc)
				: name(name)
				, desc(desc)
				, type(CommandInfo::Args_None)
			{
			};
			CmdOpt(const std::string& name, ArgType args, const std::string& desc)
				: name(name)
				, desc(desc)
				, type(args)
			{
			};

			inline friend std::ostream& operator<<(std::ostream& out, const CmdOpt& rhs)
			{
				return out;
			};
		};

		using CmdOptMap = std::unordered_map<ArgType, std::vector<CmdOpt>>;

	public:
		CommandInfo();
		CommandInfo(const CommandInfo& copy);

		CommandInfo(
			const std::string& name,
			const std::string& shortDesc,
			const std::string& longDesc,
			const std::string& exitStat,
			const std::vector<CmdOpt>& options);

		~CommandInfo() {};

		std::string		name;
		std::string		shortDesc;
		std::string		longDesc;
		std::string		exitStat;
		CmdOptMap		options;

		std::ostream& formatName(std::ostream& out) const;
		std::ostream& formatSynopsis(std::ostream& out) const;
		std::ostream& formatDescription(std::ostream& out) const;
		std::ostream& formatManual(std::ostream& out) const;
	};
}

#endif // !_COMMAND_INFO_H_
