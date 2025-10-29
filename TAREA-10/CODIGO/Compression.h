#pragma once
#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <string>

// Funciones públicas del módulo de compresión
bool huffmanCompressFile(const std::string& inputFile, const std::string& outputFile);
bool huffmanDecompressFile(const std::string& inputFile, const std::string& outputFile);

#endif
