#include <iostream>
#include<string>
#include <fstream>

using namespace std;


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

bool convert_file_to_zip(string read_filename, string write_filename){
    char* buffer = nullptr;
    size_t length = 0;

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
        buffer = new char[length];
        fin.read(buffer, length);
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
        char time[2];
        time[0] = 'N', time[1] = 'S';
        fout.write(time, 2 * sizeof(char));

        //File last modification date
        char date[2];
        date[0] = 0x85 + '\0', date[1] = 0x54 + '\0';
        fout.write(date, 2 * sizeof(char));

        //CRC-32 of uncompressed data
        char CRC[4];
        CRC[0] = 0x95 + '\0'; CRC[1] = 0X19 + '\0'; CRC[2] = 0x85 + '\0'; CRC[3] = 0x1b + '\0';
        fout.write(CRC, 4 * sizeof(char));

        //Compressed size
        char com_size[4];
        com_size[0] = 0xc + '\0', com_size[1] = '\0', com_size[2] = '\0', com_size[3] = '\0';
        fout.write(com_size, 4 * sizeof(char));
        //Uncompressed size
        char uncom_size[4];
        uncom_size[0] = 0xc + '\0', uncom_size[1] = '\0', uncom_size[2] = '\0', uncom_size[3] = '\0';
        fout.write(uncom_size, 4 * sizeof(char));
        //File name length
        char file_name_len[2];
        file_name_len[0] = 8 + '\0', file_name_len[1] = '\0';
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
        char offset[4];
        offset[0] = 0x32 + '\0', offset[1] = '\0', offset[2] = '\0', offset[3] = '\0';
        fout.write(offset, 4 * sizeof(char));

        //zip file comment length
        char com_len[2];
        com_len[0] = '\0', com_len[1] = '\0';
        fout.write(com_len, 2 * sizeof(char));

        length = fin.tellg();
        
        
        cout << "length = " << length  << " bytes" << endl;
        cout << buffer << endl;
        
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
