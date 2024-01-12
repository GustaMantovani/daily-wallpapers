# Daily Wallpapers
Esse repositório contém um script simples para alterar os papéis de parede do gnome de forma dinâmica e automatizada alternando de forma circular entre imagens de uma pasta


## Proposta
Diferente do Windows, o gnome não suprota a seleção de várias imagens para servirem como background da área de trabalho. Por isso, eu decidi criar um script que, em algum grau, possibilitasse isso. É importante forncecer verdadeiramente o caminho completo (incluindo o '/' no fim; EX.: /home/gusta/Pictures/). Isso é uma limitação que daria trabalho demais para ser resolvida e o meu objetivo era desenvolver um projeto simples. 


## Abordagem
A filosofia por trás desse script é não ser inconveniente nem inconsistente. A abordagem proposta é a seguinte: o usuário escolhe uma pasta com imagens e o script alterna entre elas de forma cricular. Assim, manter a consistência nessa ocasião implica em possibilitar que o usuário não lide com inconveninetes como não poder mover arquivos que não sejam imagens ou criar diretórios na pasta em questão; outro ponto invariável é não permitir inconsitências como repitir uma imagem por o usuário ter adicionado ou apagado as demais imagens.


O funcionamento geral do código envolve a gestão do estado atual do wallpaper, a seleção do próximo wallpaper a partir de um diretório especificado pelo usuário, e a interação com as configurações do ambiente GNOME.


Ao ser executado, o script verifica se há um registro do caminho do wallpaper atual em um arquivo chamado dw.txt. Se não houver um caminho registrado, o script seleciona o primeiro arquivo de imagem válido no diretório fornecido pelo usuário. Esse caminho é então definido como wallpaper e registrado no arquivo dw.txt.


Se já houver um caminho registrado, o script procura o próximo arquivo de imagem válido no diretório, mantendo uma sequência cíclica. A ideia é proporcionar uma rotação contínua e automática dos papéis de parede, garantindo variedade visual na área de trabalho.
Para interagir com as configurações do ambiente GNOME, o script utiliza a ferramenta gsettings. Ele constrói comandos específicos para definir o plano de fundo nos modos claro e escuro, assegurando que a alteração seja consistente e reflita em ambos os modos.


A utilização da biblioteca libmagic é incorporada ao código para identificar arquivos de imagem no diretório. Isso é essencial para evitar que arquivos não relacionados a imagens sejam considerados como papéis de parede. Nesse ponto, utilliza-se a biblioteca magic, que é muito utilizada pelos sitemas UNIX para definir o tipo de arquivos; no projeto ela é utilizada pra ler o cabeçalho dos arquivos do diretório indicado pelo usuário, olhando o MIME-type em seu cabeçalho, o que irá apontar o tipo de conjuntos de dados que o arquivo representa; essa abordagem vai além da mera verificação da extensão do arquivo, técnica que envolve a manipulação de strings, que nem sempre reflete o tipo verdadeiro do arquivo.


O script também inclui medidas para feedback ao usuário. Em caso de problemas na leitura do caminho atual ou se não for possível acessar o caminho do wallpaper, são exibidas notificações utilizando o comando notify-send. 


A chamada para execução do programa ocorre por meio do script run.sh, que exporta as variáveis de ambiente gráfico necessárias para o run-level 3 e serve o caminho do diretório escolhido pelo usuário, informação que fica salva no arquivo dw.conf, na entrada padrão.


## Usabilidade
Como usuário  você só precisa rodar o script install.sh, após isso será solicitado o caminho completo para o diretório com as imagens que você deseja que se alternem em seu plano de fundo. Após isso o script já estará funcionando.


## Notas 
- Devido à sua simplicidade, o programa possui algumas limitações. Qualquer alteração na imagem atual, no diretório indicado para o programa, pode causar alguma inconsitência na troca de imagens, porém, de forma alguma, vai interromper ou prejudicar o ciclo de troca de imagem, apenas pode reiniciar o ciclo de papéis de paredes do primeiro candidato no diretório apontado. De qualquer forma, o ciclo irá se reestabelecer e funcionar normalmente.
- Há, contudo, uma única limitação técnica que pode impactar a execução do script, relacionada às variáveis do ambiente gráfico no run-level 5 do Linux, conforme mencionado no script run.sh do código-fonte do projeto.
Para testar essa limitação, você pode executar o comando "anacron -fn -S /tmp wallpaper.daily". Caso o papel de parede não seja alterado imediatamente, isso provavelmente está relacionado ao erro mencionado. A essência desse problema reside na maneira como o serviço cronie executa as tarefas agendadas, assim como os trabalhos do anacron, pois isso ocorre no run-level 3, onde as variáveis do ambiente gráfico não são carregadas.
Para solucionar esse problema, importamos essas variáveis do ambiente gráfico no script run.sh. Apesar de não serem carregadas no nível em questão, elas ainda são acessíveis. No entanto, observe que o caminho dessas variáveis pode variar entre diferentes sistemas. Se você estiver enfrentando problemas, pode verificar o valor dessas variáveis em seu sistema através de um terminal. Substitua os valores no arquivo ~/.dw/run.sh conforme necessário. Para isso, execute o comando "echo" seguido do nome de cada variável em questão, copie a saída e faça as alterações no arquivo mencionado (EX.: echo $DISPLAY); caso queira aprender mais sobre run-levels eu recomendo o vídeo do Fábio Akita onde ele explica como funciona o boot de um Linux e aborda o assunto em questão: https://youtu.be/5F6BbhgvFOE?si=6Q_Q0KH5xH7ki4DK.
- O projeto apresenta apenas duas opções de esquemas de horários para execução do script. Isso ocorre porque é complicado manipular arquivos de configuração de agenda de tarefas no Linux por se tratarem de arquivos de configuração sensíveis em muitos sistemas. Ainda sim, você pode personalizar esses esquemas utilizando das mesmas ferramentas que o projeto usa. Por exemplo, caso queria alterar a mudança de wallpapers em função de um tempo t em horas, dias, ou semanas é possível editar o arquivo /etc/crontab da forma que você preferir. Para aprender mais sobre o cron leia o manual da ferramenta em seu Linux (cron(8)). 
- Caso queira mudar as configurações de forma que o script seja executado levando em conta que o computador não ficará ligado o tempo todo, você pode editar a linha do arquivo /etc/anacrontab com o identificador wallpaper.daily; para aprender mais sobre o anacron leia o manual da ferramento em seu Linux (anacron(8)).
- Problemas de permissões podem ocorrer com o diretório onde os timstamps do anacron são armazenados, então cheque se seu usuário tem permissão para ler, escrever e executar o diretório /var/spool/anacron. Caso contário, altere as permissões com o comando "sudo chmod o=rwx /var/spool/anacron/". Caso queira entender mais sobre permissões e diretórios eu recomendo a apostila inciante do projeto Guia Foca Linux: https://www.guiafoca.org/.
