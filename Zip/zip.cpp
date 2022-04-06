#include <iostream>
#include<string>
#include<cstring>
#include <fstream>
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <errno.h>   
#include <unistd.h>   
#include <fcntl.h>   
#include <sys/stat.h>  

#include <filesystem>
#include <chrono>

#define BUFSIZE  16 * 1024  


using namespace std;


static unsigned int crc_table[256];  
 
void init_crc_table(void)  
{  
    unsigned int c;  
    unsigned int i, j;  
 
    for (i = 0; i < 256; i++) {  
        c = (unsigned int)i;  
        for (j = 0; j < 8; j++) {  
            if (c & 1)  
                c = 0xedb88320 ^ (c >> 1);  
            else  
                c = c >> 1;  
        }  
        crc_table[i] = c;  
    }  
}  
 


bool write_normal_file(string filename){
    ofstream fout(filename);
    fout << "Hello world!";
    fout.close();
    return true;
}

bool write_bin_file(char* buffer, size_t length){
    ofstream ouF;
    ouF.open("bin");
    ouF.write(reinterpret_cast<const char*>(buffer), length);
    ouF.close();
    return true;
}
 
unsigned int crc32(unsigned int crc,unsigned char *buffer, unsigned int size)  
{  
    unsigned int i = crc ^ 0xffffffff;  
    while (size--)
    {
		i = (i >> 8) ^ crc_table[(i & 0xff) ^ *buffer++];
    }
    return i ^ 0xffffffff;
} 


int calc_img_crc(const char *in_file, unsigned int *img_crc)  
{  
    int fd;  
    int nread;  
    int ret;  
    unsigned char buf[BUFSIZE];
    unsigned int crc = 0;   
 
    fd = open(in_file, O_RDONLY);  
    if (fd < 0) {  
        printf("%d:open %s.\n", __LINE__, strerror(errno));  
        return -1;  
    }  
 
    while ((nread = read(fd, buf, BUFSIZE)) > 0) {  
        crc = crc32(crc, buf, nread);  
    }  
    *img_crc = crc;  
 
    close(fd);  
 
    if (nread < 0) {  
        printf("%d:read %s.\n", __LINE__, strerror(errno));  
        return -1;  
    }  
 
    return 0;  
}  

