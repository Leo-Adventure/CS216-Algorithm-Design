#include "zip.hpp"
#include "deflate.hpp"

extern size_t total_bit;
// double total_io_time2 = 0;

// 与 crc 32 计算相关的代码参考了 https://blog.csdn.net/gongmin856
///article/details/77101397?ops_request_misc=%257B%2522request%255Fid%2522%253A%252
//2164994317116782089370410%2522%252C%2522scm%2522%253A%25
//2220140713.130102334.pc%255Fall.%2522%257D&req
//uest_id=164994317116782089370410&biz_i
//d=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~r
//ank_v31_ecpm-2-77101397.142^v8^pc_search_result_c
//ache,157^v4^control&utm_term=crc32%E7%AE%97%E6%B3%95c%E8%AF%AD%E8%A8%80&spm=1018.2226.3001.4187
static unsigned int crc32( const unsigned char *buf, unsigned int size)
{
     unsigned int i, crc;
     crc = 0xFFFFFFFF;
 
 
     for (i = 0; i < size; i++)
      crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
 
     return crc^0xFFFFFFFF;
}



bool convert_file_to_zip(string read_filename, string write_filename){
    char* buffer = nullptr;
    unsigned int length = 0;

    int ret;  
    
    auto lstTime = filesystem::last_write_time(read_filename);
    //获取filetime和systemtime之间的时间差
    auto elapse = chrono::duration_cast<std::chrono::seconds>(filesystem::file_time_type::clock::now().time_since_epoch() - chrono::system_clock::now().time_since_epoch()).count();
    auto systemTime = chrono::duration_cast<chrono::seconds>(lstTime.time_since_epoch()).count() - elapse;
    
    tm *whole_time = localtime(&systemTime);
    
    // timeval io_time1, io_time2, io_time3, io_time4;
    // gettimeofday(&io_time1, NULL);

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
        cout << "length of file = " << length << endl;
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
        method[0] = 8+'\0', method[1] = '\0';// DEFLATE
        fout.write(method, 2 * sizeof(char));

        //File last modification time
        size_t s = whole_time->tm_sec / 2;
        size_t m = whole_time->tm_min;
        size_t h = whole_time->tm_hour;
        h <<= 6; // 将小时放到最高位，并左移6位来存放分钟
        h |= m;
        h <<= 5; // 左移5位来存放秒数信息
        h |= s;
        
        char *time = (char*)&h;
        
        fout.write(time, 2 * sizeof(char));

        //File last modification date
        int d = whole_time->tm_mday;
        int mon = whole_time->tm_mon + 1;
        int y = whole_time->tm_year - 80;

        y <<= 4; // 将年份存储在最高位，左移4位，用来存储月份
        y |= mon;
        y <<= 5;// 左移五位用来存储日
        y |= d;
     
        
        char *date = (char*)&y;
        fout.write(date, 2 * sizeof(char));

        //CRC-32 of uncompressed data
        
        int file_crc = crc32((unsigned char*)buffer, length);

        char *CRC = (char*)&file_crc;
        fout.write(CRC, 4 * sizeof(char));
        
        //uncompressed size
        int buffer_len = strlen(buffer);


        //Compressed size
        
        size_t compressed_size = 0;
        char *com_size;
        com_size = (char*)&compressed_size;
        int compressed_size_pos = fout.tellp();
        fout.write(com_size, 4 * sizeof(char));
        
        //Uncompressed size
        
        char *uncom_size = (char*)&buffer_len;
        fout.write(uncom_size, 4 * sizeof(char));
        
        //File name length
        int read_filename_len = read_filename.size();
        char *file_name_len = (char*)&read_filename_len;
        fout.write(file_name_len, 2 * sizeof(char));

        //Extra field length
        char extra_len[2];
        extra_len[0] = '\0', extra_len[1] = '\0';
        fout.write(extra_len, 2 * sizeof(char));

        //file name writting
        const char *file_name = read_filename.data();
        fout.write(file_name, read_filename.size() * sizeof(char));

        // gettimeofday(&io_time2, NULL);
//         total_io_time2 += (io_time2.tv_sec - io_time1.tv_sec) + (double)(io_time2.tv_usec -
// io_time1.tv_usec) / 1000000.0;

        

        //static huffman compression
        lz77(fout, buffer);

        // gettimeofday(&io_time3, NULL);
        int pos = fout.tellp();
        fout.seekp(compressed_size_pos, ios::beg);

        int new_compressed_size = total_bit/8;
        
        char* new_com_size = (char*)&new_compressed_size;
        fout.write(new_com_size, 4 * sizeof(char));
        fout.seekp(pos, ios::beg);

        
       
        // central file header signature
        char cen_file_header_sig[4];
        cen_file_header_sig[0] = 'P', cen_file_header_sig[1] = 'K', cen_file_header_sig[2] = 1 + '\0', cen_file_header_sig[3] = 2 + '\0';
        fout.write(cen_file_header_sig, 4 * sizeof(char));

        // version made by and version needed to extract
        char made_by_version[2];
        made_by_version[0] = 0xa + '\0', made_by_version[1] = '\0';
        fout.write(made_by_version, 2 * sizeof(char));
        fout.write(made_by_version, 2 * sizeof(char));

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
        fout.write(new_com_size, 4 * sizeof(char));
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
        int central_directory_size = 46 + read_filename_len;
        char *size_cen_dir = (char*)&central_directory_size;
        fout.write(size_cen_dir, 4 * sizeof(char));

        //offset of start of central directory with respect to the starting disk number
        int start_offset = 30 + read_filename_len + new_compressed_size;
        char *offset = (char*)&start_offset;
        fout.write(offset, 4 * sizeof(char));

        //zip file comment length
        char com_len[2];
        com_len[0] = '\0', com_len[1] = '\0';
        fout.write(com_len, 2 * sizeof(char));

        length = fin.tellg();
        

        
    }
    
    fin.close();
    fout.close();
//     gettimeofday(&io_time4, NULL);
//     total_io_time2 += (io_time4.tv_sec -io_time3.tv_sec) + (double)(io_time4.tv_usec -
// io_time3.tv_usec) / 1000000.0;
    // write_bin_file(buffer, length);
    // cout << "The part2 of IO time is " << total_io_time2 << "s" << endl;
    delete(buffer); 
    return true;
    
}

