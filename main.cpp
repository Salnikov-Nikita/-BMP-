#include <fstream>
#include <iostream>
#include <string>
#include <memory>
using namespace std;


class BMP_24 {
   private:
    unique_ptr<bool[]> bmp;
    int width = 0;
    int height = 0;

    template <typename T>
    T read(istream &f, size_t offset) const {
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
            width = read<int32_t>(in, 18);
            height = read<int32_t>(in, 22);
            int bpp = read<uint16_t>(in, 28);
            in.seekg(54);
            // cout << "width = " << width << '\n' << "height = " << height << '\n' <<
            // "bpp = " << bpp << endl;
            int row_size = (bpp * width + 31) / 32 * 4;
            bmp.reset(new bool[height * width]);
            unique_ptr <char[]> row(new char[row_size]);

            for (int i = 0; i < height; i++) {
                in.read(row.get(), row_size);
                for (int j = 0; j < width; j++) bmp[i * width + j] = row[j * bpp / 8] != 0;
            }
        }
        in.close();
    }
    void displayBMP() const {
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) cout << (bmp[i * width + j] ? "█" : " ");
            cout << endl;
        }
    }
    void closeBMP() { 
        bmp.reset(); 
        width = 0;
        height = 0;    
    }
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