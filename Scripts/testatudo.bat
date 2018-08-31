@echo off
REM executa os testes da pasta de scripts do projeto ModularT1

del estatisticas.estat

..\Release\ModularT1.exe      /sTesteTotal        /lTesteTotal        /aestatisticas


REM para visualizar as estatisiticas use o programa exbestat da pasta simples, com a sintaxe: exbestat /eestatisticas
