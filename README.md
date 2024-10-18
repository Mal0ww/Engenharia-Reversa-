# Engenharia Reversa do Firmware de Roteador TP-Link TL-WR1042ND

## Visão Geral

Este projeto tem como foco a engenharia reversa do firmware de um roteador, visando analisar sua estrutura, extrair informações valiosas e entender seu funcionamento. O objetivo é estudar o firmware em nível binário, documentando os achados e fornecendo uma compreensão profunda de como o roteador opera, além de destacar possíveis vulnerabilidades de segurança, otimizações ou falhas.

**Importante:** Este projeto é estritamente para fins educacionais e de pesquisa. Não promovemos o uso ilegal ou antiético de técnicas de engenharia reversa. Todas as ações devem estar de acordo com as leis locais e internacionais relacionadas à engenharia reversa e uso de software.

## Objetivos

- **Desmontagem e Análise:** Radare2 para desmontar o firmware binário.
- **Estrutura do Firmware:** Identificar seções principais como bootloaders, kernel, sistema de arquivos raiz e áreas de configuração.
- **Coficação:** Reescrever o código binário em Código C.
- **Documentação:** Documentar o processo de engenharia reversa, as descobertas e qualquer componente extraído.


## Ferramentas e Técnicas Utilizadas

- **Ferramentas de Desmontagem (Disassembly):**
  - [Radare2](https://rada.re/)
  - [Binwalk](https://github.com/ReFirmLabs/binwalk) (para extração do firmware)


## Como Utilizar

1. **Extração do Firmware:**
   Use o `binwalk` para extrair o firmware do arquivo `.bin`:
   ```bash
   binwalk -e firmware.bin
## Licença

Este projeto utiliza o firmware do roteador TP-Link TL-WR1042ND V1, que é licenciado sob a licença **GPL (General Public License)**. Isso significa que você pode utilizar, modificar e distribuir o código sob os mesmos termos da GPL. O código-fonte correspondente está disponível no [GPL Code Center da TP-Link](https://www.tp-link.com/en/support/gpl-code/).

Este projeto está licenciado sob a **GPLv2** — consulte o arquivo LICENSE para mais detalhes.
