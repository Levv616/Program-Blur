#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>

void apply_blur(unsigned char* in, unsigned char* out, int w, int h, int c, int k_size) {
    int r = k_size / 2; // Radius kernel

    // Loop Y: Menelusuri baris gambar
    for (int y = 0; y < h; y++) {
        // Loop X: Menelusuri kolom gambar
        for (int x = 0; x < w; x++) {
            // Loop CH: Menelusuri channel (R, G, B, A)
            for (int ch = 0; ch < c; ch++) {
                float sum = 0.0f;
                int count = 0;

                // Loop Kernel Y (Jendela tetangga)
                for (int ky = -r; ky <= r; ky++) {
                    // Loop Kernel X (Jendela tetangga)
                    for (int kx = -r; kx <= r; kx++) {
                        int py = y + ky;
                        int px = x + kx;

                        // Boundary Check: Pastikan piksel tetangga tidak keluar dari gambar
                        if (px >= 0 && px < w && py >= 0 && py < h) {
                            // Rumus akses array 1D untuk gambar interleaved:
                            // Index = (py * width + px) * channels + channel_offset
                            sum += in[(py * w + px) * c + ch];
                            count++;
                        }
                    }
                }

                // Hitung rata-rata dan simpan ke buffer output
                out[(y * w + x) * c + ch] = (unsigned char)(sum / count);
            }
        }
    }
}

int main() {
    // Ganti dengan path gambar Anda
    const char *input_path = "input.png"; // Note: typo di bilinier.c, sebaiknya ganti ke file yang benar
    const char *output_path = "blurred_output.png";

    int width, height, channels;
    
    // Baca gambar
    unsigned char *src = stbi_load(input_path, &width, &height, &channels, 0);
    if (!src) {
        fprintf(stderr, "Gagal membaca file %s. Pastikan file ada.\n", input_path);
        return 1;
    }

    printf("Gambar sumber: %dx%d (%d channel)\n", width, height, channels);
    printf("Sedang memproses blur...\n");

    // Alokasi memori untuk gambar output
    unsigned char *blurred = (unsigned char*)malloc(width * height * channels);
    if (!blurred) {
        fprintf(stderr, "Gagal alokasi memori.\n");
        stbi_image_free(src);
        return 1;
    }

    // Panggil fungsi blur (ukuran kernel 5x5)
    int kernel_size = 5;
    apply_blur(src, blurred, width, height, channels, kernel_size);

    // Simpan gambar hasil blur
    if (!stbi_write_png(output_path, width, height, channels, blurred, width * channels)) {
        fprintf(stderr, "Gagal menyimpan file %s\n", output_path);
    } else {
        printf("Gambar hasil blur (kernel %dx%d) disimpan ke: %s\n", kernel_size, kernel_size, output_path);
    }

    // Bebaskan memori
    stbi_image_free(src);
    free(blurred);

    return 0;
}
