#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

////////////////////////////////////////////////////////////////
// class for path splitting

namespace vml
{
	namespace strings
	{

		class SplitPath
		{

				// -------------------------------------------------------
				// this character is the separator it MUST be '\\'

				static const char _SEPARATOR_ = '\\';

			public:
				
				// -------------------------------------------------------
				// Returns the drive's letter from the passed path.

				static std::string GetDrive(const std::string& path)
				{
					if (path.size() == 0)
						throw std::runtime_error("no valid path given");
					std::string m_buffer;
					size_t colon = (size_t)path.find(':');
					if (colon != std::string::npos)
						m_buffer = path[colon - 1];
					return m_buffer;
				}
				
				// -------------------------------------------------------
				// Returns the directory from the passed path.

				static std::string GetDirectory(const std::string &path)
				{
					if (path.size() == 0)
						throw std::runtime_error("no valid path given");
					std::string m_buffer;
					size_t last = (size_t)path.size() - 1;
					if (last == std::string::npos) return "";
					size_t sep = (size_t)path.rfind(_SEPARATOR_);
					if (sep == last)
						m_buffer = path;
					else if (sep != std::string::npos)
						m_buffer = path.substr(0, size_t(sep)+1);
					return m_buffer;
				}

				// -------------------------------------------------------
				// Returns the file's folder from the passed path.

				static std::string GetFolder(const std::string &path)
				{
					if (path.size() == 0)
						throw std::runtime_error("no valid path given");
					std::string m_buffer;
					std::string dir = GetDirectory(path);
					size_t sep;
					sep = dir.rfind(_SEPARATOR_);
					sep = dir.rfind(_SEPARATOR_, sep - 1);
					if (sep == std::string::npos) return "";
					m_buffer = dir.substr( sep+1, dir.size() - sep - 2);
					return m_buffer;
				}

				// -------------------------------------------------------
				// Returns the file's title from the passed path.
				// title is filename without extension

				static std::string GetTitle(const std::string& path)
				{
					std::string m_buffer;
					std::string name = GetName(path);
					if (name.empty())
						name = path;
					int dot = (int)name.rfind(".");
					if (dot == std::string::npos)
						m_buffer = name;
					else
						m_buffer = name.substr(0, dot);
					return m_buffer;
				}
				
				// -------------------------------------------------------
				// gets file extension 

				static std::string GetExtension(const std::string& path)
				{
					if (path.size() == 0)
						throw std::runtime_error("no valid path given");
					// Make sure we only look in the filename, and not the path 
					std::string name = vml::strings::SplitPath::GetName(path);
					size_t pos = name.rfind('.');
					if (pos != std::string::npos)
						return name.substr(pos + 1);
					return "";
				}

				// -------------------------------------------------------
				// Returns file name

				static std::string GetName(const std::string& path)
				{
					if (path.size() == 0)
						throw std::runtime_error("no valid path given");
					size_t pos = path.rfind(_SEPARATOR_);
					if (pos != std::string::npos)
						return path.substr(pos + 1);
					return "";
				}

				// -------------------------------------------------------
				// Return a path object without the extension 

				static std::string RemoveExtensionFromPath(const std::string& path)
				{
					if (path.size() == 0)
						throw std::runtime_error("no valid path given");
					size_t sep_pos = path.rfind(_SEPARATOR_);
					size_t dot_pos = path.rfind('.');
					if (dot_pos == std::string::npos)
						return "";
					if (sep_pos == std::string::npos || sep_pos < dot_pos)
						return std::string(path.substr(0, dot_pos));
					return "";
				}
					
				// -------------------------------------------------------
				// splits a given path into a vector of folders ,last element is file

				static std::vector<std::string> DirSplit(const std::string& s)
				{
					std::vector<std::string> directories;
					if (s.size() == 0)
						return directories;
					size_t position = 0, currentPosition = 0;
					while (currentPosition != -1)
					{
						currentPosition = s.find_first_of(_SEPARATOR_, position);
						directories.emplace_back(s.substr(position, currentPosition - position));
						position = currentPosition + 1;
					}
					return directories;
				}

				// -------------------------------------------------------
				// Sanitize a string

				static std::string SanitizeFilePath(const std::string& path)
				{
					std::string sanitizedPath = path;

					#ifdef _WIN32

						// Windows-specific sanitation Replace forward slashes with backslashes

						std::string sep(1, _SEPARATOR_);

						size_t start_pos = 0;
						while ((start_pos = sanitizedPath.find("/", start_pos)) != std::string::npos)
						{
							sanitizedPath.replace(start_pos, 1, std::string(1, _SEPARATOR_));
							start_pos += 1;
						}

						// Remove redundant backslashes

						sanitizedPath.erase(std::unique(sanitizedPath.begin(), sanitizedPath.end(), [](char a, char b) {
							return a == _SEPARATOR_ && b == _SEPARATOR_;
							}), sanitizedPath.end());
				
					#else

						// Unix/Linux-specific sanitation
						// Replace backslashes with forward slashes

						size_t start_pos = 0;
						while ((start_pos = sanitizedPath.find(std::string(1, _SEPARATOR_), start_pos)) != std::string::npos)
						{
							sanitizedPath.replace(start_pos, 1, "/");
							start_pos += 1;
						}

						// Remove redundant forward slashes

						sanitizedPath.erase(std::unique(sanitizedPath.begin(), sanitizedPath.end(), [](char a, char b) {
							return a == '/' && b == '/';
							}), sanitizedPath.end());
					
					#endif

					// Additional sanitation: remove invalid characters (example: just removing '*' and '?')

					sanitizedPath.erase(std::remove_if(sanitizedPath.begin(), sanitizedPath.end(), [](char c) {
						return c == '*' || c == '?' || c == '!';
						}), sanitizedPath.end());

					// Handle trailing slash (optional: depending on your use case)

					if (sanitizedPath.length() > 1 && (sanitizedPath.back() == _SEPARATOR_ || sanitizedPath.back() == '/')) {
						sanitizedPath.pop_back();
					}

					return sanitizedPath;
				}

				// -------------------------------------------------------
				// ctor / dtor

				SplitPath()
				{}
				~SplitPath()
				{}

			};

	} // end of namespace strings

} // end of namespace vml