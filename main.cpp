#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BMP_24 {
   private:
    vector<vector<int>> bmp;

    template <typename T>
    T read(istream &f, size_t offset) {
        T value;
        f.seekg(offset);
        f.read((char *)&value, sizeof(value));
        return value;
    }

   public:
    void openBMP(const string &file_path) {
        ifstream in;
        in.open(file_path, ios::binary | ios::in);
        if (in.is_open()) {
            int width = read<int32_t>(in, 18);
            int height = read<int32_t>(in, 22);
            int bpp = read<uint16_t>(in, 28);
            in.seekg(54);
            // cout << "width = " << width << '\n' << "height = " << height << '\n' <<
            // "bpp = " << bpp << endl;
            int row_size = (bpp * width + 31) / 32 * 4;
            bmp.resize(height);
            for (int i = 0; i < height; i++) {
                bmp[i].resize(width);
                vector<char> row(row_size);
                in.read(row.data(), row_size);
                for (int j = 0; j < width; j++) bmp[i][j] = row[j * bpp / 8] != 0;
            }
        }
        in.close();
    }
    void displayBMP() {
        for (int i = bmp.size() - 1; i >= 0; i--) {
            for (int j = 0; j < bmp[i].size(); j++) cout << (bmp[i][j] ? "█" : " ");
            cout << endl;
        }
    }
    void closeBMP() { bmp.clear(); }
};
// ./a.out "/Users/nikita/Git/Консольное приложение, для чтения и отображения
// BMP файлов/bw.bmp"
int main(int argc, char **argv) {
    if (argc > 1) {
        string file_path = argv[1];
        BMP_24 b;
        b.openBMP(file_path);
        b.displayBMP();
        b.closeBMP();
    }
    return 0;
}