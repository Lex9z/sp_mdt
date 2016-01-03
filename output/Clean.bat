@del /F /Q *.obj
@del /F /Q *.lib
@del /F /Q *.bak2
@del /F /Q *.bak
@del /F /Q *.cgl
@del /F /Q *.drc
@del /F /Q *.map
@del /F /Q *.tds


@rd /S /Q .svn
@rd /S /Q backup_and_restore_data
@md backup_and_restore_data

@del /F /Q SPMultiPortFlashDownloadProject.ini
@del /F /Q Clean.bat