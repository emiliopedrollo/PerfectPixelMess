# PerfectPixelMess ![travis-ci](https://travis-ci.org/emiliopedrollo/PerfectPixelMess.svg?branch=master)

PerfectPixelMess é um projeto simples de manipulação de imagem em formato PPM desenvolvido como trabalho acadêmico por [Emílio B. Pedrollo] para a cadeira de Laboratório de Programação II pela UFSM.

[Emílio B. Pedrollo]: <https://github.com/emiliopedrollo>

## Obtenção do código fonte
O código-fonte está disponível publicamente online no endereço https://github.com/emiliopedrollo/PerfectPixelMess/ e é possível efetuar o download do fonte compactado em *.zip* diretamente no site ou através do gerenciador de versões *git* com a seguinte linha de comando:
```sh
git clone https://github.com/emiliopedrollo/PerfectPixelMess.git
```


## Compilando
Apesar de ser possível compilar o código fonte através do compilador padão *gcc* para *unix* é muito mais fácil utilizar o pacote *cmake* para automatizar o processo:

### Utilizando cmake
Caso não tenha o *cmake* instalado é possivel instalá-lo com a seguinte linha de comando como super usuário em distros baseadas no Debian (como o Ubuntu):
```sh
apt-get install -y cmake 
```
Para compilar basta ir até a pasta contendo o fonte e executar
```sh
cmake . && make
```
O compilador é esta instruído a utilizar o padrão C *ansi* para este projeto. Após a compilação o executável **ppm** estará na mesma pasta com os códigos-fonte.


### Utilizando gcc
Caso você não tenha o *cmake* instalado e não possua permissão administrativas para instala-lo ou simplismente queira utilizar um método mais pimitivo é possível utilizar o compilador padrão *gcc* com a seguinte linha:
```sh
gcc -o ppm -std=c11 -ansi main.c utils.c utils.h lists.c lists.h image.c image.h conv_mat.c conv_mat.h filter.c filter.h -lm
```

## Executando
O programa adota ideologias frequentemente observadas em outras aplicação unix como a entrada e saída de dados através da entrada e saída padrão respectivamente e o padrão com como se são definidos parametros de execução.

Para uma explicação melhor do funcionamento e de quais são os paramatros de execução pode-se rodar o compilado com o parametro ```-h``` ou ```--help``` para exibir a habitual tela de isntruções encontrada em praticamente todo executável para *unix*.

```
user@machine:~/ppm$ ./ppm -h
Usage: ppm [options]...
Apply filters to ppm images

Mandatory arguments to long options are mandatory for short options too.
  -i --input=FILE              Use FILE as input stream. Otherwise ppm uses stdin.
  -o --output=FILE             Outputs resulted image to FILE. Otherwise prints to stdout.
  -f --filter=F1,F2=1;7        Sets the filter to use on the input image. Where F1 represent 
                                 one of the filters listed bellow, F2=1 represent another with 1
                                 as an argument. Filters must be separated by commas. Arguments, 
                                 if more than one must be separated by semicolons.
  -c --custom=S,M=1;1;1...[,D] Runs a custom user defined convolution matrix where S is the matrix
                                 size (must be an odd number) and M defines the matrix values
                                 separated by semicolons. Optionally can be defined a divider to be
                                 applied to the sum.


The available filters are:
  INVERT       Inverts the image
  RGB=R[;G;B]  Color adjustment needs 1 or 3 integers ranging from -255 to 255 as parameter
  GREYSCALE    Converts a colored image to greyscale
  ROTATE=angle Rotates the image, acceptable angles are 90, 180, 270 and 360
  FLIP         Inverts the image upside down
  MIRROR       Inverts the image sideways
  BLUR[=S]     Blurs the image using a S size pixel matrix as sample
  BORDER       Border detection
  SHARP        Sharpens the image
```
## Filtros implementados
Conforme solicitado na descrição do trabalho foram implementados filtros de cor, posicionais e de matriz de convolução:

### Filtros de cor

#### INVERT
Simples filtro de inversão de cores. funciona sobrescrevendo o brilho de cada pixel individual pela subtração pelo brilho atual do brilho máximo definido para a imagem (para cada uma das 3 cores).

#### RGB
Recebe 1 ou 3 parametros. Se um parametro for informado será aplicado este diferencial de brilho para todas as cores de cada pixel da imagem. Se 3 parametros forem informados o primeiro parametro será aplicado ao brilho da cor vermelha, o segundo ao brilho da cor verde e o terceiro ao brilho da cor azul. Se a imagem possuir um brilho máximo diferente de 255 o brilho será aplicado proporcinalmente ao brilho máximo gerando resultados similares independente do brilho máximo da imagem fornecida.

#### GREYSCALE
Transforma uma imagem colorida em uma imagem em escala de cinza. Para realizar a média foi utilizada em consideração o valor de luminância e aplicada a fórmula ```0.299R + 0.587G + 0.114B``` para gerar o tom de cinza para cada pixel da imagem.

### Filtros de posicionais

#### ROTATE
Rotaciona a imagem. É necessário informar um ângulo para a rotação. Os valores aceitaveis são 0, 90, 180, 270 e 360. Funciona simplesmente transpondo a matriz de pixels para uma nova disposição.

#### FLIP
Transpõe verticalmente os pixels de uma imagem.

#### MIRROR
Transpõe horizontalmente os pixels de uma imagem.

### Filtros com matriz de convolução

#### BLUR
Gera um efeito de desfoque na imagem. Opcionalmente é possível informar um parâmetro para a quantidade de pixels de amostra *(lado da matriz = tamanho da amostra * 2 + 1)* que será utilizado no processamento.

#### BORDER
Realçar (Detecta) as bordas da imagem. Utiliza uma matriz disponibilizada publicamente [aqui](https://docs.gimp.org/2.8/pt_BR/plug-in-convmatrix.html).

#### SHARP
Utiliza um filtro de realce linear Laplaciano disponibilizado em um [pdf educativo](https://drive.google.com/file/d/0B5kbZ7fKpj4UdVdWckU3WG9sU0tkaGVwUGxyM0hyTGxpN2tJ/view).

### Matriz de convolução costumizada
Ainda é possível que o utilizador da aplicação defina uma matriz para ser aplicada atrvés do parâmetro ```-c``` ou ```--custom```. Por exemplo o parametro ```-c 3,M=1;1;1;1;1;1;1;1;1,9``` irá criar uma matrix *3x3* com cada membro da matriz com valor *1*. O valor é opcional e define valor pelo qual o resultado da soma será dividio, neste caso o somatório será dividido por *9*.

