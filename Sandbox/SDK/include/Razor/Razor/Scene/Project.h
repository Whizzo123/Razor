#pragma once

#include <string>

namespace Razor
{
	/**
	* Struct to hold data about this game project
	*/
	struct Project
	{
		std::string m_ProjectName; /** Name of the project */
		std::string m_AssetDirectory; /** Directory containing all the asset files */
		std::string m_DllDirectory; /** Directory containing the scripting DLL */
		std::string m_MainScenePath; /** Path to the main scene (we will open this on load up)*/
		std::string m_ProjectPath; /** Path to the project dir itself */

		/**
		* Default constructor for Project class
		*/
		Project() : m_ProjectName(""), m_AssetDirectory(""), m_DllDirectory(""), m_MainScenePath(""), m_ProjectPath("")
		{

		}

		/**
		* Constructor for Project class
		* 
		* @param ProjectName - name for the project
		* @param AssetDirectory - directory containing all the asset files
		* @param DllDirectory - directory containing the scripting DLL
		*/
		Project(const std::string& ProjectName, const std::string& AssetDirectory, const std::string& DllDirectory, const std::string& ProjectPath)
			: m_ProjectName(ProjectName), m_AssetDirectory(AssetDirectory), m_DllDirectory(DllDirectory), m_ProjectPath(ProjectPath)
		{

		}
	};
}