#pragma once

#include <string>
#include <windows.h>

namespace FileDialogUtility
{
    /**
     * Opens a file dialog to select a file.
     *
     * @param filter The file filter (e.g., "OBJ Files\0*.obj\0All Files\0*.*\0").
     * @param defaultExtension The default file extension (e.g., "obj").
     * @return The selected file path as a string. Returns an empty string if canceled.
     */
    std::wstring OpenFileDialog(const char* filter, const char* defaultExtension);
}

