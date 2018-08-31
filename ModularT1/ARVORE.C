/***************************************************************************
*  $MCI Módulo de implementação: Módulo árvore
*
*  Arquivo gerado:              ARVORE.C
*  Letras identificadoras:      ARV
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
***************************************************************************/

#include   <malloc.h>
#include   <stdio.h>

#define ARVORE_OWN
#include "ARVORE.H"
#undef ARVORE_OWN

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor do nó da árvore
*
*
*  $ED Descrição do tipo
*     Descreve a organização do nó
*
***********************************************************************/

   typedef struct tgNoArvore {

         struct tgNoArvore * pNoPai ;
               /* Ponteiro para pai
               *
               *$EED Assertivas estruturais
               *   É NULL sse o nó é raiz
               *   Se não for raiz, um de pNoEsq ou pNoDir de pNoPai do nó
               *   corrente apontam para o nó corrente */

         struct tgNoArvore * pNoEsq ;
               /* Ponteiro para filho à esquerda
               *
               *$EED Assertivas estruturais
               *   se pNoEsq do nó X != NULL então pNoPai de pNoEsq aponta para o nó X */

         struct tgNoArvore * pNoDir ;
               /* Ponteiro para filho à direita
               *
               *$EED Assertivas estruturais
               *   se pNoDir do nó X != NULL então pNoPai de pNoDir aponta para o nó X */

         char Valor ;
               /* Valor do nó */

	   ARV_tpModoVisita ModoVisita;
	   // Modo de visita do no

   } tpNoArvore ;

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor da cabeça de uma árvore
*
*
*  $ED Descrição do tipo
*     A cabe‡a da árvore é o ponto de acesso para uma determinada árvore.
*     Por intermédio da referência para o nó corrente e do ponteiro
*     pai pode-se navegar a árvore sem necessitar de uma pilha.
*     Pode-se, inclusive, operar com a árvore em forma de co-rotina.
*
***********************************************************************/

   struct tgArvore {

         tpNoArvore * pNoRaiz ;
               /* Ponteiro para a raiz da árvore */

         tpNoArvore * pNoCorr ;
               /* Ponteiro para o nó corrente da árvore */

   };
   typedef struct tgArvore* ARV_tpArvore;

/*****  Dados encapsulados no módulo  *****/

/***** Protótipos das funções encapuladas no módulo *****/

   static tpNoArvore * CriarNo(char ValorParm ) ;

   static ARV_tpCondRet CriarNoRaiz(ARV_tpArvore pArvore, char ValorParm ) ;

   static void DestroiArvore(tpNoArvore * pNo) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: ARV Criar árvore
*  ****/

   ARV_tpCondRet ARV_CriarArvore(void** arvore){
	   
	ARV_tpArvore pArvore;
	//se nao houver aonde criar, a arvore esta criada por vacuidade.
	if(!arvore) return ARV_CondRetOK;
      if (*arvore) ARV_DestruirArvore(arvore);

	pArvore = ( ARV_tpArvore ) malloc( sizeof(struct tgArvore)) ;
      if (!pArvore) return ARV_CondRetFaltouMemoria;

      pArvore->pNoRaiz = NULL ;
      pArvore->pNoCorr = NULL ;

	*arvore = pArvore;

      return ARV_CondRetOK ;

   } /* Fim função: ARV Criar árvore */

/***************************************************************************
*
*  Função: ARV Destruir árvore
*  ****/

   void ARV_DestruirArvore(void** arvore){
	ARV_tpArvore pArvore;
	if(!arvore) return;
	pArvore = (ARV_tpArvore) (*arvore);
	if (pArvore){
         if (pArvore->pNoRaiz) DestroiArvore(pArvore->pNoRaiz);
         free(pArvore);
         *arvore = NULL;
      } /* if */

   } /* Fim função: ARV Destruir árvore */
	

/***************************************************************************
*
*  Função: ARV Adicionar filho à esquerda
*  ****/

   ARV_tpCondRet ARV_InserirEsquerda(void* arvore, char ValorParm){

      ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
	tpNoArvore* pNo;
	tpNoArvore* pCorr;

      /* Tratar vazio, esquerda */
	if(!pArvore) return ARV_CondRetArvoreNaoExiste;
	if (!pArvore->pNoRaiz) return CriarNoRaiz(pArvore, ValorParm);

      /* Criar nó à esquerda de folha */
	pCorr = pArvore->pNoCorr;
	if (!pCorr) return ARV_CondRetErroEstrutura;

	if (!pCorr->pNoEsq) {
            pNo = CriarNo(ValorParm) ;
            if (!pNo) return ARV_CondRetFaltouMemoria;

		pNo->ModoVisita = ARV_ModoParaEsq;
		pNo->pNoPai	= pCorr;
		pCorr->pNoEsq = pNo ;
            pArvore->pNoCorr = pNo ;

            return ARV_CondRetOK ;
         } /* if */

      /* Tratar não folha à esquerda */

         return ARV_CondRetNaoEhFolha ;

   } /* Fim função: ARV Adicionar filho à esquerda */

