#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class BMP_24 {
private:
    vector<vector <int> > bmp;   

    //template <>
    int32_t read_i32(istream &f, size_t offset) {
        int32_t value;
        f.seekg(offset);
        f.read((char *)&value, sizeof(value));
        return value;
    }

    int16_t read_i16(istream &f, size_t offset) {
        int16_t value;
        f.seekg(offset);
        f.read((char *)&value, sizeof(value));
        return value;
    }

public:
    void openBMP(const string & file_path) {
        ifstream in;
        in.open(file_path);
        if (in.is_open()){
            int width = read_i32(in, 18);
            int height = read_i32(in, 22);
            int bpp = read_i16(in, 28);
            in.seekg(54);
            //cout << "width = " << width << '\n' << "height = " << height << '\n' <<  "bpp = " << bpp << endl;
            int row_size = (bpp * width + 31) / 32 * 4;
            bmp.resize(height);
            for (int i = 0; i < height; i++) {
                bmp[i].resize(width);
                vector <char> row(row_size);      
                in.read(row.data(), row_size);
                for (int j = 0; j < width; j++) {
                    bmp[i][j] = row[j * bpp / 8] != 0;
                }     
            }
        }
        in.close();
    }
    void displayBMP() {
        for (int i = bmp.size() - 1; i >= 0; i--) {
            for (int j = 0; j < bmp[i].size(); j++) {
                cout << (bmp[i][j] ? '#' : ' ');
            }
            cout << endl;
        }
    }
};
// ./a.out "/Users/nikita/Git/Консольное приложение, для чтения и отображения BMP файлов/bw.bmp"
int main(int argc, char** argv) {
    //getline(cin, file_path);
    cout << "khrbfg\n";
    string file_path;
    if (argc > 1) {
        file_path = argv[1];
    } else {
        getline(cin, file_path);
    }

    BMP_24 b;
    b.openBMP(file_path);
    b.displayBMP();
    return 0;
}