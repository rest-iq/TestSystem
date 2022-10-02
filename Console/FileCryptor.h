#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;
class FileCryptor 
{
    int shift = 20;
public:
    void cryptDirectory(const fs::path& dir) {
        if (!is_directory(dir)) {
            return;
        }
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                crypt(entry);
            }
            if (entry.is_directory()) {
                cryptDirectory(entry);
            }
        }
    }
    void decryptDirectory(const fs::path& dir) {
        if (!is_directory(dir)) {
            return;
        }
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                decrypt(entry);
            }
            if (entry.is_directory()) {
                decryptDirectory(entry);
            }
        }
    }
    void crypt(const fs::path& file) {
        if (!is_regular_file(file)) {
            return;
        }
        ifstream inputFile(file);
        fs::path tempFilePath = file.parent_path().append("temp.data");
        ofstream tempFile(tempFilePath);
        const int BUFFER_SIZE = 1000;
        char* buffer = new char[BUFFER_SIZE];
        char* cryptBuffer = new char[BUFFER_SIZE];
        do {
            inputFile.read(buffer, 1000);
            auto read = inputFile.gcount();
            for (long i = 0; i < read; ++i) {
                char val = buffer[i];
                char cryptVal;
                if (((int)val + shift) > 254) {
                    cryptVal = (char)(254 - ((int)val + shift));
                }
                else {
                    cryptVal = (char)((int)val + shift);
                }
                cryptBuffer[i] = cryptVal;
            }
            if (read > 0) {
                tempFile.write(cryptBuffer, read);
            }
        } while (!inputFile.eof());
        inputFile.close();
        tempFile.close();
        delete[] buffer;
        delete[] cryptBuffer;
        fs::rename(tempFilePath, file);
    }
    void decrypt(fs::path cryptedFile) {
        if (!is_regular_file(cryptedFile)) {
            return;
        }
        ifstream inputFile(cryptedFile);
        fs::path tempFilePath = cryptedFile.parent_path().append("temp.data");
        ofstream tempFile(tempFilePath);
        const int BUFFER_SIZE = 1000;
        char* cryptBuffer = new char[BUFFER_SIZE];
        char* buffer = new char[BUFFER_SIZE];
        do {
            inputFile.read(cryptBuffer, BUFFER_SIZE);
            auto read = inputFile.gcount();
            for (long i = 0; i < read; ++i) {
                char val = cryptBuffer[i];
                char decryptVal;
                if (((int)val - shift) < 254) {
                    decryptVal = (char)((int)val - shift);
                }
                else {
                    decryptVal = (char)(254 + ((int)val - shift));
                }
                buffer[i] = decryptVal;
            }
            if (read > 0) {
                tempFile.write(buffer, read);
            }
        } while (!inputFile.eof());
        inputFile.close();
        tempFile.close();
        delete[] buffer;
        delete[] cryptBuffer;
        fs::rename(tempFilePath, cryptedFile);
    }
};