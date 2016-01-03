# ---------------------------------------------------------------------------
!if !$d(BCB)
BCB = $(MAKEDIR)\..
!endif

# ---------------------------------------------------------------------------
# IDE SECTION
# ---------------------------------------------------------------------------
# The following section of the project makefile is managed by the BCB IDE.
# It is recommended to use the IDE to change any of the values in this
# section.
# ---------------------------------------------------------------------------

VERSION = BCB.06.00
# ---------------------------------------------------------------------------
PROJECT = output\SPMultiPortFlashDownloadProject.exe
OBJFILES = output\MultiPortFlashDownloadProject.obj \
    output\MultiPortFlashDownloadTool.obj output\MultiPort_Utils.obj \
    output\INIFileHandle.obj output\FlashDownLoad.obj output\time_utils.obj \
    output\form_About.obj output\file_utils.obj output\form_MultiLoad.obj \
    output\ThreadManager.obj output\GCTrace.obj output\SetBeginComPort.obj \
    output\ToolSettings.obj output\NvramBackupMap.obj \
    output\FirmwareUpgrade.obj output\TScanPort.obj output\COMPortUtils.obj \
    output\CustomerSetup.obj output\PathSettings.obj \
    output\ImageDetailInfo.obj output\NewAddedImages.obj \
    output\NewIniHandle.obj output\ImagesFormat.obj output\com_util.obj \
    output\EfuseWriter_Console.obj output\EfuseSettings.obj \
    output\formChecksumProgress.obj output\PCMemoryChecksumProgress.obj
RESFILES = MultiPortFlashDownloadProject.res
MAINSOURCE = MultiPortFlashDownloadProject.cpp
RESDEPEN = $(RESFILES) MultiPortFlashDownloadTool.dfm Form\form_About.dfm \
    Form\form_MultiLoad.dfm Form\SetBeginComPort.dfm Form\ToolSettings.dfm \
    Form\formChecksumProgress.dfm
LIBFILES = output\SLA_Challenge.lib output\Eboot.lib output\FlashToolLib.lib \
    output\SPMETA_DLL.lib
IDLFILES = 
IDLGENFILES = 
LIBRARIES = vcldb.lib vclx.lib bcbsmp.lib adortl.lib vcl.lib rtl.lib dbrtl.lib
PACKAGES = vcl.bpi rtl.bpi vclx.bpi bcbsmp.bpi dbrtl.bpi adortl.bpi vcldb.bpi \
    qrpt.bpi bdertl.bpi dsnap.bpi cds.bpi bdecds.bpi teeui.bpi teedb.bpi \
    tee.bpi vcldbx.bpi teeqr.bpi ibxpress.bpi visualclx.bpi visualdbclx.bpi \
    vclie.bpi xmlrtl.bpi inet.bpi inetdbbde.bpi inetdbxpress.bpi nmfast.bpi \
    bcbie.bpi soaprtl.bpi dclocx.bpi dbexpress.bpi dbxcds.bpi indy.bpi \
    bcb2kaxserver.bpi MMTools_c6.bpi dclusr.bpi
SPARELIBS = dbrtl.lib rtl.lib vcl.lib adortl.lib bcbsmp.lib vclx.lib vcldb.lib
DEFFILE = 
OTHERFILES = 
# ---------------------------------------------------------------------------
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
USERDEFINES = _DEBUG
SYSDEFINES = NO_STRICT;_NO_VCL
INCLUDEPATH = Efuse;Brom;Eboot;FirmwareUpgrade;Form;Common;Headers;INIFileHandle;"C:\Program Files\Borland\CBuilder6\Projects";$(BCB)\include;$(BCB)\include\vcl
LIBPATH = Efuse;FirmwareUpgrade;"Comm\USB COM";"META App";GPS;Comm\PCMCIA;FailReason;Form;Common;INIFileHandle;"C:\Program Files\Borland\CBuilder6\Projects";"C:\Multiport download source\Multiport_DL source v2.4.1004_VCL";$(BCB)\Projects\Lib;freqz;firls;D:\Meta\Andy\Mfiles;$(BCB)\lib\obj;$(BCB)\lib
WARNINGS= -w-par
PATHCPP = .;INIFileHandle;INIFileHandle;Common;Form;Common;Form;Form;Form;FirmwareUpgrade;FirmwareUpgrade;INIFileHandle;Efuse;Efuse;Efuse;Form
PATHASM = .;
PATHPAS = .;
PATHRC = .;
PATHOBJ = .;$(LIBPATH)
# ---------------------------------------------------------------------------
CFLAG1 = -Od -H=$(BCB)\lib\vcl60.csm -Vx -Ve -RT- -X- -r- -a8 -b -k -y -v -vi- -tWD \
    -tWM- -c