bool convert_file_to_zip(string read_filename, string write_filename){
    char* buffer = nullptr;
    size_t length = 0;

    int ret;  
    unsigned int file_crc = 0xffffffff;  
   
    init_crc_table();  
    ret = calc_img_crc(read_filename.data(), &file_crc);  
    if (ret < 0) 
	{  
        exit(1);  
    }  

  

    auto lstTime = filesystem::last_write_time(read_filename);
    //获取filetime和systemtime之间的时间差
    auto elapse = chrono::duration_cast<std::chrono::seconds>(filesystem::file_time_type::clock::now().time_since_epoch() - chrono::system_clock::now().time_since_epoch()).count();
    auto systemTime = chrono::duration_cast<chrono::seconds>(lstTime.time_since_epoch()).count() - elapse;
    
    tm *lsystemTime = localtime(&systemTime);
    

    fstream fin(read_filename);
    ofstream fout(write_filename);
    if(!fin.is_open()){
        cout << "Could not open the file " << read_filename << endl;
        cerr << "Error in line " << __LINE__ << ", in function \"" << __FUNCTION__ << "\" of file \"" << __FILE__ << "\"" <<endl;
        fin.clear();
        return false; 
    }else{
        
        fin.seekg(0, ios::end);
        length = fin.tellg();
        fin.seekg(0, ios::beg);
        buffer = new char[length + 1];
        memset(buffer, 0, sizeof(char) * (length + 1));
        fin.read(buffer, length);
        // cout << "Length = " << length << endl;
        // cout << "buffer = " << buffer << endl;
        fout.seekp(0, ios::beg);

        // 填充zip header 字段
        char header[4];
        header[0] = 'P';
        header[1] = 'K';
        header[2] = 3 + '\0';
        header[3] = 4 + '\0';

        fout.write(header, 4 * sizeof(char));
        
        // 填充 zip 版本字段
        char version[2];
        version[0] = 0x14 + '\0', version[1] = '\0';
        fout.write(version, 2 * sizeof(char));

        //General purpose bit flag
        char general[2];
        general[0] = '\0', general[1] = '\0';
        fout.write(general, 2 * sizeof(char));

        //Compression method
        char method[2];
        method[0] = '\0', method[1] = '\0';
        fout.write(method, 2 * sizeof(char));

        //File last modification time
        int s = lsystemTime->tm_sec / 2;
        int m = lsystemTime->tm_min;
        int h = lsystemTime->tm_hour;
        h <<= 6;
        h |= m;
        h <<= 5;
        h |= s;
        

        char *time = (char*)&h;
        
        fout.write(time, 2 * sizeof(char));

        //File last modification date
        int d = lsystemTime->tm_mday;
        int mon = lsystemTime->tm_mon + 1;
        int y = lsystemTime->tm_year - 80;
        // cout << "d = " << d << " mon = " << mon << " y = " << y << endl;
        y <<= 4;
        y |= mon;
        y <<= 5;
        y |= d;
        // cout << "Y = " << y << endl;
        
        char *date = (char*)&y;
        fout.write(date, 2 * sizeof(char));

        //CRC-32 of uncompressed data
        char *CRC = (char*)&file_crc;
        fout.write(CRC, 4 * sizeof(char));

        //Compressed size
        int buffer_len = strlen(buffer);
        // cout << buffer_len << endl;

        char *com_size;
        com_size = (char*)&buffer_len;
        fout.write(com_size, 4 * sizeof(char));
        //Uncompressed size
        
        char *uncom_size = nullptr;

        uncom_size = (char*)&buffer_len;
        // uncom_size[0] = 0x7 + '\0', uncom_size[1] = '\0', uncom_size[2] = '\0', uncom_size[3] = '\0';
        fout.write(uncom_size, 4 * sizeof(char));
        
        //File name length
        int read_filename_len = read_filename.size();
        // cout << "read_filename_len = " << read_filename_len << endl;
        char *file_name_len = (char*)&read_filename_len;
        fout.write(file_name_len, 2 * sizeof(char));

        //Extra field length
        char extra_len[2];
        extra_len[0] = '\0', extra_len[1] = '\0';
        fout.write(extra_len, 2 * sizeof(char));

        const char *file_name = read_filename.data();
        fout.write(file_name, read_filename.size() * sizeof(char));
    

        fout.write(buffer, length);
        // central file header signature
        char cen_file_header_sig[4];
        cen_file_header_sig[0] = 'P', cen_file_header_sig[1] = 'K', cen_file_header_sig[2] = 1 + '\0', cen_file_header_sig[3] = 2 + '\0';
        fout.write(cen_file_header_sig, 4 * sizeof(char));

        // version made by and version needed to extract
        fout.write(version, 2 * sizeof(char));
        fout.write(version, 2 * sizeof(char));

        // general purpose bit flag
        fout.write(general, 2 * sizeof(char));

        // compression method
        fout.write(method, 2 * sizeof(char));

        // last mod time and date
        fout.write(time, 2 * sizeof(char));
        fout.write(date, 2 * sizeof(char));

        // crc-32
        fout.write(CRC, 4 * sizeof(char));

        //compression and uncompressed size
        fout.write(com_size, 4 * sizeof(char));
        fout.write(uncom_size, 4 * sizeof(char));

        // file name length
        fout.write(file_name_len, 2 * sizeof(char));

        // extra field length
        fout.write(extra_len, 2 * sizeof(char));

        // file comment length
        char comment_len[2];
        comment_len[0] = '\0', comment_len[1] = '\0';
        fout.write(comment_len, 2 * sizeof(char));

        // disk number start
        char disk_start[2];
        disk_start[0] = '\0', disk_start[1] = '\0';
        fout.write(disk_start, 2 * sizeof(char));

        // internal file attributes
        char in_file_atr[2];
        in_file_atr[0] = 1 + '\0', in_file_atr[1] = '\0';
        fout.write(in_file_atr, 2 * sizeof(char));

        
        //external file attributes
        char ex_file_atr[4];
        ex_file_atr[0] = 0x20 + '\0', ex_file_atr[1] = '\0', ex_file_atr[2] = '\0', ex_file_atr[3] = '\0';
        fout.write(ex_file_atr, 4 * sizeof(char));

        // relative offset of local header
        char rel_off[] = "\0\0\0\0";
        fout.write(rel_off, 4 * sizeof(char));

        // filename
        fout.write(file_name, read_filename.size() * sizeof(char));


        // end of central dir signature
        char sig[4];
        sig[0] = 'P', sig[1] = 'K', sig[2] = 0x5 + '\0', sig[3] = 0x6 + '\0';
        fout.write(sig, sizeof(char) * 4);
        
        // number of this disk
        char num_disk[2];
        num_disk[0] = '\0', num_disk[1] = '\0';
        fout.write(num_disk, 2 * sizeof(char));

        // number of the disk with the start of the central directory
        char num_disk_central[2];
        num_disk_central[0] = '\0', num_disk_central[1] = '\0';
        fout.write(num_disk_central, 2 * sizeof(char));

        // total number of entries in the central directory on this disk
        char total_entries[2];
        total_entries[0] = 1 + '\0', total_entries[1] = '\0';
        fout.write(total_entries, 2 * sizeof(char));

        //total number of entries in the central directory   
        char tot_ent_dir[2];
        tot_ent_dir[0] = 1 + '\0', tot_ent_dir[1] = '\0';
        fout.write(tot_ent_dir, 2 * sizeof(char));

        // size of the central directory
        char size_cen_dir[4];
        size_cen_dir[0] = 0x36 + '\0', size_cen_dir[1] = '\0', size_cen_dir[2] = '\0', size_cen_dir[3] = '\0';
        fout.write(size_cen_dir, 4 * sizeof(char));

        //offset of start of central directory with respect to the starting disk number
        int start_offset = 30 + read_filename_len + buffer_len;
        char *offset = (char*)&start_offset;
        fout.write(offset, 4 * sizeof(char));

        //zip file comment length
        char com_len[2];
        com_len[0] = '\0', com_len[1] = '\0';
        fout.write(com_len, 2 * sizeof(char));

        length = fin.tellg();
        
        
        // cout << "length = " << length  << " bytes" << endl;
        // cout << buffer << endl;
        
    }
    
    fin.close();
    fout.close();
    // write_bin_file(buffer, length);
    delete(buffer);
    return true;
    
}



int main(){

    string filename = "text.txt";
    // write_normal_file(filename);
    convert_file_to_zip(filename, "output.zip");
}
