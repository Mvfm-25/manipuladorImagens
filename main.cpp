// Estado inicial do projeto, estou lendo apenas lendo um arquivo.txt e imprimindo seu conteúdo na tela.
// [mvfm]

// Criado : 05/11/2025  ||  Última modificação: 05/11/2025

#include <iostream>
#include <fstream>
#include <string>

int main(){
    std::ifstream arquivo("img/texto.txt");
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