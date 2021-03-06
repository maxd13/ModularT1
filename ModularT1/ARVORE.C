/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo �rvore
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
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*       3.00   avs   28/02/2003 Uniformiza��o da interface das fun��es e
*                               de todas as condi��es de retorno.
*       2.00   avs   03/08/2002 Elimina��o de c�digo duplicado, reestrutura��o
*       1.00   avs   15/08/2001 In�cio do desenvolvimento
*
***************************************************************************/

#include   <malloc.h>
#include   <stdio.h>

#define ARVORE_OWN
#include "ARVORE.H"
#undef ARVORE_OWN

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor do n� da �rvore
*
*
*  $ED Descri��o do tipo
*     Descreve a organiza��o do n�
*
***********************************************************************/

   typedef struct tgNoArvore {

         struct tgNoArvore * pNoPai ;
               /* Ponteiro para pai
               *
               *$EED Assertivas estruturais
               *   � NULL sse o n� � raiz
               *   Se n�o for raiz, um de pNoEsq ou pNoDir de pNoPai do n�
               *   corrente apontam para o n� corrente */

         struct tgNoArvore * pNoEsq ;
               /* Ponteiro para filho � esquerda
               *
               *$EED Assertivas estruturais
               *   se pNoEsq do n� X != NULL ent�o pNoPai de pNoEsq aponta para o n� X */

         struct tgNoArvore * pNoDir ;
               /* Ponteiro para filho � direita
               *
               *$EED Assertivas estruturais
               *   se pNoDir do n� X != NULL ent�o pNoPai de pNoDir aponta para o n� X */

         char Valor ;
               /* Valor do n� */

	   ARV_tpModoVisita ModoVisita;
	   // Modo de visita do no

   } tpNoArvore ;

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor da cabe�a de uma �rvore
*
*
*  $ED Descri��o do tipo
*     A cabe�a da �rvore � o ponto de acesso para uma determinada �rvore.
*     Por interm�dio da refer�ncia para o n� corrente e do ponteiro
*     pai pode-se navegar a �rvore sem necessitar de uma pilha.
*     Pode-se, inclusive, operar com a �rvore em forma de co-rotina.
*
***********************************************************************/

   struct tgArvore {

         tpNoArvore * pNoRaiz ;
               /* Ponteiro para a raiz da �rvore */

         tpNoArvore * pNoCorr ;
               /* Ponteiro para o n� corrente da �rvore */

   };
   typedef struct tgArvore* ARV_tpArvore;

/*****  Dados encapsulados no m�dulo  *****/

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static tpNoArvore * CriarNo(char ValorParm ) ;

   static ARV_tpCondRet CriarNoRaiz(ARV_tpArvore pArvore, char ValorParm ) ;

   static void DestroiArvore(tpNoArvore * pNo) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: ARV Criar �rvore
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

   } /* Fim fun��o: ARV Criar �rvore */

/***************************************************************************
*
*  Fun��o: ARV Destruir �rvore
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

   } /* Fim fun��o: ARV Destruir �rvore */
	

/***************************************************************************
*
*  Fun��o: ARV Adicionar filho � esquerda
*  ****/

   ARV_tpCondRet ARV_InserirEsquerda(void* arvore, char ValorParm){

      ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
	tpNoArvore* pNo;
	tpNoArvore* pCorr;

      /* Tratar vazio, esquerda */
	if(!pArvore) return ARV_CondRetArvoreNaoExiste;
	if (!pArvore->pNoRaiz) return CriarNoRaiz(pArvore, ValorParm);

      /* Criar n� � esquerda de folha */
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

      /* Tratar n�o folha � esquerda */

         return ARV_CondRetNaoEhFolha ;

   } /* Fim fun��o: ARV Adicionar filho � esquerda */

