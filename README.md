# Daily Wallpapers
Esse repositório contém um script simples para alterar os papéis de parede do gnome de forma dinâmica e automatizada alternando de forma circular entre imagens de uma pasta


## Proposta
Diferente do Windows, o gnome não suprota a seleção de várias imagens para servirem como background da área de trabalho. Por isso, eu decidi criar um script que em algum grau possibilitasse isso.


## Abordagem
A filosofia por trás desse script é não ser inconveniente nem inconsistente. A abordagem proposta é a seguinte: o usuário escolhe uma pasta com imagens e o script alterna entre elas de forma cricular. Assim, manter a consistência nessa ocasião implica em possibilitar que o usuário não lide com inconveninetes como não poder mover arquivos que não sejam imagens ou criar diretórios na pasta em questão; outro ponto invariável é não permitir inconsitências como repitir uma imagem por o usuário ter adicionado ou apagado as demais imagens.
O funcionamento geral do código envolve a gestão do estado atual do wallpaper, a seleção do próximo wallpaper a partir de um diretório especificado pelo usuário, e a interação com as configurações do ambiente GNOME.
Ao ser executado, o script verifica se há um registro do caminho do wallpaper atual em um arquivo chamado dw.txt. Se não houver um caminho registrado, o script seleciona o primeiro arquivo de imagem válido no diretório fornecido pelo usuário. Esse caminho é então definido como wallpaper e registrado no arquivo dw.txt.
Se já houver um caminho registrado, o script procura o próximo arquivo de imagem válido no diretório, mantendo uma sequência cíclica. A ideia é proporcionar uma rotação contínua e automática dos papéis de parede, garantindo variedade visual na área de trabalho.
Para interagir com as configurações do ambiente GNOME, o script utiliza a ferramenta gsettings. Ele constrói comandos específicos para definir o plano de fundo nos modos claro e escuro, assegurando que a alteração seja consistente e reflita em ambos os modos.
A utilização da biblioteca libmagic é incorporada ao código para identificar arquivos de imagem no diretório. Isso é essencial para evitar que arquivos não relacionados a imagens sejam considerados como papéis de parede. Nesse ponto, utilliza-se a biblioteca magic, que é muito utilizada pelos sitemas UNIX para definir o tipo de arquivos; no projeto ela é utilizada pra ler o cabeçalho dos arquivos do diretório indicado pelo usuário, olhando o MIME-type em seu cabeçalho, o que irá apontar o tipo de conjuntos de dados que o arquivo representa; essa abordagem vai além da mera verificação da extensão do arquivo, técnica que envolve a manipulação de strings, que nem sempre reflete o tipo verdadeiro do arquivo.
O script também inclui medidas para feedback ao usuário. Em caso de problemas na leitura do caminho atual ou se não for possível acessar o caminho do wallpaper, são exibidas notificações utilizando o comando notify-send. 


## Usabilidade
Como usuário  você só precisa rodar o script install.sh, após isso será solicitado o caminho completo para o diretório com as imagens que você deseja que se alternem em seu plano de fundo. Após isso o script já estará funcionando.


## Limitações 
Devido à sua simplicidade, o programa possui algumas limitações. Qualquer alteração na imagem atual, no diretório indicado para o programa, pode causar alguma inconsitência na troca de imagens, porém, de forma alguma, vai interromper ou prejudicar o ciclo de troca de imagem, apenas pode reiniciar o ciclo de papéis de paredes do primeiro candidato no diretório apontado. De qualquer forma, o ciclo irá se reestabelecer e funcionar normalmente.
Existe apenas uma única limitação técnica que pode causar problemas na execução script, que são as variáveis do ambiente gráfico do run-level 5 do Linux. No código fonte do projeto elas se encontram no script run.sh. 
