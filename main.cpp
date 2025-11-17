// Estado inicial do projeto. Aprendendo a como incorporar arquivos da biblioteca stb_image para minha conveniência.
// Por agora, estou testando apenas o método de imagens grayscale.
// [mvfm]

// Criado : 05/11/2025  ||  Última modificação: 17/11/2025

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Trazendo da biblioteca stb_image por conveniência do programador.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

// Protótipos
int lerArquivo(std::string caminhoArquivo);
int bwImagem(std::string caminhoArquivo);

int main(){
    bwImagem("img/Puppet_Tails_Is_Shocked.png");
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

int bwImagem(std::string caminhoArquivo){
    int largura, altura, canais;

    // Fazendo a conversão de String para uma 'C-Style' string.
    // Do jeito que estou lendo, utilizando o stbi, consigo carregar qualquer formato comum.
    unsigned char* img = stbi_load(caminhoArquivo.c_str(), &largura, &altura, &canais, 0);

    // Caso null, basicamente.
    if(!img){
        std::cout << "Falha ao carregar imagem!";
        return 1;
    }

    // Pra ter certeza que só retorna 1 canal, Grayscale
    std::vector<unsigned char> imgBW(largura * altura);

    // Bem auto-explicativo.
    bool temRGB = (canais == 3 || canais == 4);

    for(int y = 0; y < altura; y++){
        for(int x = 0; x < largura; x++){

            int idxOrigem = (y * largura + x) * canais;
            unsigned char r, g, b;

            if(temRGB){
                r = img[idxOrigem + 0];
                g = img[idxOrigem + 1];
                b = img[idxOrigem + 2];
            } else {
                r = g = b = img[y * largura + x];
            }

            // Convertendo finalmente para cinza.
            unsigned char cinza = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

            imgBW[y * largura + x] = cinza;

        }
    }

    // Fazendo parte de saída
    // Sempre salvando como png por enquanto.
    const char* saida = "img/saida_bw.png";

    int sucesso = stbi_write_png(
        saida,
        largura,
        altura,
        1, // 1 canal.
        imgBW.data(),
        largura
    );

    if(!sucesso){
        std::cerr << "Falha ao salvar a imagem resultante!" << std::endl;
        stbi_image_free(img);
        return 1;
    }

    std::cout << "Imagem resultante salva como : "  << saida << std::endl;
    stbi_image_free(img);
    return 0;
}