/***************************************************************************
*
*  Função: ARV Adicionar filho à direita
*  ****/

   ARV_tpCondRet ARV_InserirDireita(void* arvore, char ValorParm){

      ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
      tpNoArvore * pCorr ;
      tpNoArvore * pNo ;

      /* Tratar vazio, direita */
	if(!pArvore) return ARV_CondRetArvoreNaoExiste;
	if (!pArvore->pNoRaiz) return CriarNoRaiz(pArvore, ValorParm);

      /* Criar nó à direita de folha */
	pCorr = pArvore->pNoCorr;
	if (!pCorr) return ARV_CondRetErroEstrutura;

	if (!pCorr->pNoDir) {
            pNo = CriarNo(ValorParm) ;
            if (!pNo) return ARV_CondRetFaltouMemoria;

		pNo->ModoVisita = ARV_ModoParaDir;
		pNo->pNoPai	= pCorr;
		pCorr->pNoDir = pNo ;
            pArvore->pNoCorr = pNo ;

            return ARV_CondRetOK ;
         } /* if */

      /* Tratar não folha à direita */

         return ARV_CondRetNaoEhFolha ;

   } /* Fim função: ARV Adicionar filho à direita */

/***************************************************************************
*
*  Função: ARV Ir para nó pai
*  ****/

   ARV_tpCondRet ARV_IrPai(void* arvore){
	ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
      if (!pArvore) return ARV_CondRetArvoreNaoExiste;
	if (!pArvore->pNoRaiz) return ARV_CondRetArvoreVazia;
	if (!pArvore->pNoCorr) return ARV_CondRetErroEstrutura;
      if (!pArvore->pNoCorr->pNoPai)return ARV_CondRetNohEhRaiz;
        
	pArvore->pNoCorr = pArvore->pNoCorr->pNoPai;
	pArvore->pNoCorr->ModoVisita = ARV_ModoDePai;
	return ARV_CondRetOK;

   } /* Fim função: ARV Ir para nó pai */

/***************************************************************************
*
*  Função: ARV Ir para nó à esquerda
*  ****/

   ARV_tpCondRet ARV_IrNoEsquerda(void* arvore){
	ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
      if (!pArvore) return ARV_CondRetArvoreNaoExiste;
	if (!pArvore->pNoRaiz) return ARV_CondRetArvoreVazia;
	if (!pArvore->pNoCorr) return ARV_CondRetErroEstrutura;
      if (!pArvore->pNoCorr->pNoEsq) return ARV_CondRetNaoPossuiFilho;

      pArvore->pNoCorr = pArvore->pNoCorr->pNoEsq;
	pArvore->pNoCorr->ModoVisita = ARV_ModoParaEsq;
      return ARV_CondRetOK;

   } /* Fim função: ARV Ir para nó à esquerda */

/***************************************************************************
*
*  Função: ARV Ir para nó à direita
*  ****/

   ARV_tpCondRet ARV_IrNoDireita(void* arvore){
	ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
      if (!pArvore) return ARV_CondRetArvoreNaoExiste;
	if (!pArvore->pNoRaiz) return ARV_CondRetArvoreVazia;
	if (!pArvore->pNoCorr) return ARV_CondRetErroEstrutura;
      if (!pArvore->pNoCorr->pNoDir) return ARV_CondRetNaoPossuiFilho;

      pArvore->pNoCorr = pArvore->pNoCorr->pNoDir;
	pArvore->pNoCorr->ModoVisita = ARV_ModoParaDir;
      return ARV_CondRetOK;
   } /* Fim função: ARV Ir para nó à direita */

/***************************************************************************
*
*  Função: ARV Obter valor corrente
*  ****/

   ARV_tpCondRet ARV_ObterValorCorr(void* arvore, char * ValorParm ){
	ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
      if (!pArvore) return ARV_CondRetArvoreNaoExiste;
	if (!pArvore->pNoRaiz) return ARV_CondRetArvoreVazia;
	if (!pArvore->pNoCorr) return ARV_CondRetErroEstrutura;
      * ValorParm = pArvore->pNoCorr->Valor;
      return ARV_CondRetOK ;
   } /* Fim função: ARV Obter valor corrente */


