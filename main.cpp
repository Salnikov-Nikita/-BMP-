#include <fstream>
#include <iostream>
#include <memory>
#include <string>
using namespace std;

/*
Заголовок файла растровой графики (14 байт)
Сигнатура файла BMP (2 байт)
Размер файла (4 байт)
Не используется (2 байт)
Не используется (2 байт)
Местонахождение данных растрового массива (4 байт)

Длина этого заголовка (4 байт)
Ширина изображения (4 байт)
Высота изображения (4 байт)
Число цветовых плоскостей (2 байт)
Бит/пиксель (2 байт)
Метод сжатия (4 байт)
Длина растрового массива (4 байт)
Горизонтальное разрешение (4 байт)
Вертикальное разрешение (4 байт)
Число цветов изображения (4 байт)
Число основных цветов (4 байт)

*/

struct bmp_header {
    uint16_t signature;
    uint32_t size;
    uint16_t unused_1;
    uint16_t unused_2;
    uint32_t position;

    uint32_t header_len;
    uint32_t width;
    uint32_t height;
    uint16_t color_planes;
    uint16_t bpp;
    uint32_t compression_method;
    uint32_t raster_array_len;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    uint32_t count_colors;
    uint32_t count_prior_colors;

   private:
    template <typename T>
    void read_field(istream &f, T &value) const {
        f.read((char *)&value, sizeof(value));
    }

   public:
    void read(istream &f) {
        read_field(f, signature);
        read_field(f, size);
        read_field(f, unused_1);
        read_field(f, unused_2);
        read_field(f, position);
        read_field(f, header_len);
        read_field(f, width);
        read_field(f, height);
        read_field(f, color_planes);
        read_field(f, bpp);
        read_field(f, compression_method);
        read_field(f, raster_array_len);
        read_field(f, horizontal_resolution);
        read_field(f, vertical_resolution);
        read_field(f, count_colors);
        read_field(f, count_prior_colors);
    };
};

class BMP_24 {
   private:
    unique_ptr<bool[]> bmp;
    int width = 0;
    int height = 0;

   public:
    void openBMP(const string &file_path) {
        ifstream in;
        in.open(file_path, ios::binary | ios::in);
        if (in.is_open()) {
            bmp_header header;
            header.read(in);
            width = header.width;
            height = header.height;
            int bpp = header.bpp;
            in.seekg(header.position);
            int row_size = (bpp * width + 31) / 32 * 4;
            bmp.reset(new bool[height * width]);
            unique_ptr<char[]> row(new char[row_size]);

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