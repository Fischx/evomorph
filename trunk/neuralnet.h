/**
 * Alterações:
 * 2000     - Criado por Adrian Perez
 * Jun 2006 - Alterado por Alan Godoy Souza Mello.
 */

#ifndef _NEURALNET_H
#define _NEURALNET_H

#include <string>
#include <vector>
#include <map>


/* number of input layer nodes */
#define INPUT_LAYER 7

/* number of output layer nodes */
#define OUTPUT_LAYER 2

/* number of middle layer nodes */
#define MIDDLE_LAYER 10

/* number of middle layers */
#define N_MIDDLE_LAYERS 5

/* number of connections in the neural netkwork*/
#define NN_SIZE (((INPUT_LAYER + OUTPUT_LAYER) * MIDDLE_LAYER) + \
                (MIDDLE_LAYER * MIDDLE_LAYER) * (N_MIDDLE_LAYERS - 1) + \
                (MIDDLE_LAYER * OUTPUT_LAYER))

/* number of layers in the ann */
#define N_LAYERS (N_MIDDLE_LAYERS + 2) 



using namespace std;

class cNeuralNet {

 private:
  double m_feedback[OUTPUT_LAYER];
  void Clear();
  
 protected:
  class cNeuron;
  class cSynapse;
  
 public:
  void Run( double input[] , double output[]);

  cNeuralNet( double genotype[NN_SIZE]);
  virtual ~cNeuralNet();

  
 protected:
  
  class cNeuron	{
  public:
    void Clear();
    virtual void Run();
    void AddOutSynapse( cNeuron* target, double synapseWeight );
    void SendInput( double in );
    double GetTotal() const;
    virtual ~cNeuron();
    
  protected:
    
    // Computes the output function given the total.
    virtual double CalcOutput();
    
    double	m_total;  // total of the summed inputs up to this point
    
    vector< cSynapse >	m_outSynapse;
    
  };
  
  class cSynapse {
    cNeuron*	m_pSrc;
    cNeuron*	m_pDest;
    double	m_weight;
    
  public:
    
    cSynapse( cNeuron* pSrc, cNeuron* pDst, double weight );
    
    void Fire( double amount );
  };
  
  friend class cNeuron;
  
  vector< vector< cNeuron* > > m_neuron;
  
};


inline double cNeuralNet::cNeuron::GetTotal() const {
  return m_total;
}

#endif // _NEURALNET_H