/***************************************************************************
*
*  Fun��o: ARV Adicionar filho � direita
*  ****/

   ARV_tpCondRet ARV_InserirDireita(void* arvore, char ValorParm){

      ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
      tpNoArvore * pCorr ;
      tpNoArvore * pNo ;

      /* Tratar vazio, direita */
	if(!pArvore) return ARV_CondRetArvoreNaoExiste;
	if (!pArvore->pNoRaiz) return CriarNoRaiz(pArvore, ValorParm);

      /* Criar n� � direita de folha */
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

      /* Tratar n�o folha � direita */

         return ARV_CondRetNaoEhFolha ;

   } /* Fim fun��o: ARV Adicionar filho � direita */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� pai
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

   } /* Fim fun��o: ARV Ir para n� pai */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� � esquerda
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

   } /* Fim fun��o: ARV Ir para n� � esquerda */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� � direita
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
   } /* Fim fun��o: ARV Ir para n� � direita */

/***************************************************************************
*
*  Fun��o: ARV Obter valor corrente
*  ****/

   ARV_tpCondRet ARV_ObterValorCorr(void* arvore, char * ValorParm ){
	ARV_tpArvore pArvore = (ARV_tpArvore) arvore;
      if (!pArvore) return ARV_CondRetArvoreNaoExiste;
	if (!pArvore->pNoRaiz) return ARV_CondRetArvoreVazia;
	if (!pArvore->pNoCorr) return ARV_CondRetErroEstrutura;
      * ValorParm = pArvore->pNoCorr->Valor;
      return ARV_CondRetOK ;
   } /* Fim fun��o: ARV Obter valor corrente */


/***************************************************************************
*
*  Fun��o: ARV MarcarVisitado
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
*  Fun��o: ARV Exibe arvore.
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
	//modos de visita sao absolutamente desnecessarios ao percorrer a arvore, 
	//sao menos eficientes, requerem a colaboracao do resto do codigo inteiro para funcionar, e estao sujeitos a erros.
	//originalmente haviamos tentado utiliza-los aqui, mas isto seria dor de cabeca desnecessaria.
	//Talvez eles fossem mais uteis se tivessemos modificado o codigo para que quando se saisse de um no, o seu modo de visita voltasse
	//ao modo padrao ou ao seu modo original antes da visita. Alem disto terianos que ter uma funcao de acesso para retornar o valor do ModoVisita
	//do no corrente. MAS NOTE QUE NADA DISSO FAZIA PARTE DA ESPECIFICACAO ORIGINAL.
	if(pCorr->pNoPai && pCorr->pNoPai->pNoDir == pCorr) printf("%c ", pCorr->Valor);
	else printf("( %c ", pCorr->Valor);

	if(pCorr->pNoEsq){
		pArvore->pNoCorr = pCorr->pNoEsq;
		pArvore->pNoCorr->ModoVisita = ARV_ModoParaEsq;
		ARV_ExibirArvore(arvore);
	} 
	
	if(pCorr->pNoDir){
		pArvore->pNoCorr = pCorr->pNoDir;
		pArvore->pNoCorr->ModoVisita = ARV_ModoParaDir;
		ARV_ExibirArvore(arvore);
	}else printf(") ");
   }


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: ARV Criar n� da �rvore
*
*  $FV Valor retornado
*     Ponteiro para o n� criado.
*     Ser� NULL caso a mem�ria tenha se esgotado.
*     Os ponteiros do n� criado estar�o nulos e o valor � igual ao do
*     par�metro.
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

   } /* Fim fun��o: ARV Criar n� da �rvore */


/***********************************************************************
*
*  $FC Fun��o: ARV Criar n� raiz da �rvore
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

   } /* Fim fun��o: ARV Criar n� raiz da �rvore */


/***********************************************************************
*
*  $FC Fun��o: ARV Destruir a estrutura da �rvore
*
*
***********************************************************************/

   void DestroiArvore(tpNoArvore * pNo){
	if(!pNo) return;
      DestroiArvore(pNo->pNoEsq);
      DestroiArvore(pNo->pNoDir);
      free(pNo);
   } /* Fim fun��o: ARV Destruir a estrutura da �rvore */

/********** Fim do m�dulo de implementa��o: M�dulo �rvore **********/

