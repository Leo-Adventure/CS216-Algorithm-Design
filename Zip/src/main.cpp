#include "zip.hpp"


int main(int argc, char * argv[]){
    // struct timeval total_t1, total_t2;
    
    string file_name = "";
    string output_file_name = "";
    if(argc < 2){
        cout << "Please input the .txt file that you want to compress: " << endl;
        cin >> file_name;
        cout << "Please enter the output file name: " << endl;
        cin >> output_file_name;
    }else if(argc < 3){
        file_name = argv[1];
        cout << "Please enter the output file name: " << endl;
        cin >> output_file_name;
    }else if(argc == 3){
        file_name = argv[1];
        output_file_name = argv[2];
    }else{
        cerr << "Too many input variable...." << endl;
        return 1;
    }
    // 计算 zip 程序运行总时间
    // gettimeofday(&total_t1, NULL);
    convert_file_to_zip(file_name, output_file_name);
//     gettimeofday(&total_t2, NULL);
//     double total_time_use = (total_t2.tv_sec - total_t1.tv_sec) + (double)(total_t2.tv_usec -
// total_t1.tv_usec) / 1000000.0;
//     cout << "The total time used in zipping = " << total_time_use << "s" <<endl;
    return 0;
}
