// Métodos de edição continuam bem simples por enquanto, futuras implementações são :
// Tons de sépia, rotação da imagem, aumentar nitidez, gaussian blur.
// [mvfm]

// Criado : 05/11/2025  ||  Última modificação: 18/11/2025

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
int lerArquivoTexto(std::string caminhoArquivo);
int bwImagem(std::string caminhoArquivo, std::string caminhoSaida);
int revImagem(std::string caminhoArquivo, std::string caminhoSaida);

// Protótipos de Complementares.
unsigned char* carregaImg(std::string caminhoArquivo, int* largura, int* altura, int* canais);
int salvaImg(std::string caminhoArquivo, int largura, int altura, int canais, const unsigned char* dados);

int main(){
    std::string arquivoEntrada, arquivoSaida;

    int escolha = 0;
    
    std::cout << "Digite o caminho da imagem: " << std::endl;
    std::getline(std::cin, arquivoEntrada);

    std::cout << "Digite o nome do arquivo de saída: " << std::endl;
    std::getline(std::cin, arquivoSaida);

    std::cout << "Digite como queiras editar a imagem selecionada: " << std::endl;
    std::cout << "1 - Tons de Cinza || 2 - Cores invertidas " << std::endl;
    std::cin >> escolha;
    
    switch (escolha)
    {
        case 1 : bwImagem(arquivoEntrada, arquivoSaida); break;
        case 2 :revImagem(arquivoEntrada, arquivoSaida); break;
    }
    return 0;
}

int lerArquivoTexto(std::string caminhoArquivo){
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

// Novo método para lidar com o carregar de imagens, pra não repetir tantas linhas de código em novos métodos de manipulação.
unsigned char* carregaImg(std::string caminhoArquivo, int* largura, int* altura, int* canais){

    unsigned char* img = stbi_load(caminhoArquivo.c_str(), largura, altura, canais, 0);
    if(!img){
        std::cerr << "Erro ao carregar a imagem!" << caminhoArquivo << std::endl; 
        return nullptr;
    }

    std::cout << "Imagem carregada: " << *largura << "x" << *altura << " com " << *canais << " canais" << std::endl;
    return img;
}

// Novo método complementar, agora para salvar imagens.
int salvaImg(std::string caminhoArquivo, int largura, int altura, int canais, const unsigned char* dados){

    int sucesso = stbi_write_png(
        caminhoArquivo.c_str(),
        largura,
        altura,
        canais,
        dados,
        largura * canais
    );
    
    if(!sucesso){
        std::cerr << "Falha ao salvar imagem: " << caminhoArquivo << std::endl;
        return 1;
    }
    
    std::cout << "Imagem salva com sucesso: " << caminhoArquivo << std::endl;
    return 0;

}

int bwImagem(std::string caminhoArquivo, std::string caminhoSaida){
    int largura, altura, canais;

    // Do jeito que estou lendo, utilizando o stbi, consigo carregar qualquer formato comum.
    // Adendo : Testando ontem, carregando uma imagem .webp parece estar dando erro, mas não tenho certeza o pq.
    unsigned char* img = carregaImg(caminhoArquivo, &largura, &altura, &canais);

    // Só pra ter certeza 
    if(!img){return 1;}

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
    int resultado = salvaImg(caminhoSaida.c_str(), largura, altura, 1, imgBW.data());
    stbi_image_free(img);
    return resultado;
}

// Método para inverter as cores de uma imagem.
int revImagem(std::string caminhoArquivo, std::string caminhoSaida){

    std::cout << "Iniciando método para inverter as cores da imagem..." << std::endl;;

    int largura, altura, canais;
    unsigned char* img = carregaImg(caminhoArquivo, &largura, &altura, &canais);

    // Só pra ter certeza 
    if(!img){return 1;}

    // Levando em consideração os canais para o tamanho do novo arquivo.
    // Só leva em consideração os canais só no caso do bw.
    std::vector<unsigned char> revImagem(largura * altura * canais);

    if(!img){
        std::cout << "Falha ao carregar imagem!";
        return 1;
    }

    for(int y = 0; y < altura; y++){
        for(int x = 0; x < largura; x++){

            int idxOrigem = (y * largura + x) * canais;
            unsigned char r, g, b;

            // Mesma coisa, agora sem condicional pois estou assumindo que toda imagem enviada por aqui tem RGB.
            r = 255 - img[idxOrigem + 0];
            g = 255 - img[idxOrigem + 1];
            b = 255 - img[idxOrigem + 2];

            revImagem[idxOrigem + 0] = r;
            revImagem[idxOrigem + 1] = g;
            revImagem[idxOrigem + 2] = b;

            // Se tiver canal alpha, copie sem modificar
            if(canais == 4){
                revImagem[idxOrigem + 3] = img[idxOrigem + 3];
            }
        }
    }

    // Fazendo parte de saída
    // Sempre salvando como png por enquanto.
    int resultado = salvaImg(caminhoSaida.c_str(), largura, altura, canais, revImagem.data());
    stbi_image_free(img);
    return resultado;
}