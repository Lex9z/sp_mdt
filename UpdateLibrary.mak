BROM_DLL_VERSION = "brom_v7.1504.00.00_full"

BROM_DLL_FOLDER_NAME = $(BROM_DLL_VERSION)
D_ROOT = .
Parent_ROOT = ..
BROM_DLL_SOURCE_NAME = $(Parent_ROOT)\$(BROM_DLL_FOLDER_NAME)
BROM_DLL_HEADER_PATH = $(BROM_DLL_SOURCE_NAME)
Update_Brom_Library_and_Header_File:  
  echo "Attr modify..."
  attrib -r .\Brom\brom.h /s
  attrib -r .\Brom\com_enum.h /s
  attrib -r .\Brom\DOWNLOAD.h /s
  attrib -r .\Brom\external_memory_config.h /s
  attrib -r .\Brom\flashtool.h /s
  attrib -r .\Brom\flashtool_api.h /s
  attrib -r .\Brom\flashtool_handle.h /s
  attrib -r .\Brom\mtk_mcu.h /s
  attrib -r .\Brom\mtk_resource.h /s
  attrib -r .\Brom\mtk_status.h /s
  attrib -r .\Brom\host.h /s
  
  echo "Copy DA..."
  copy /Y $(BROM_DLL_SOURCE_NAME)\bin\MTK_AllInOne_DA.bin .\output\MTK_AllInOne_DA.bin
  copy /Y $(BROM_DLL_SOURCE_NAME)\bin\DA_SWSEC.bin .\output\DA_SWSEC.bin
  
  echo "Copy Lib..."
  copy /Y $(BROM_DLL_HEADER_PATH)\bcb\FlashToolLib.lib .\lib\FlashToolLib.lib
  copy /Y $(BROM_DLL_HEADER_PATH)\bcb\FlashToolLib.dll .\output\FlashToolLib.dll
  
  echo "Copy header file..."
  copy /Y $(BROM_DLL_HEADER_PATH)\include\brom.h .\Brom\brom.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\com_enum.h .\Brom\com_enum.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\DOWNLOAD.h .\Brom\DOWNLOAD.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\external_memory_config.h .\Brom\external_memory_config.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\flashtool.h .\Brom\flashtool.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\flashtool_api.h .\Brom\flashtool_api.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\flashtool_handle.h .\Brom\flashtool_handle.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\mtk_mcu.h .\Brom\mtk_mcu.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\mtk_resource.h .\Brom\mtk_resource.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\mtk_status.h .\Brom\mtk_status.h
  copy /Y $(BROM_DLL_HEADER_PATH)\include\host.h .\Brom\host.h
  echo "End!"
#





