/***************************************************************************
*
*  Função: ARV MarcarVisitado
*  ****/   

   ARV_tpCondRet MarcarVisitado(void* arvore, ARV_tpModoVisita Modo){
	ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
      if (!pArvore) return ARV_CondRetArvoreNaoExiste;
	if(!pArvore->pNoRaiz) return ARV_CondRetArvoreVazia;
	if (!pArvore->pNoCorr) return ARV_CondRetErroEstrutura;
	pArvore->pNoCorr->ModoVisita = Modo;
	return ARV_CondRetOK;
   }


/***************************************************************************
*
*  Função: ARV Exibe arvore.
*  
*  Notas:
*  Assumimos aqui que o no corrente da arvore comeca no no raiz.
*  Tambem entendemos que ordem prefixada pela esquerda = primeiro no corrente, depois esquerda, depois direita.
*  Note que foi necessario utilizar para ela um argumento void* para que fique consistente com
*  o resto da implementacao das funcoes de acesso.
*  ****/

   void ARV_ExibirArvore(void* arvore){
	ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
	tpNoArvore* pCorr;
	if(!pArvore) return;
	if(!pArvore->pNoRaiz || !pArvore->pNoCorr) return;
	pCorr = pArvore->pNoCorr;
	   
	if(pCorr->ModoVisita == ARV_ModoParaDir) printf("%c ", pCorr->Valor);
	else printf("( %c ", pCorr->Valor);
	if(!pCorr->pNoDir) printf(")\n");

	if(pCorr->pNoEsq){
		pArvore->pNoCorr = pCorr->pNoEsq;
		pArvore->pNoCorr->ModoVisita = ARV_ModoParaEsq;
		ARV_ExibirArvore(arvore);
	} 
	
	if(pCorr->pNoDir){
		pArvore->pNoCorr = pCorr->pNoDir;
		pArvore->pNoCorr->ModoVisita = ARV_ModoParaDir;
		ARV_ExibirArvore(arvore);
	}
   }


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: ARV Criar nó da árvore
*
*  $FV Valor retornado
*     Ponteiro para o nó criado.
*     Será NULL caso a memória tenha se esgotado.
*     Os ponteiros do nó criado estarão nulos e o valor é igual ao do
*     parâmetro.
*
***********************************************************************/

   tpNoArvore * CriarNo(char ValorParm){

      tpNoArvore * pNo = (tpNoArvore *) malloc(sizeof( tpNoArvore )) ;
      if (!pNo) return NULL;

      pNo->pNoPai = NULL ;
      pNo->pNoEsq = NULL ;
      pNo->pNoDir = NULL ;
	pNo->ModoVisita = ARV_ModoPadrao;
      pNo->Valor  = ValorParm ;
      return pNo ;

   } /* Fim função: ARV Criar nó da árvore */


/***********************************************************************
*
*  $FC Função: ARV Criar nó raiz da árvore
*
*  $FV Valor retornado
*     ARV_CondRetOK
*     ARV_CondRetFaltouMemoria
*     ARV_CondRetNaoCriouRaiz
*	ARV_CondRetArvoreNaoExiste
*
***********************************************************************/

   ARV_tpCondRet CriarNoRaiz(ARV_tpArvore pArvore, char ValorParm){

      tpNoArvore * pNo;

      if (!pArvore) return ARV_CondRetArvoreNaoExiste;

      if (!pArvore->pNoRaiz){
         pNo = CriarNo(ValorParm);
         if (!pNo) return ARV_CondRetFaltouMemoria;
         pArvore->pNoRaiz = pNo;
         pArvore->pNoCorr = pNo;

         return ARV_CondRetOK ;
      } /* if */

      return ARV_CondRetNaoCriouRaiz ;

   } /* Fim função: ARV Criar nó raiz da árvore */


/***********************************************************************
*
*  $FC Função: ARV Destruir a estrutura da árvore
*
*
***********************************************************************/

   void DestroiArvore(tpNoArvore * pNo){
	if(!pNo) return;
      DestroiArvore(pNo->pNoEsq);
      DestroiArvore(pNo->pNoDir);
      free(pNo);
   } /* Fim função: ARV Destruir a estrutura da árvore */

/********** Fim do módulo de implementação: Módulo árvore **********/

