#pragma once


namespace vml
{
	namespace strings
	{

		class Print
		{
			private:

				template <typename T>
				static void wprintargument(std::wstringstream& ss, T t)
				{
					ss << t << " ";
				}

				template <typename T>
				static void printargument(std::stringstream& ss, T t)
				{
					ss << t << " ";
				}

			public:

				template <typename... t>
				static std::wstring WArgs(t&&... args)
				{
					std::wstringstream ss;
					int dummy[] = { 0, ((void)wprintargument(ss,std::forward<t>(args)),0)... };
					return ss.str();
				}

				template <typename... t>
				static std::string Args(t&&... args)
				{
					std::stringstream ss;
					int dummy[] = { 0, ((void)printargument(ss,std::forward<t>(args)),0)... };
					return ss.str();
				}

		};

	}
}
