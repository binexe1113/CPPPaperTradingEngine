# Paper Trading Engine

Uma engine de **paper trading** que permite simular operações de compra e venda de ações em tempo real sem arriscar dinheiro real. Ideal para testar estratégias de investimento, estudar comportamento de mercado e validar algoritmos de trading.

## Índice

- [Funcionalidades](#funcionalidades)  
- [Tecnologias Utilizadas](#tecnologias-utilizadas)  
- [Como Usar](#como-usar)  
- [Exemplos](#exemplos)  
- [Contribuindo](#contribuindo)  
- [Licença](#licença)  

## Funcionalidades

- Simulação de compra e venda de ações com capital inicial configurável.  
- Histórico detalhado de operações e saldo em tempo real.  
- Suporte a múltiplos tickers.  
- Estratégias customizáveis de trading.  
- Visualização de preços históricos e gráficos interativos (opcional).  
- Logs de performance e métricas de retorno.  

## Tecnologias Utilizadas

- C++ para a lógica principal da engine.  
- [httplib](https://github.com/yhirose/cpp-httplib) para requisições HTTP (Yahoo Finance API).  
- [GLFW + ImGui + ImPlot](https://github.com/ocornut/imgui) para interface gráfica e gráficos (opcional).  
- STL para manipulação de dados e containers.  
- CMake para compilação e gerenciamento do projeto.  

## Como Usar

1. Clone o repositório:  
   ```bash
   git clone https://github.com/seu-usuario/paper-trading-engine.git
   cd paper-trading-engine
   ```

2. Compile o projeto:  
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Execute a engine:  
   ```bash
   ./PaperTradingEngine
   ```

4. Configure os tickers e capital inicial no arquivo `config.json` ou diretamente no código.  

## Exemplos

```cpp
// Criando um portfólio e comprando ações
Portfolio portfolio(10000); // capital inicial 10.000
portfolio.buy("AAPL", 10, 150.0); // compra 10 ações de AAPL a 150 cada
portfolio.sell("AAPL", 5, 155.0); // vende 5 ações de AAPL a 155 cada
```

Saída esperada:

```
Saldo atual: 10075
Histórico de operações:
[COMPRA] AAPL 10 x 150
[VENDA] AAPL 5 x 155
```

## Contribuindo

Contribuições são bem-vindas!  
1. Fork o projeto  
2. Crie uma branch (`git checkout -b feature/nova-funcionalidade`)  
3. Commit suas alterações (`git commit -am 'Adiciona nova funcionalidade'`)  
4. Push para a branch (`git push origin feature/nova-funcionalidade`)  
5. Abra um Pull Request  

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).