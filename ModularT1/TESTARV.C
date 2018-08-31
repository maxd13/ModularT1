/***************************************************************************
*  $MCI Módulo de implementação: Módulo de teste específico
*
*  Arquivo gerado:              TESTARV.C
*  Letras identificadoras:      TARV
*
*  Nome da base de software:    Exemplo de teste automatizado
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\SIMPLES.BSW
*
*  Projeto: Disciplinas INF 1628 / 1301
*  Gestor:  DI/PUC-Rio
*  Autores: avs - Arndt von Staa
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*       3.00   avs   28/02/2003 Uniformização da interface das funções e
*                               de todas as condições de retorno.
*       2.00   avs   03/08/2002 Eliminação de código duplicado, reestruturação
*       1.00   avs   15/08/2001 Início do desenvolvimento
*
*  $ED Descrição do módulo
*     Este mÇodulo contém as funções específicas para o teste do
*     módulo árvore. Ilustra como redigir um interpretador de comandos
*     de teste específicos utilizando o arcabouço de teste para C.
*
*  $EIU Interface com o usuário pessoa
*     Comandos de teste específicos para testar o módulo árvore:
*
*     =criar        - chama a função ARV_CriarArvore( )
*     =insdir <Char>
*                   - chama a função ARV_InserirDireita( <Char> )
*                     Obs. notação: <Char>  é o valor do parâmetro
*                     que se encontra no comando de teste.
*
*     "=insesq" <Char>
*                   - chama a função ARV_InserirEsquerda( <Char> )
*     "=irpai"      - chama a função ARV_IrPai( )
*     "=iresq"      - chama a função ARV_IrEsquerda( )
*     "=irdir"      - chama a função ARV_IrDireita( )
*     "=obter" <Char>
*                   - chama a função ARV_ObterValorCorr( ) e compara
*                     o valor retornado com o valor <Char>
*     "=destroi"    - chama a função ARV_DestruirArvore( )
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>

#include    "TST_ESPC.H"

#include    "generico.h"
#include    "lerparm.h"

#include    "arvore.h"

/* Tabela dos nomes dos comandos de teste específicos */
#define	START_CMD		  "=iniciar"
#define	RESET_CMD		  "=resetar"
#define	PRINT_CMD		  "=exibir"
#define	MARK_CMD		  "=marcar"
#define     CRIAR_ARV_CMD       "=criar"
#define     INS_DIR_CMD         "=insdir"
#define     INS_ESQ_CMD         "=insesq"
#define     IR_PAI_CMD          "=irpai"
#define     IR_ESQ_CMD          "=iresq"
#define     IR_DIR_CMD          "=irdir"
#define     OBTER_VAL_CMD       "=obter"
#define     DESTROI_CMD         "=destruir"

#define NUM_ARV 10

void* arvores[NUM_ARV];

