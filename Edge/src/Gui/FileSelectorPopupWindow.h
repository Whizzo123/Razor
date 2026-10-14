#pragma once
#include "PopupWindow.h"
#include "Razor.h"
#include <stack>

namespace EdgeEditor {
    using CallbackFunc = std::function<void(Razor::FilePath path)>;
    class FileSelectorPopupWindow : public PopupWindow {
    public:
        FileSelectorPopupWindow(CallbackFunc callback);
        bool Draw() override;
    private:
        void Open() override;
        void Close() override;

        void CallbackAndClose();

        std::vector<Razor::FilePath> GrabFiles(const Razor::FilePath& path);
        bool DrawFileGui(const Razor::FilePath& file);

        bool _mbIsOpen;
        std::stack<Razor::FilePath> _mSearchStack;
        std::string _mSelectedButtonLabel;
        Razor::FilePath _mSelectedFilePath;
        CallbackFunc _mCallback;
    };
}