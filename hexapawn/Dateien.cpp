#include <fstream>

int main(int argc, char** argv){
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    std::ifstream ifile("data2/" + std::to_string(rows) + "x" + std::to_string(cols), std::ios_base::in | std::ios_base::binary);
    std::ofstream ofile("data3/" + std::to_string(rows) + "x" + std::to_string(cols), std::ios_base::out | std::ios_base::binary);
    while(true){
        char next = ifile.get();
        if(next != 0){
            ifile.close();
            ofile.close();
            return 0;
        }
        next = ifile.get();
        next = ifile.get();
        for(int i = 0; i < 6; i++){
            ofile.put(ifile.get());
        }
    }
    return 0;
}
