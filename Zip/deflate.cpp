#include<iostream>
#include<map>


#define DEFLATE_NUM_PRECODE_SYMS		19
#define DEFLATE_MAX_MATCH_LEN			258
typedef unsigned char u8;

using namespace std;

// lengths value
/* Table: length slot => length slot base value */
static const unsigned deflate_length_slot_base[] = {
	3,    4,    5,    6,    7,    8,    9,    10,
	11,   13,   15,   17,   19,   23,   27,   31,
	35,   43,   51,   59,   67,   83,   99,   115,
	131,  163,  195,  227,  258,
};

// lengths bits
/* Table: length slot => number of extra length bits */
static const u8 deflate_extra_length_bits[] = {
	0,    0,    0,    0,    0,    0,    0,    0,
	1,    1,    1,    1,    2,    2,    2,    2,
	3,    3,    3,    3,    4,    4,    4,    4,
	5,    5,    5,    5,    0,
};


// distance value
/* Table: offset slot => offset slot base value */
static const unsigned deflate_offset_slot_base[] = {
	1,     2,     3,     4,     5,     7,     9,     13,
	17,    25,    33,    49,    65,    97,    129,   193,
	257,   385,   513,   769,   1025,  1537,  2049,  3073,
	4097,  6145,  8193,  12289, 16385, 24577,
};

// distance bits
/* Table: offset slot => number of extra offset bits */
static const u8 deflate_extra_offset_bits[] = {
	0,     0,     0,     0,     1,     1,     2,     2,
	3,     3,     4,     4,     5,     5,     6,     6,
	7,     7,     8,     8,     9,     9,     10,    10,
	11,    11,    12,    12,    13,    13,
};

/* Table: length => length slot */
static const u8 deflate_length_slot[DEFLATE_MAX_MATCH_LEN + 1] = {
	0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 12,
	12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 16,
	16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18,
	18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
	21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
	22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
	23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 28,
};

/*
 * A condensed table which maps offset => offset slot as follows:
 *
 *	offset <= 256: deflate_offset_slot[offset]
 *	offset > 256: deflate_offset_slot[256 + ((offset - 1) >> 7)]
 *
 * This table was generated by scripts/gen_offset_slot_map.py.
 */
static const u8 deflate_offset_slot[512] = {
	0, 0, 1, 2, 3, 4, 4, 5, 5, 6, 6, 6, 6, 7, 7, 7,
	7, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9,
	9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
	11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 0, 16, 17, 18, 18, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21,
	22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
};

/* The order in which precode codeword lengths are stored */
static const u8 deflate_precode_lens_permutation[DEFLATE_NUM_PRECODE_SYMS] = {
	16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

size_t HashCode (const std::string &str) {
    size_t h = 0;
    for (size_t i = 0; i < str.size(); ++i)
        h = h * 31 + size_t(str[i]);
    return h;
}

void literal_to_bits(char* chars, char* out){
    return;
}
void distance_len_to_bits(size_t distance, size_t len, char* out){
    return;
}

pair<size_t, size_t> str_match(string str){
    return {-1, -1};
}

void update_hash(size_t i){
    return;
}




bool lz77(const char * buffer, char *output){
    
    size_t max_len = strlen(buffer);
    string buffer_str = buffer;

    if(!buffer){
        cerr << "The input char pointer is nullptr" << endl;
        cout << "Error in line " << __LINE__ << " in function " << __FUNCTION__ << " of file " << __FILE__ << endl;
        return false;
    }
    for(size_t i = 0; i < max_len;){
        string out = "";
        size_t idx = 0, len = 0;
        if(i + 5 >= max_len){
            out = buffer_str.substr(i, max_len - i);
        }else if(str_match(buffer_str.substr(i, 4)).first != -1){
            idx = str_match(buffer_str.substr(i, 4)).first;
            len = str_match(buffer_str.substr(i, 4)).second;
            char * res = nullptr;
            distance_len_to_bits(i - idx, len, res);
            out = res;
            for(int j = 0; j < len; j ++){
                update_hash(i + j);
            }
            i += len;
        }else{
            out = buffer_str.at(i) + "";
            update_hash(i);
            i++;
        }
    }



    return true;


}

