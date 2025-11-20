// Implementados método sépia (alguns commits atrás) e gaussian blur.
// ironicamente deixando os mais fáceis por último agora.
// [mvfm]
//
// Criado : 05/11/2025  || Última vez modificado : 20/11/2025

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Pra não ter que explicitamente ter que declarar PI quando compilo por minha máquina Windows.
#define _USE_MATH_DEFINES
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

// Enum para tipos de processamento
enum TipoProcessamento {
    TONS_CINZA = 1,
    CORES_INVERTIDAS = 2,
    SEPIA = 3,
    GAUSSIAN_BLUR = 4,
    SOBEL = 5
};

// Protótipos
unsigned char* carregaImg(std::string caminhoArquivo, int* largura, int* altura, int* canais);
int salvaImg(std::string caminhoArquivo, int largura, int altura, int canais, const unsigned char* dados);
int processaImagem(std::string caminhoArquivo, std::string caminhoSaida, TipoProcessamento tipo);
std::vector<float> criaKernelGaussiano(int raio, float sigma);

// O que o usuário vê e interage com.
int main(){
    std::string arquivoEntrada, arquivoSaida;
    int escolha = 0;
    
    std::cout << "Digite o caminho da imagem: " << std::endl;
    std::getline(std::cin, arquivoEntrada);

    std::cout << "Digite o nome do arquivo de saída: " << std::endl;
    std::getline(std::cin, arquivoSaida);

    std::cout << "Digite como queiras editar a imagem selecionada: " << std::endl;
    std::cout << "1 - Tons de Cinza || 2 - Cores invertidas || 3 - Sépia || 4 - Gaussian Blur  || 5 - Detecção de Bordas" << std::endl;
    std::cin >> escolha;
    
    processaImagem(arquivoEntrada, arquivoSaida, (TipoProcessamento)escolha);
    
    return 0;
}

// Método principal pra carregar a imagem.
// Chamada de stbi basicamente.
unsigned char* carregaImg(std::string caminhoArquivo, int* largura, int* altura, int* canais){
    unsigned char* img = stbi_load(caminhoArquivo.c_str(), largura, altura, canais, 0);
    if(!img){
        std::cerr << "Erro ao carregar a imagem: " << caminhoArquivo << std::endl; 
        return nullptr;
    }
    std::cout << "Imagem carregada: " << *largura << "x" << *altura << " com " << *canais << " canais" << std::endl;
    return img;
}

// Método principal pra salvar o output. 
// Basicamente só uma chamada de um método stbi com algumas prevenções por cima.
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

// Cria kernel necessário pra borrar a imagem.
std::vector<float> criaKernelGaussiano(int raio, float sigma) {
    int tamanho = 2 * raio + 1;
    std::vector<float> kernel(tamanho * tamanho);
    float soma = 0.0f;
    
    for(int y = -raio; y <= raio; y++) {
        for(int x = -raio; x <= raio; x++) {
            float expoente = -(x*x + y*y) / (2 * sigma * sigma);
            float valor = std::exp(expoente) / (2 * M_PI * sigma * sigma);
            kernel[(y + raio) * tamanho + (x + raio)] = valor;
            soma += valor;
        }
    }
    
    // Normaliza
    for(float& v : kernel) {
        v /= soma;
    }
    
    return kernel;
}