IDLCFLAGS = 
PFLAGS = -N2output -N0output -$Y+ -$W -$O- -$A8 -v -JPHNE -M
RFLAGS = 
AFLAGS = /mx /w2 /zi
LFLAGS = -loutput -Ioutput -D"" -aa -Tpe -GD -s -Gn -Gi -M -v
# ---------------------------------------------------------------------------
ALLOBJ = c0w32.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cp32mt.lib
# ---------------------------------------------------------------------------
!ifdef IDEOPTIONS

[Version Info]
IncludeVerInfo=0
AutoIncBuild=0
MajorVer=1
MinorVer=0
Release=0
Build=0
Debug=0
PreRelease=0
Special=0
Private=0
DLL=0

[Version Info Keys]
CompanyName=
FileDescription=
FileVersion=1.0.0.0
InternalName=
LegalCopyright=
LegalTrademarks=
OriginalFilename=
ProductName=
ProductVersion=1.0.0.0
Comments=

[Debugging]
DebugSourceDirs=$(BCB)\source\vcl

!endif





# ---------------------------------------------------------------------------
# MAKE SECTION
# ---------------------------------------------------------------------------
# This section of the project file is not used by the BCB IDE.  It is for
# the benefit of building from the command-line using the MAKE utility.
# ---------------------------------------------------------------------------

.autodepend
# ---------------------------------------------------------------------------
!if "$(USERDEFINES)" != ""
AUSERDEFINES = -d$(USERDEFINES:;= -d)
!else
AUSERDEFINES =
!endif

!if !$d(BCC32)
BCC32 = bcc32
!endif

!if !$d(CPP32)
CPP32 = cpp32
!endif

!if !$d(DCC32)
DCC32 = dcc32
!endif

!if !$d(TASM32)
TASM32 = tasm32
!endif

!if !$d(LINKER)
LINKER = ilink32
!endif

!if !$d(BRCC32)
BRCC32 = brcc32
!endif


# ---------------------------------------------------------------------------
!if $d(PATHCPP)
.PATH.CPP = $(PATHCPP)
.PATH.C   = $(PATHCPP)
!endif

!if $d(PATHPAS)
.PATH.PAS = $(PATHPAS)
!endif

!if $d(PATHASM)
.PATH.ASM = $(PATHASM)
!endif

!if $d(PATHRC)
.PATH.RC  = $(PATHRC)
!endif

!if $d(PATHOBJ)
.PATH.OBJ  = $(PATHOBJ)
!endif
# ---------------------------------------------------------------------------
$(PROJECT): $(OTHERFILES) $(IDLGENFILES) $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) -L$(LIBPATH) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES)
!
# ---------------------------------------------------------------------------
.pas.hpp:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.pas.obj:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.cpp.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.cpp.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.asm.obj:
    $(BCB)\BIN\$(TASM32) $(AFLAGS) -i$(INCLUDEPATH:;= -i) $(AUSERDEFINES) -d$(SYSDEFINES:;= -d) $<, $@

.rc.res:
    $(BCB)\BIN\$(BRCC32) $(RFLAGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -fo$@ $<



# ---------------------------------------------------------------------------




