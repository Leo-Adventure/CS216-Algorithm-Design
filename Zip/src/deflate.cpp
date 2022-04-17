#include"deflate.hpp"

#include<bitset>
map<size_t, size_t> pos_hash_map;
map<size_t, queue<int>> hash_pos_map;

int remain_len = 0;
size_t total_bit = 0;
int bits = 0; 
// double total_io_time = 0;
void emitBytes(ofstream& fout)
{
  while (remain_len >= 8)
  {
    unsigned int num = (bits >> (remain_len - 8)) & 0xff;
    unsigned int reverse_num = 0;
	for(int i = 0; i < 8; i++){
		reverse_num |= (num & 0b1);
		
		reverse_num <<= 1;
		num >>= 1;
	}
	reverse_num >>= 1;
    char *c =(char*)&reverse_num;

	timeval write_time1, write_time2;
	// gettimeofday(&write_time1, NULL);
	fout.write(c, sizeof(char) * 1);
// 	gettimeofday(&write_time2, NULL);
// 	total_io_time += (write_time2.tv_sec - write_time1.tv_sec) + (double)(write_time2.tv_usec -
// write_time1.tv_usec) / 1000000.0;
    remain_len -= 8;
   
  }
}

/*
@parameter fout: the file stream object
@parameter bit: the binary number that represents bit stream
@parameter l: the length of bit
*/

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



void literal_to_bits(ofstream& fout, const unsigned char* chars, int len){

	for(int i = 0; i < len; i++){
		unsigned char ch = chars[i];
       	int num = ch;
        if(num >= 0 && num <= 143){
			num = (num + 0b00110000);
           
			emitBits(fout, num, 8);
		}else{
			
			num -= 144;
			num = (num + 0b110010000) ;
            
            emitBits(fout, num, 9);
		}
	}
  
}

void distance_len_to_bits(ofstream &fout, size_t distance, size_t len){
   
	int len_idx = 0;
	int len_diff = 0;
    int len_extra = 0;

	// 找到长度对应字段索引
	for(size_t i = 0; i < 29; i++){
		if(len < deflate_length_slot_base[i]){
			len_idx = i - 1;
			len_diff = len - deflate_length_slot_base[len_idx];
            
            len_extra = deflate_extra_length_bits[len_idx];
			break;
		}
	}
    unsigned int reverse_num = 0;
	for(int i = 0; i < len_extra; i++){
		reverse_num |= (len_diff & 0b1);
		reverse_num <<= 1;
		len_diff >>= 1;
	}
	reverse_num >>= 1;
    len_diff = reverse_num;
	if(len < 115){
		unsigned int len_num = len_idx + 0b0000001;
		len_num <<= len_extra;
		len_num |= (len_diff);
		
		unsigned int len_total_bit = 7 + len_extra;
       	emitBits(fout, len_num, len_total_bit);
	}else{
		unsigned int len_num = len_idx - 115 + 0b11000000;
		
		len_num <<= len_extra;
		len_num |= (len_diff);
		unsigned int len_total_bit = 8 + len_extra;
       
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
	unsigned int dist_extra_bit = deflate_extra_offset_bits[dist_idx];

    int reverse_diff_num = 0;
	for(int i = 0; i < dist_extra_bit; i++){
		reverse_diff_num |= (dist_diff & 0b1);//10
		reverse_diff_num <<= 1;//10_
		dist_diff >>= 1;//0
	}

	reverse_diff_num >>= 1;
    dist_diff = reverse_diff_num;
	int dist_num = dist_idx;//5 bits
	dist_num <<= dist_extra_bit;
	dist_num |= dist_diff;
	int dist_total_bit = 5 + dist_extra_bit;

	
	emitBits(fout, dist_num, dist_total_bit);
   
    
}

/*
@parameter buffer: 缓冲区对应的字符串
@parameter str: 待匹配的字符串
@parameter str_start_pos: 待匹配字符串在原有字节流的起始位置
*/
pair<size_t, size_t> str_match(const string& buffer, const string& str, int str_start_pos){

	size_t hash = HashCode(str.substr(0, 4));
	if(hash_pos_map.count(hash)){ // 如果缓冲区有匹配的字符串
		queue<int> que = hash_pos_map[hash];
		int que_size = que.size();
		int idx = 0, length = 0;
		for(int i = 0; i < que_size; i++){
			int pos = que.front();
			que.pop();
			if(pos < str_start_pos - 5000){
				pos_hash_map.erase(pos);
				continue;
			}
			int cnt = 0;
			int str_len = str.size();
			while(cnt < str_len && buffer[pos + cnt] == str[cnt] && pos + cnt < str_start_pos){
				cnt ++;
			}
			if(cnt >= length){ // 记录最长字符串的起始位置以及长度
				length = cnt;
				idx = pos;
			}
			que.push(pos);
		}

        if(length < 4){ // 只记录长度至少为4 的字符串信息，不满足则返回{-1，-1}
            return {-1, -1};
        }
		else{
            return {idx, length};
        }
	}else{
		return {-1, -1};
	}
}

void update_hash(string buffer, int i){
    // 遍历滑动窗口之外的元素进行删除

	size_t hashcode = HashCode(buffer.substr(i, 4));
	pos_hash_map[i] = hashcode;
  
	hash_pos_map[hashcode].push(i);

    return;
}

/*
@parameter fout: 输出流对象
@parameter buffer: 需要进行lz77 编码的字节流
*/

bool lz77(ofstream& fout, const char * buffer){

    int data_header = 0b110;
    emitBits(fout, data_header, 3); // 标识文件块信息以及 deflate 编码信息的比特
    
    size_t max_len = strlen(buffer);
	
    string buffer_str = buffer;

	string out = "";
    if(!buffer){ // 判断传入空指针的情况
        cerr << "The input char pointer is nullptr" << endl;
        cout << "Error in line " << __LINE__ << " in function " << __FUNCTION__ << " of file " << __FILE__ << endl;
        return false;
    }
    for(size_t i = 0; i < max_len;){
       
        size_t idx = 0, len = 0;
        if(i + 5 >= max_len){// 剩余空间 <= 5, 直接发射
			int len = max_len - i;
			literal_to_bits(fout, (unsigned char*)(buffer + i), len);
			break;
			
        }
        
        else if(str_match(buffer_str, buffer_str.substr(i), i).first != -1){ // 字符串匹配
            
			idx = str_match(buffer_str, buffer_str.substr(i), i).first; // 返回最长匹配字符串起始位置的索引
            len = str_match(buffer_str, buffer_str.substr(i), i).second; // 返回最长匹配字符串的长度

			idx = i-idx;// 将索引转换成为距离
            
            distance_len_to_bits(fout, idx, len); // 将长度以及距离进行编码后发射
			for(int j = 0; j < len; j ++){
                update_hash(buffer_str, i + j);
            }

            i += len;
        }
        else{ // 字符串不匹配，直接发射 literal
			literal_to_bits(fout, (unsigned char*)(buffer + i), 1);
            update_hash(buffer_str, i);
            i++;
        }
    }

     //结束
    emitBits(fout, 0, 7);

    // 字节补齐
    int remain_bit = (total_bit / 8 + 1) * 8 - total_bit;
    if(remain_bit){
        emitBits(fout, 0, remain_bit);
    }
	
    return true;
}

