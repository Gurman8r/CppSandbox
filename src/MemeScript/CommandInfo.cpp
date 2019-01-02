#include <MemeScript/CommandInfo.h>
#include <iomanip>

namespace ml
	{
	const std::string CommandInfo::ArgNames[CommandInfo::MAX_ARG_TYPE] = {
		"Options",
		"Arguments",
		"Filename",
		"std::string",
		"Expr"
	};

	CommandInfo::CommandInfo()
		: name()
		, shortDesc()
		, longDesc()
		, exitStat()
		, options()
	{
	}
	
	CommandInfo::CommandInfo(const CommandInfo & copy)
		: name(copy.name)
		, shortDesc(copy.shortDesc)
		, longDesc(copy.longDesc)
		, exitStat(copy.exitStat)
		, options(copy.options)
	{
	}

	CommandInfo::CommandInfo(
		const std::string& name, 
		const std::string& shortDesc, 
		const std::string& longDesc, 
		const std::string& exitStat, 
		const std::vector<CmdOpt>& optVec)
		: name(name)
		, shortDesc(shortDesc)
		, longDesc(longDesc)
		, exitStat(exitStat)
		, options()
	{
		for (const CmdOpt& opt : optVec)
		{
			if (options[opt.type].empty())
			{
				options[opt.type].resize(0);
			}
			
			options[opt.type].push_back(opt);
		}
	}


	std::ostream& CommandInfo::formatName(std::ostream& out) const
	{
		out << name << " - " << shortDesc << "\n";
		return out;
	}
	
	std::ostream& CommandInfo::formatSynopsis(std::ostream& out) const
	{
		out << name << ": ";

		if (!options.empty())
		{
			for (auto pair : options)
			{
				switch (pair.first)
				{
				case Args_None:
					out << "[-";
					for (auto opt : pair.second)
					{
						out << opt.name;
					}
					out << "] ";
					break;

				case Args_List:
					for (auto opt : pair.second)
					{
						out << "[";
						out << opt.name;
						out << " ...] ";
					}
					break;
				}
			}
		}

		out << "\n";

		return out;
	}
	
	std::ostream& CommandInfo::formatDescription(std::ostream& out) const
	{
		if (!shortDesc.empty())
		{
			out << std::setw(shortDesc.size() + 4) << shortDesc << "\n"
				<< "\n";
		}

		if (!longDesc.empty())
		{
			out << std::setw(longDesc.size() + 4) << longDesc << "\n"
				<< "\n";
		}

		if (!options.empty())
		{
			for (auto pair : options)
			{
				const std::string& title = ArgNames[pair.first];
				
				out << std::setw(title.size() + 4) << title << ":\n";

				for (auto e : pair.second)
				{
					out << std::setw(8) 
						<< (pair.first == Args_None ? "-" : "")
						<< e.name << "\t" << e.desc << "\n";
				}
			}

			out << "\n";
		}

		if (!exitStat.empty())
		{
			out << std::setw(17) << "Exit Status:\n"
				<< std::setw(exitStat.size() + 4) << exitStat << "\n";
		}

		return out;
	}
	
	std::ostream& CommandInfo::formatManual(std::ostream& out) const
	{
		out << std::setfill(' ')
			<< std::setw(4) << "NAME\n"
			<< std::setw(8);
		formatName(out);
		out << "\n";

		auto foo = std::setw(10);

		out << std::setw(4) << "SYNOPSIS\n"
			<< std::setw(8);
		formatSynopsis(out);
		out << "\n";

		out << std::setw(4) << "DESCRIPTION\n"
			<< std::setw(8);
		formatDescription(out);

		return out;
	}
}