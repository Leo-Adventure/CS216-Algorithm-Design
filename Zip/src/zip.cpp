#include "zip.hpp"

static unsigned int crc_table[256]; 

static int remain_len = 0;
static size_t total_bit = 0;
static int bits = 0; 

map<size_t, size_t> pos_hash_map;
map<size_t, queue<int>> hash_pos_map;


bool write_normal_file(string filename){
    ofstream fout(filename);
    fout << "Hello world!Hello world!Hello world!Hello world!Hello world!Hello world!Hello world!Hello world!Hello world!Hello world! \
    Hello world!Hello world! Hello world!Hello world!Hello world!Hello world!Hello world!" ;
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




void emitBytes(ofstream& fout)
{
  while (remain_len >= 8)
  {
    int num = (bits >> (remain_len - 8)) & 0xff;
    int reverse_num = 0;
	for(int i = 0; i < 8; i++){
		reverse_num |= (num & 0b1);
		
		reverse_num <<= 1;
		num >>= 1;
	}
	reverse_num >>= 1;
    char *c = (char *)&reverse_num;
	bitset<8> bitvec2(reverse_num);
    // cout << "In emitting, num = " << bitvec2 << endl;
	fout.write(c, sizeof(char) * 1);
    remain_len -= 8;
   
  }
}

void emitBits(ofstream& fout, int bit, int l)
{
  bits <<= l;
  bits |= bit;
  remain_len += l;
  total_bit += l;
  emitBytes(fout);
}

size_t HashCode (const std::string &str) {
    size_t h = 0;
    for (size_t i = 0; i < str.size(); ++i)
        h = h * 31 + size_t(str[i]);
    return h;
}



void literal_to_bits(ofstream& fout, const char* chars, int len){
	for(int i = 0; i < len; i++){
		char ch = chars[i];
		int num = ch - '\0';
		if(num <= 143){
			num = (num + 0b00110000) & 0b11111111;
			emitBits(fout, num, 8);
		}else{
			num -= 144;
			num = (num + 0b110010000) & 0b111111111;
			emitBits(fout, num, 9);
		}
	}
    
}

void distance_len_to_bits(ofstream &fout, size_t distance, size_t len){
    // cout << "len = " << len << " distance = " << distance << endl;
	int len_idx = 0;
	int len_diff = 0;
    int len_extra = 0;
    // cout << "tellg() = " << fout.tellp() << endl;
	// 找到长度对应字段索引
	for(size_t i = 0; i < 29; i++){
		if(len < deflate_length_slot_base[i]){
			len_idx = i - 1;
			len_diff = len - deflate_length_slot_base[len_idx];
            
            len_extra = deflate_extra_length_bits[len_idx];
			break;
		}
	}
    int reverse_num = 0;
	for(int i = 0; i < len_extra; i++){
		reverse_num |= (len_diff & 0b1);
		reverse_num <<= 1;
		len_diff >>= 1;
	}
	reverse_num >>= 1;
    len_diff = reverse_num;
	if(len < 115){
		int len_num = len_idx + 0b0000001;
		len_num <<= len_extra;
		len_num |= (len_diff);
		
		int len_total_bit = 7 + len_extra;
        bitset<8> bitvec2(len_num);
        // cout << "len_num = " << bitvec2;
        // cout << ", len_total_bit = " << len_total_bit << endl; 
		
        
		emitBits(fout, len_num, len_total_bit);
	}else{
		int len_num = len_idx - 115 + 0b11000000;
		
		len_num <<= len_extra;
		len_num |= (len_diff);
		int len_total_bit = 8 + len_extra;
        bitset<8> bitvec2(len_num);
        // cout << "len_num = " << bitvec2;
        // cout << ", len_total_bit = " << len_total_bit << endl; 
		emitBits(fout, len_num, len_total_bit);
	}
	int dist_idx = 0;
	int dist_diff = 0;
	// 找到距离对应索引
	for(size_t i = 0; i < 30; i++){
		if(distance < deflate_offset_slot_base[i]){
			dist_idx = i - 1;
			dist_diff = distance - deflate_offset_slot_base[dist_idx];
			break;
		}
	}
	int dist_extra_bit = deflate_extra_offset_bits[dist_idx];
    // cout << "dist_extra_bit = " << dist_extra_bit << endl;
    int reverse_diff_num = 0;
	for(int i = 0; i < dist_extra_bit; i++){
		reverse_diff_num |= (dist_diff & 0b1);//10
		reverse_diff_num <<= 1;//10_
		dist_diff >>= 1;//0
	}
    // cout << "reverse_diff_num =" << reverse_diff_num << endl;
	reverse_diff_num >>= 1;
    dist_diff = reverse_diff_num;
    // cout << "dist_diff = " << dist_diff << endl;

	int dist_num = dist_idx;//5 bits
	dist_num <<= dist_extra_bit;
	dist_num |= dist_diff;
    bitset<9> bitvec3(dist_num);
    // cout << "dist_num = " << bitvec3;
   
	
	int dist_total_bit = 5 + dist_extra_bit;
    // cout << ", dist_total_bit = " << dist_total_bit << endl; 
	
	emitBits(fout, dist_num, dist_total_bit);
    // cout << "tellg() = " << fout.tellp() << endl;
    
}

pair<size_t, size_t> str_match(const string& buffer, const string& str, int str_start_pos){
	/*
	计算[i,i+4)的hash
	枚举hash表内所有hash值相等的pos:
	//此时说明[i,i+4)与[pos,pos+4)有很大概率相等
	从i开始逐字节比较，得到最长匹配长度
	返回最长匹配长度和位置
	*/
	size_t hash = HashCode(str.substr(0, 4));
	if(hash_pos_map.count(hash)){
		queue<int> que = hash_pos_map[hash];
		int que_size = que.size();
		int idx = 0, length = 0;
		for(int i = 0; i < que_size; i++){
			int pos = que.front();
			que.pop();
			
			int cnt = 0;
			int str_len = str.size();
			while(cnt < str_len && buffer[pos + cnt] == str[cnt] && pos + cnt < str_start_pos){
				cnt ++;
			}
			if(cnt >= length){
				length = cnt;
				idx = pos;
			}

			que.push(pos);
		}
		return {idx, length};
	}else{
		return {-1, -1};
	}
    
}

void update_hash(string buffer, size_t i){
	/*
		更新hash：
		把i-32768之前位置的hash从字典当中删除
		计算[i,i+4)的hash并放入hash表
	*/
	for(auto iter = pos_hash_map.rbegin(); iter != pos_hash_map.rend() && iter -> first >= i - 32768; iter++){
		size_t hash = iter -> second;
		queue<int>q = hash_pos_map[hash];

		while(!q.empty() && q.front() < i - 32768){
			q.pop();
		}
		pos_hash_map.erase(iter->first);

     }
	size_t hashcode = HashCode(buffer.substr(i, 4));
	pos_hash_map[i] = hashcode;
	hash_pos_map[hashcode].push(i);
    return;
}


bool lz77(ofstream& fout, const char * buffer){
    
    size_t max_len = strlen(buffer);
	
	
    string buffer_str = buffer;
    
	
	string out = "";
    if(!buffer){
        cerr << "The input char pointer is nullptr" << endl;
        cout << "Error in line " << __LINE__ << " in function " << __FUNCTION__ << " of file " << __FILE__ << endl;
        return false;
    }
    for(size_t i = 0; i < max_len;){
		
        size_t idx = 0, len = 0;
        if(i + 5 >= max_len){// 剩余空间 <= 5, 直接发射

            // out += buffer_str.substr(i);
			
			int len = max_len - i;
			literal_to_bits(fout, buffer + i, len);
			break;
			
        }else if(str_match(buffer_str, buffer_str.substr(i), i).first != -1){ // 字符串匹配
            
			idx = str_match(buffer_str, buffer_str.substr(i), i).first;
            len = str_match(buffer_str, buffer_str.substr(i), i).second;
			idx = i-idx;
            // cout << "len = " << len << endl;
            // cout << "distance = " << idx << endl;
            
            distance_len_to_bits(fout, idx, len);
			
            
			for(int j = 0; j < len; j ++){
                update_hash(buffer_str, i + j);
            }

            i += len;
        }else{ // 字符串不匹配，直接发射
			
            // out += buffer_str[i];
			literal_to_bits(fout, buffer + i, 1);
            update_hash(buffer_str, i);
            i++;
        }
    }
	// cout << "out = " << out << endl;
	// output = out.data();
	
    return true;
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
        method[0] = 8+'\0', method[1] = '\0';// DEFLATE
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

        int data_header = 0b110;
        emitBits(fout, data_header, 3);

        //static huffman compression
        lz77(fout, buffer);

        //结束
	    emitBits(fout, 0, 7);
	
        // 字节补齐
        int remain_bit = (total_bit / 8 + 1) * 8 - total_bit;
        if(remain_bit){
            emitBits(fout, 0, remain_bit);
        }

        int pos = fout.tellp();
        fout.seekp(compressed_size_pos, ios::beg);
        // cout << "fout.tellp() = " << fout.tellp() << endl;
        int new_compressed_size = total_bit/8;
        
        char* new_com_size = (char*)&new_compressed_size;
        fout.write(new_com_size, 4 * sizeof(char));
        fout.seekp(pos, ios::beg);
        // cout << "fout.tellp() = " << fout.tellp() << endl;
        
       
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
        char size_cen_dir[4];
        size_cen_dir[0] = 0x36 + '\0', size_cen_dir[1] = '\0', size_cen_dir[2] = '\0', size_cen_dir[3] = '\0';
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
        
        
        // cout << "length = " << length  << " bytes" << endl;
        // cout << buffer << endl;
        
    }
    
    fin.close();
    fout.close();
    // write_bin_file(buffer, length);
    delete(buffer);
    return true;
    
}
