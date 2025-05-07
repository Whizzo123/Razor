#include "ProjectExplorer.h"
#include <filesystem>
#include "Utils/Windows/CDialogEventHandler.h"
#include "FileIO/ModelSerializer.h"

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

namespace EdgeEditor
{
	/*
	* 
	*/
	void ProjectExplorer::Render()
	{
		bool bIsOpen;
		ImGui::Begin("Project Explorer", &bIsOpen, ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize(ImVec2(200.0f, 200.0f));
		ImGui::BeginTable("FileTable", 4);
		for (const std::string& Name : FileNames)
		{
			ImGui::TableNextColumn();
			DrawFileGui(Name);
		}
		ImGui::EndTable();
		if (ImGui::Button("Import", ImVec2(100.0f, 50.0f)))
		{
			OpenFile();
		}
		ImGui::End();
	}

	std::vector<std::string> ProjectExplorer::GrabFiles(const std::string& Path)
	{
		std::vector<std::string> FileNames;
		
		struct stat sb;

		for (const std::filesystem::directory_entry& DirectoryEntry : std::filesystem::directory_iterator(Path))
		{
			std::filesystem::path FilePath = DirectoryEntry.path();
			std::string FileName = FilePath.string();

			if (stat(FileName.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR))
			{
				// TODO deal with directory
			}
			FileNames.push_back(FileName);
		}

		return FileNames;
	}

	void ProjectExplorer::DrawFileGui(const std::string& FileName)
	{
		const ImTextureID ButtonImage = 0;
		ImGui::ImageButton(ButtonImage, ImVec2(100.0f, 100.0f));
		ImGui::Text(FileName.c_str());
	}

	// TODO this is windows only will want a linux version too should probably be hidden behind a platform generic interface
	void ProjectExplorer::OpenFile()
	{
	

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
	}

	bool ProjectExplorer::SaveModelToProject(const std::string& Name)
	{
		Razor::Model Model = Razor::Engine::Get().ProcessModel(Name.c_str());
		int LastIndexOf = Name.find_last_of('\\');
		std::string FileName = Name.substr(LastIndexOf + 1);
		int FirstIndexOf = FileName.find_first_of('.');
		FileName = FileName.substr(0, FirstIndexOf);
		ModelSerializer::Serialize("project/" + FileName, Razor::CreateRef<Razor::Model>(Model));
		FileNames = GrabFiles(ProjectDir);
		return true;
	}
}

