// Estado inicial do projeto, estou lendo apenas lendo um arquivo.txt e imprimindo seu conteúdo na tela.
// [mvfm]

// Criado : 05/11/2025  ||  Última modificação: 05/11/2025

#include <iostream>
#include <fstream>
#include <string>

// Trazendo da biblioteca stb_image por conveniência do programador.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

// Protótipos
int lerArquivo(std::string caminhoArquivo);

int main(){
    std::string caminhoArquivo = "img/texto.txt";
    lerArquivo(caminhoArquivo);
    return 0;
}

int lerArquivo(std::string caminhoArquivo){
    std::ifstream arquivo(caminhoArquivo);
    std::string linha;
    std::string conteudo;

    // Erro ao abrir o arquivo.
    if(!arquivo.is_open()){
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return 1;
    }

    // Passando linha à linha.
    while(std::getline(arquivo, linha)){
        conteudo += linha + "\n";
    }

    // Fechando o arquivo.
    arquivo.close();

    // Finalmente imprimindo o conteúdo
    std::cout << conteudo;
    return 0;

}

int carregaImagem(std::string caminhoArquivo){
    int largura, altura, canais;
    // Fazendo a conversão de String para uma 'C-Style' string.
    unsigned char* img = stbi_load(caminhoArquivo.c_str(), &largura, &altura, &canais, 0);

    // Caso null, basicamente.
    if(!img){
        std::cout << "Falha ao carregar imagem!";
        return 0;
    }


}