// Golden Goose.
int processaImagem(std::string caminhoArquivo, std::string caminhoSaida, TipoProcessamento tipo){
    int largura, altura, canais;
    unsigned char* img = carregaImg(caminhoArquivo, &largura, &altura, &canais);
    
    if(!img) return 1;
    
    // Determinar canais de saída baseado no tipo, aqui só pra ter que lidar com edições BW.
    int canaisSaida = (tipo == TONS_CINZA) ? 1 : canais;
    std::vector<unsigned char> imgProcessada(largura * altura * canaisSaida);
    
    // Configurações específicas para blur
    int raio = 3;
    float sigma = 1.5f;
    std::vector<float> kernel;
    if(tipo == GAUSSIAN_BLUR) {
        kernel = criaKernelGaussiano(raio, sigma);
    }
    
    // Loop principal, praticamente tudo acontece aqui.
    for(int y = 0; y < altura; y++){
        for(int x = 0; x < largura; x++){
            
            int idxOrigem = (y * largura + x) * canais;
            int idxDestino = (y * largura + x) * canaisSaida;
            
            // Escrito dessa maneira pra não ter que repetir tantas linhas de código como antes.
            // Agora está um pouco mais parecido com o trabalho de PBN.
            // Mas bonitinho também.
            switch(tipo) {
                
                // Uooooh thank you Frank Miller.
                case TONS_CINZA: {
                    unsigned char r, g, b;
                    
                    if(canais >= 3) {
                        r = img[idxOrigem + 0];
                        g = img[idxOrigem + 1];
                        b = img[idxOrigem + 2];
                    } else {
                        r = g = b = img[idxOrigem];
                    }
                    
                    unsigned char cinza = (unsigned char)(0.299f * r + 0.587f * g + 0.114f * b);
                    imgProcessada[y * largura + x] = cinza;
                    break;
                }
                
                // Método para negar as cores. Inverte as cores.
                case CORES_INVERTIDAS: {
                    imgProcessada[idxDestino + 0] = 255 - img[idxOrigem + 0];
                    imgProcessada[idxDestino + 1] = 255 - img[idxOrigem + 1];
                    imgProcessada[idxDestino + 2] = 255 - img[idxOrigem + 2];
                    
                    if(canais == 4) {
                        imgProcessada[idxDestino + 3] = img[idxOrigem + 3];
                    }
                    break;
                }
                
                // Método para 'envelhecer' a imagem.
                case SEPIA: {
                    float rTemp = (img[idxOrigem + 0] * 0.393f) + (img[idxOrigem + 1] * 0.769f) + (img[idxOrigem + 2] * 0.189f);
                    float gTemp = (img[idxOrigem + 0] * 0.349f) + (img[idxOrigem + 1] * 0.686f) + (img[idxOrigem + 2] * 0.168f);
                    float bTemp = (img[idxOrigem + 0] * 0.272f) + (img[idxOrigem + 1] * 0.534f) + (img[idxOrigem + 2] * 0.131f);
                    
                    imgProcessada[idxDestino + 0] = (unsigned char)(rTemp > 255 ? 255 : rTemp);
                    imgProcessada[idxDestino + 1] = (unsigned char)(gTemp > 255 ? 255 : gTemp);
                    imgProcessada[idxDestino + 2] = (unsigned char)(bTemp > 255 ? 255 : bTemp);
                    
                    if(canais == 4) {
                        imgProcessada[idxDestino + 3] = img[idxOrigem + 3];
                    }
                    break;
                }
                
                // Método para 'borrar' a imagem basicamente. Óculos sujos.
                case GAUSSIAN_BLUR: {
                    for(int c = 0; c < canais; c++) {
                        if(c == 3) { // Canal alpha
                            imgProcessada[idxDestino + c] = img[idxOrigem + c];
                            continue;
                        }
                        
                        float soma = 0.0f;
                        
                        // Convolução com kernel
                        for(int ky = -raio; ky <= raio; ky++) {
                            for(int kx = -raio; kx <= raio; kx++) {
                                int py = std::max(0, std::min(altura - 1, y + ky));
                                int px = std::max(0, std::min(largura - 1, x + kx));
                                
                                int idx = (py * largura + px) * canais + c;
                                int kidx = (ky + raio) * (2 * raio + 1) + (kx + raio);
                                
                                soma += img[idx] * kernel[kidx];
                            }
                        }
                        
                        imgProcessada[idxDestino + c] = (unsigned char)soma;
                    }
                    break;
                }

                // Método de detecção de bordas baseado em diferença de cores. Outro kernel.
                case SOBEL: {
                    float kernelX[3][3] = {
                        {-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}
                    };
                    float kernelY[3][3] = {
                        {-1, -2, -1},
                        { 0,  0,  0},
                        { 1,  2,  1}
                    };

                    // Converter para escala de cinza primeiro
                    // Não re-utilizando o processamento BW pois não tenho certeza se isso funcionaria.
                    unsigned char r, g, b;
                    if(canais >= 3) {
                        r = img[idxOrigem + 0];
                        g = img[idxOrigem + 1];
                        b = img[idxOrigem + 2];
                    } else {
                        r = g = b = img[idxOrigem];
                    }
                    unsigned char cinza = (unsigned char)(0.299f * r + 0.587f * g + 0.114f * b);

                    float magX = 0.0f;
                    float magY = 0.0f;

                    // Aplicando kernels com verificação de bordas
                    for(int ky = -1; ky <= 1; ky++) {
                        for(int kx = -1; kx <= 1; kx++) {
                            int py = std::max(0, std::min(altura - 1, y + ky));
                            int px = std::max(0, std::min(largura - 1, x + kx));
                            
                            int idx = (py * largura + px) * canais;
                            
                            // Pegar valor em escala de cinza do pixel vizinho
                            unsigned char pixelCinza;
                            if(canais >= 3) {
                                pixelCinza = (unsigned char)(0.299f * img[idx] + 
                                                            0.587f * img[idx + 1] + 
                                                            0.114f * img[idx + 2]);
                            } else {
                                pixelCinza = img[idx];
                            }
                            
                            magX += pixelCinza * kernelX[ky + 1][kx + 1];
                            magY += pixelCinza * kernelY[ky + 1][kx + 1];
                        }
                    }

                    // Calcular magnitude do gradiente
                    float magnitude = std::sqrt(magX * magX + magY * magY);
                    magnitude = std::min(255.0f, magnitude);

                    // Saída em escala de cinza
                    imgProcessada[idxDestino] = (unsigned char)magnitude;
                    
                    break;
                }
                
                default:
                    std::cerr << "Tipo de processamento inválido!" << std::endl;
                    stbi_image_free(img);
                    return 1;
            }
        }
    }
    
    int resultado = salvaImg(caminhoSaida, largura, altura, canaisSaida, imgProcessada.data());
    stbi_image_free(img);
    return resultado;
}