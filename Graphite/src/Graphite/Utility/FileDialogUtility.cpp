#include "gfpch.h"

#include <commdlg.h>
#include "FileDialogUtility.h"
#include <iostream>

namespace FileDialogUtility {
    std::wstring OpenFileDialog(const char* filter, const char* defaultExtension) {
        wchar_t filename[MAX_PATH] = L""; // Buffer to store the selected file path
        wchar_t wideFilter[256] = L"";    // Buffer for the wide-character filter
        wchar_t wideDefaultExt[16] = L""; // Buffer for the wide-character default extension

        // Convert filter to wide string
        size_t filterLength = strlen(filter) + 1; // Include null terminator
        mbstowcs(wideFilter, filter, filterLength);

        // Convert default extension to wide string
        if (defaultExtension != nullptr) {
            size_t extLength = strlen(defaultExtension) + 1;
            mbstowcs(wideDefaultExt, defaultExtension, extLength);
        }

        OPENFILENAME ofn;                // Structure to configure the file dialog
        ZeroMemory(&ofn, sizeof(ofn));   // Initialize the structure to zero
        ofn.lStructSize = sizeof(ofn);   // Size of the structure
        ofn.hwndOwner = NULL;            // Handle to the owner window (NULL for none)
        ofn.lpstrFile = filename;        // Pointer to the buffer that receives the file path
        ofn.nMaxFile = MAX_PATH;         // Size of the file path buffer
        ofn.lpstrFilter = wideFilter;    // Filter options
        ofn.lpstrDefExt = (defaultExtension != nullptr) ? wideDefaultExt : NULL;
        ofn.nFilterIndex = 1;            // Default filter index
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Flags for dialog behavior

        // Display the Open dialog box
        if (GetOpenFileName(&ofn)) {
            return std::wstring(filename); // Return the selected file path
        }
        else {
            // Handle dialog cancellation or error
            DWORD error = CommDlgExtendedError();
            if (error != 0) {
                std::cerr << "Error: " << error << std::endl;
            }
            return L""; // Return an empty string if no file was selected
        }
    }
}

