
# Bomb Party Game (Terminal Padronizado)

## Compilar
```bash
make
```

## Gerar Documentação
```bash
doxygen Doxyfile
```

Abra `docs/html/index.html` no navegador.

### Mudanças
- Todos os acessos ao terminal (posições ANSI) foram substituídos por funções no módulo `terminal_utils`.
- Layout do HUD padronizado: bomba (linhas 3–12), input (linha 20), feedback (linha 22).
- Código mais legível e centralização das posições de tela.
