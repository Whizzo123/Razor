#include "ProjectExplorer.h"
#include <filesystem>

#ifdef RZ_PLATFORM_WINDOWS
#include <direct.h>
#define getcwd _getcwd
#elif defined(RZ_PLATFORM_LINUX)
#include <unistd.h>
#endif
#ifdef RZ_PLATFORM_WINDOWS
#include "Utils/Windows/CDialogEventHandler.h"
HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void** ppv)
{
	*ppv = NULL;
	CDialogEventHandler* pDialogEventHandler = new (std::nothrow) CDialogEventHandler();
	HRESULT hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;
	if (SUCCEEDED(hr))
	{
		hr = pDialogEventHandler->QueryInterface(riid, ppv);
		pDialogEventHandler->Release();
	}
	return hr;
}
#endif

namespace EdgeEditor
{
	/*
	* 
	*/
	void ProjectExplorer::Render()
	{
		bool bIsOpen;
		Razor::RazorImGui::Begin("Project Explorer", &bIsOpen, Razor::RazorGuiWindowFlags_MenuBar);
		Razor::RazorImGui::SetWindowSize(Razor::Vector2(200.0f, 200.0f));
		if (Razor::RazorImGui::BeginTable("FileTable", 4))
		{
			if (_mSearchStack.empty())
			{
				Razor::RazorImGui::EndTable();
				Razor::RazorImGui::End();
				return;
			}
			std::vector<Razor::FilePath> fileNames = GrabFiles(_mSearchStack.top());
			for (const Razor::FilePath& entry : fileNames)
			{
				Razor::RazorImGui::TableNextColumn();
				DrawFileGui(entry);
			}
			Razor::RazorImGui::EndTable();
		}
		if (Razor::RazorImGui::Button("Import", Razor::Vector2(100.0f, 50.0f)))
		{
			OpenFile();
		}
		if (Razor::RazorImGui::Button("Back", Razor::Vector2(100.0f, 50.0f)))
		{
			if (_mSearchStack.size() > 1)
			{
				_mSearchStack.pop();
			}
		}
		Razor::RazorImGui::End();
	}

	std::vector<Razor::FilePath> ProjectExplorer::GrabFiles(const std::string & Path)
	{
		std::vector<Razor::FilePath> fileNames;

		struct stat sb;
		// TODO verify that Path is actually a path to a file otherwise we will throw here 
		try 
		{
		for (const std::filesystem::directory_entry& DirectoryEntry : std::filesystem::directory_iterator(Path))
		{
			std::filesystem::path filePath = DirectoryEntry.path();
			std::string fileName = filePath.string();
			Razor::FilePath path(fileName);
			bool bIsDirectory = false;
			if (stat(static_cast<std::string>(path).c_str(), &sb) == 0 && (sb.st_mode & S_IFDIR))
			{
				bIsDirectory = true;
			}
			fileNames.push_back({ path, bIsDirectory });
		}
		} catch (std::filesystem::filesystem_error err) {
			char buffer[512];
			buffer[0] = '\0';
			getcwd(&buffer[0], 512);
			RZ_ERROR("ProjectExplorer::GrabFiles -> Threw file system error path was {0}, current working directory is {1}", Path, buffer);
		}

		return fileNames;
	}

	void ProjectExplorer::DrawFileGui(const Razor::FilePath& entry)
	{
		void* ButtonImage = 0;
		if (Razor::RazorImGui::ImageButton(ButtonImage, Razor::Vector2(100.0f, 100.0f)))
		{
			if (!entry.IsDir())
			{
				//_mSelectedAssetPath = entry.mName;
			}
			else
			{
				_mSearchStack.push(entry);
			}
		}
		Razor::FilePath path = entry.RemoveFromPath(Razor::FilePath(_mSearchStack.top()));
		Razor::RazorImGui::Text(static_cast<std::string>(path).c_str());
	}

	// TODO this is windows only will want a linux version too should probably be hidden behind a platform generic interface
	void ProjectExplorer::OpenFile()
	{
	
		#ifdef RZ_PLATFORM_WINDOWS
		IFileDialog* FileDialogPtr = nullptr;
		HRESULT Result = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&FileDialogPtr));

		if (SUCCEEDED(Result))
		{
			IFileDialogEvents* EventsPtr = nullptr;
			Result = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&EventsPtr));
			if (SUCCEEDED(Result))
			{
				DWORD DWCookie;
				Result = FileDialogPtr->Advise(EventsPtr, &DWCookie);
				if (SUCCEEDED(Result))
				{
					DWORD DWFlags;
					Result = FileDialogPtr->GetOptions(&DWFlags);
					if (SUCCEEDED(Result))
					{
						Result = FileDialogPtr->SetOptions(DWFlags | FOS_FORCEFILESYSTEM);
						if (SUCCEEDED(Result))
						{
							// Set the file types to display only. 
							// Notice that this is a 1-based array.
							Result = FileDialogPtr->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
							if (SUCCEEDED(Result))
							{
								// Set the selected file type index to Word Docs for this example.
								//if we ever want this functionality hr = pfd->SetFileTypeIndex(INDEX_WORDDOC);
								if (SUCCEEDED(Result))
								{
									// Set the default extension to be ".doc" file.
									//if we ever want this functionality Result = FileDialogPtr->SetDefaultExtension(L"doc;docx");
									if (SUCCEEDED(Result))
									{
										// Show the dialog
										Result = FileDialogPtr->Show(NULL);
										if (SUCCEEDED(Result))
										{
											// Obtain the result once the user clicks 
											// the 'Open' button.
											// The result is an IShellItem object.
											IShellItem* psiResult;
											Result = FileDialogPtr->GetResult(&psiResult);
											if (SUCCEEDED(Result))
											{
												// We are just going to print out the 
												// name of the file for sample sake.
												PWSTR pszFilePath = NULL;
												Result = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
													&pszFilePath);
												if (SUCCEEDED(Result))
												{
													std::wstring ws = std::wstring(pszFilePath);
													SaveModelToProject(std::string(ws.begin(), ws.end()));
													CoTaskMemFree(pszFilePath);
												}
												psiResult->Release();
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		#endif
		#ifdef RZ_PLATFORM_LINUX

		#endif
	}

	bool ProjectExplorer::SaveModelToProject(const std::string& Name)
	{
		Razor::Model Model = Razor::Engine::Get().ProcessModel(Name.c_str());
		int LastIndexOf = Name.find_last_of('\\');
		std::string FileName = Name.substr(LastIndexOf + 1);
		int FirstIndexOf = FileName.find_first_of('.');
		FileName = FileName.substr(0, FirstIndexOf);
		Razor::ModelSerializer::Serialize(_mRootPath + "/" + FileName, Razor::CreateRef<Razor::Model>(Model));
		return true;
	}

	void ProjectExplorer::Refresh(const std::string& path)
	{
		_mRootPath = path;
		while (_mSearchStack.size() > 0)
		{
			_mSearchStack.pop();
		}
		_mSearchStack.push(_mRootPath);
	}
}

