#include "zip.hpp"


int main(int argc, char * argv[]){
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
    
    convert_file_to_zip(file_name, output_file_name);
    return 0;
}
