#ifndef _ML_CORE_EVENTS_HPP_
#define _ML_CORE_EVENTS_HPP_

#include <MemeCore/IEvent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API CoreEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_CORE_EVENT = IEvent::EV_CORE + 1,
			
			EV_FS_ChangeDir,
			EV_OS_Execute,
			
			MAX_CORE_EVENT
		};

		CoreEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FS_ChangDirEvent : public CoreEvent
	{
		const String dir;

		FS_ChangDirEvent(const String & dir)
			: CoreEvent(EV_FS_ChangeDir)
			, dir(dir)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeInfo().name() << " " << dir;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API OS_ExecuteEvent : public CoreEvent
	{
		const String	cmd;
		const String	file;
		const String	args;
		const String	path;

		OS_ExecuteEvent(
			const String & cmd,
			const String & file = String(),
			const String & args = String(),
			const String & path = String())
			: CoreEvent(EV_OS_Execute)
			, cmd	(cmd)
			, file	(file)
			, args	(args)
			, path	(path)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeInfo().name() << " "
				<< cmd << " "
				<< file << " "
				<< args << " "
				<< path << " ";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API OS_OpenEvent : public OS_ExecuteEvent
	{
		OS_OpenEvent(
			const String & file,
			const String & args = String(),
			const String & path = String())
			: OS_ExecuteEvent("open", file, args, path)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CORE_EVENTS_HPP_