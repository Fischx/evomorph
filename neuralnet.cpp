/**
 * Rede neural.
 *
 * Alterações:
 * 2000     - Criado por Adrian Perez
 * Jun 2006 - Alterado por Alan Godoy Souza Mello.
 */

using namespace std;

#include <math.h> // for atof, exp
#include "neuralnet.h"


#include <stdio.h>

/**
 * Carrega e cria a rede neural, a partir de um genótipo.
 */
cNeuralNet::cNeuralNet( double genotype[NN_SIZE]) {

  for (int i = 0; i < OUTPUT_LAYER; i++) {
    m_feedback[i] = 0;
  }

  int layers[N_LAYERS];
  
  layers[0] = INPUT_LAYER + OUTPUT_LAYER;

  for (int i = 1; i <= N_MIDDLE_LAYERS; i++) {
      layers[i] = MIDDLE_LAYER;
  }

  layers[N_LAYERS - 1] = OUTPUT_LAYER;
  
  int conta = 0;

  for (int i = 0; i < N_LAYERS; i++) {
    vector< cNeuralNet::cNeuron* > garbage;
    m_neuron.push_back( garbage );
    
    for (int j = 0; j < layers[i]; j++) {
      cNeuron* pNew = new cNeuron();
      m_neuron[i].push_back( pNew );
    }
  }
  
  int layer = 0;
  int neuron = 0;
  int synapse = 0;
  
  for (; conta < NN_SIZE; conta++) {
    if (synapse == layers[layer + 1]) {
      neuron++;
      synapse = 0;

      if (neuron == layers[layer]) {
	layer++;
	neuron = 0;
      }
    }
    
    m_neuron[layer][neuron]->AddOutSynapse(m_neuron[layer + 1][synapse++], genotype[conta]);
  }
  
}

/**
 * Deleta cada neurônio da rede.
 */
cNeuralNet::~cNeuralNet() {
  
  for (int i = 0; i < N_LAYERS; i++) {
    int layerSize = m_neuron[i].size();
    
    for (int j = 0; j < layerSize; j++) {
      delete m_neuron[i][j];
    }
  }
}

/**
 * Limpa os estímulos de cada neurônio em cada camada.
 */
void cNeuralNet::Clear() {

  for (int i = 0; i < N_LAYERS; i++) {
    int layerSize = m_neuron[i].size();
    
    for (int j = 0; j < layerSize; j++) {
      m_neuron[i][j]->Clear();
    }
  }
}

/**
 * Roda a rede neural, ativando cada neurônio em cada camada.
 */
void cNeuralNet::Run( double input[INPUT_LAYER] , double output[OUTPUT_LAYER]) {

  Clear();

  for (int i = 0; i < INPUT_LAYER; i++) {
    m_neuron[0][i]->SendInput(input[i]);
  }

  for (int i = 0; i < OUTPUT_LAYER; i++) {
    m_neuron[0][INPUT_LAYER + i]->SendInput(m_feedback[i]);
  }
  
  for (int i = 0; i < N_LAYERS; i++) {
    int layerSize = m_neuron[i].size();
    
    for (int j = 0; j < layerSize; j++) {
      m_neuron[i][j]->Run();
    }
  }

  for (int i = 0; i < OUTPUT_LAYER; i++) {
    m_feedback[i] = output[i] = m_neuron[N_LAYERS-1][i]->GetTotal();
  }

}

/**
 * Limpa o total de estimulo calculado para o neuronio.
 */
void cNeuralNet::cNeuron::Clear() {
  m_total = 0;
}

cNeuralNet::cNeuron::~cNeuron(){
}

/**
 * Envia o estímulo para todas as sinapses.
 */
void cNeuralNet::cNeuron::Run() {
  double output = CalcOutput();
  int size = m_outSynapse.size();
  
  for (int i = 0; i < size; i++) {
    m_outSynapse[i].Fire(output);
  }
}

/**
 * Calcula o estímulo retornado pelo neurônio. 
 */
double cNeuralNet::cNeuron::CalcOutput() {
  return(tanh(m_total));
}

/**
 * Adiciona a entrada ao estímulo do neurônio.
 */
void cNeuralNet::cNeuron::SendInput(double in) {
  m_total += in;
}

/**
 * Cria uma sinapse de saída do neurônio.
 */
void cNeuralNet::cNeuron::AddOutSynapse(cNeuron* target, double synapseWeight) {
  m_outSynapse.push_back(cSynapse(this, target, synapseWeight));
}

/**
 * Cria uma sinapse entre dois neurônios.
 */
cNeuralNet::cSynapse::cSynapse(cNeuron *pSrc, cNeuron *pDest, double weight)
  : m_pSrc(pSrc) , m_pDest(pDest) , m_weight(weight) {
}

/**
 * Envia o sinal, multiplicado pelo peso da aresta.
 */
void cNeuralNet::cSynapse::Fire(double amount) {
  m_pDest->SendInput(amount * m_weight);
}