/***** Prototipos das funcoes encapuladas no modulo *****/

   static int VerificarInx( int inxArvore ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TARV Efetuar operações de teste específicas para árvore
*
*  $ED Descrição da função
*     Efetua os diversos comandos de teste específicos para o módulo
*     árvore sendo testado.
*
*  $EP Parâmetros
*     $P ComandoTeste - String contendo o comando
*
*  $FV Valor retornado
*     Ver TST_tpCondRet definido em TST_ESPC.H
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      ARV_tpCondRet CondRetObtido   = ARV_CondRetOK ;
      ARV_tpCondRet CondRetEsperada = ARV_CondRetFaltouMemoria ;
                                      /* inicializa para qualquer coisa */

      char ValorEsperado = '?'  ;
      char ValorObtido   = '!'  ;
      char ValorDado     = '\0' ;
	int modo = 0;

      int  NumLidos = -1 ;
	int  inxArvore = -1 ;

      TST_tpCondRet Ret ;

	int i;
	
	//Inicializar o contexto.
	if(strcmp(ComandoTeste, START_CMD) == 0){
		for(i = 0; i < NUM_ARV; i++) arvores[i] = NULL;
		return TST_CondRetOK;
	}
	//Reinicializar o contexto (tambem serve para destrui-lo).
	else if(strcmp(ComandoTeste, RESET_CMD) == 0){
		for(i = 0; i < NUM_ARV; i++){
			if(arvores[i]) ARV_DestruirArvore(arvores + i);
			arvores[i] = NULL;
		}
		return TST_CondRetOK;
	}
      /* Testar ARV Criar árvore */

         else if ( strcmp( ComandoTeste , CRIAR_ARV_CMD ) == 0 ){
            NumLidos = LER_LerParametros( "ii" , &inxArvore, &CondRetEsperada );

            if ( NumLidos != 2 || !VerificarInx(inxArvore)) return TST_CondRetParm;

            CondRetObtido = ARV_CriarArvore(arvores + inxArvore);

            return TST_CompararInt( CondRetEsperada , CondRetObtido ,
                                    "Retorno errado ao criar árvore." );

         } /* fim ativa: Testar ARV Criar árvore */

      /* Testar ARV Adicionar filho à direita */

         else if ( strcmp( ComandoTeste , INS_DIR_CMD ) == 0 ){

            NumLidos = LER_LerParametros( "ici" , &inxArvore, &ValorDado , &CondRetEsperada);

            if ( NumLidos != 3 || !VerificarInx(inxArvore)) return TST_CondRetParm;

		CondRetObtido = ARV_InserirDireita(arvores[inxArvore], ValorDado);

            return TST_CompararInt( CondRetEsperada , CondRetObtido ,
                                    "Retorno errado inserir à direita." );

         } /* fim ativa: Testar ARV Adicionar filho à direita */

      /* Testar ARV Adicionar filho à esquerda */

         else if ( strcmp( ComandoTeste , INS_ESQ_CMD ) == 0 ){

            NumLidos = LER_LerParametros( "ici" , &inxArvore, &ValorDado , &CondRetEsperada);

            if ( NumLidos != 3 || !VerificarInx(inxArvore)) return TST_CondRetParm;

            CondRetObtido = ARV_InserirEsquerda(arvores[inxArvore], ValorDado);

            return TST_CompararInt( CondRetEsperada , CondRetObtido ,
                                    "Retorno errado ao inserir à esquerda." );

         } /* fim ativa: Testar ARV Adicionar filho à esquerda */

      /* Testar ARV Ir para nó pai */

         else if ( strcmp( ComandoTeste , IR_PAI_CMD ) == 0 ){

            NumLidos = LER_LerParametros( "ii" , &inxArvore, &CondRetEsperada);

		if ( NumLidos != 2 || !VerificarInx(inxArvore)) return TST_CondRetParm;

            CondRetObtido = ARV_IrPai(arvores[inxArvore]);

            return TST_CompararInt( CondRetEsperada , CondRetObtido ,
                                    "Retorno errado ao ir para pai." );

         } /* fim ativa: Testar ARV Ir para nó pai */

      /* Testar ARV Ir para nó à esquerda */

         else if ( strcmp( ComandoTeste , IR_ESQ_CMD ) == 0 ){

            NumLidos = LER_LerParametros( "ii", &inxArvore, &CondRetEsperada);

            if ( NumLidos != 2 || !VerificarInx(inxArvore)) return TST_CondRetParm;

            CondRetObtido = ARV_IrNoEsquerda(arvores[inxArvore]);

            return TST_CompararInt( CondRetEsperada , CondRetObtido ,
                                    "Retorno errado ao ir para esquerda." );

         } /* fim ativa: Testar ARV Ir para nó à esquerda */

      /* Testar ARV Ir para nó à direita */

         else if ( strcmp( ComandoTeste , IR_DIR_CMD ) == 0 ){

            NumLidos = LER_LerParametros( "ii", &inxArvore, &CondRetEsperada);

		if ( NumLidos != 2 || !VerificarInx(inxArvore)) return TST_CondRetParm;

            CondRetObtido = ARV_IrNoDireita(arvores[inxArvore]);

            return TST_CompararInt( CondRetEsperada , CondRetObtido ,
                                    "Retorno errado ao ir para direita." );

         } /* fim ativa: Testar ARV Ir para nó à direita */

      /* Testar ARV Obter valor corrente */

         else if ( strcmp( ComandoTeste , OBTER_VAL_CMD ) == 0 ){

            NumLidos = LER_LerParametros( "ici", &inxArvore, &ValorEsperado, &CondRetEsperada);

		if ( NumLidos != 3 || !VerificarInx(inxArvore)) return TST_CondRetParm;

            CondRetObtido = ARV_ObterValorCorr(arvores[inxArvore], &ValorObtido);

            Ret = TST_CompararInt( CondRetEsperada , CondRetObtido ,
                                   "Retorno errado ao obter valor corrente." );

            if ( Ret != TST_CondRetOK ) return Ret;

            return TST_CompararChar( ValorEsperado , ValorObtido ,
                                     "Conteúdo do nó está errado." ) ;

         } /* fim ativa: Testar ARV Obter valor corrente */

      /* Testar ARV Destruir árvore */

         else if ( strcmp( ComandoTeste , DESTROI_CMD ) == 0 ){

		NumLidos = LER_LerParametros( "i", &inxArvore);

		if ( NumLidos != 1 || !VerificarInx(inxArvore)) return TST_CondRetParm;

            ARV_DestruirArvore(arvores+inxArvore);

            return TST_CondRetOK ;

         } /* fim ativa: Testar ARV Destruir árvore */

	   //Exibir arvore
	   else if ( strcmp( ComandoTeste , PRINT_CMD) == 0 ){

		NumLidos = LER_LerParametros( "i", &inxArvore);

		if ( NumLidos != 1 || !VerificarInx(inxArvore)) return TST_CondRetParm;

            ARV_ExibirArvore(arvores+inxArvore);

            return TST_CondRetOK ;

         }// fim ativa: Exibir Arvore

	   /* Testar ARV Marcar Visitado */

	   else if ( strcmp( ComandoTeste , MARK_CMD) == 0 ){

            NumLidos = LER_LerParametros( "ici" , &inxArvore, &modo, &CondRetEsperada);

            if ( NumLidos != 3 || !VerificarInx(inxArvore)) return TST_CondRetParm;

		CondRetObtido = MarcarVisitado(arvores[inxArvore], (ARV_tpModoVisita)modo);

            return TST_CompararInt( CondRetEsperada , CondRetObtido ,
                                    "Retorno errado ao marcar nó." );

         } /* fim ativa: Testar ARV Marcar Visitado*/

      return TST_CondRetNaoConhec ;

   } /* Fim função: TARV Efetuar operações de teste específicas para árvore */


/***********************************************************************
*
*  $FC Funcao: TARV -Verificar indice de arvore
*
*  $FV Valor retornado
*     0 - inxArvore nao vale
*     1 - inxArvore vale
*
***********************************************************************/

   int VerificarInx( int inxArvore ){

      if ( ( inxArvore <   0 )
        || ( inxArvore >= NUM_ARV))
      {
         return 0 ;
      } /* if */

      return 1 ;

   }


/********** Fim do módulo de implementação: Módulo de teste específico **********/

