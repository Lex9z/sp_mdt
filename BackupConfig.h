#ifndef BACKUP_CONFIG_H_
#define BACKUP_CONFIG_H_

#include <list>
#include <string>
#include <vector>

#include <map>
#pragma comment(lib,"C:\Program Files\Borland\CBuilder6\Lib\Psdk\shell32.lib")

#include "flashtool_handle.h"
#include "GCTrace.h"
using std::string;

class BackupItem{
public:
    BackupItem(){}
    explicit BackupItem(const std::string &partiName, const std::string &filePath, const std::string &chksumFile,
	                     U64 startAddr, unsigned int len)
     : partition_name_(partiName),
       file_path_(filePath),
       chksum_file_(chksumFile),
       start_addr_(startAddr),
       length_(len)
    {
        Dump();
    }

    ~BackupItem() {}
public:
    void set_partition_name(const std::string &partition_name)
    {
        partition_name_ = partition_name;
    }

    void set_file_path(const std::string &file_path)
    {
        file_path_ = file_path;
    }

    void set_chksum_file(const std::string &chksum_file)
    {
             chksum_file_ = chksum_file;
    }

    void set_start_addr(U64 start_addr)
    {
             start_addr_ = start_addr;
    }

    void set_length(unsigned int length)
    {
             length_ = length;
	}

    std::string partition_name() const
    {
		return partition_name_;
    }

	std::string file_path() const
	{
		return file_path_;
	}

	std::string chksum_file() const
	{
	     return chksum_file_;
	}

    U64 start_addr() const
    {
        return start_addr_;
    }

	unsigned int length() const
	{
        return length_;
	}
	void Dump();

private:
	std::string partition_name_;
	std::string file_path_;
	std::string chksum_file_;
	U64 start_addr_;
	unsigned int length_;
};

class BackupConfig{

public:
        BackupConfig(const std::string &backupFolder);
        ~BackupConfig();
        std::string Get_BackupINI_path(const std::string& scatter_file);
        std::list<std::string> backup_partitions();
        std::map<std::string, BackupItem> backup_map();

      bool QueryBackupItemByPartitionName(std::string partition_name, BackupItem *backup_item);

      int   InitBackupInfoTable();
	  void  InitVectorForFindPartName(string strName, vector<string> &vctDstTemp );
	  bool  FindPartName(string strSrc, vector<string> vctDstStr );

private:
	std::list<std::string> backup_partitions_;
	std::map<std::string, BackupItem> backup_map_;
	std::string backup_folder_;
};
#endif

