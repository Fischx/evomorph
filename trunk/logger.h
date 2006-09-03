#include "genotype.h"
#include "vector3.h"
#include "vector2.h"

#include <stdio.h>

class cLoader {
  
 private:
  FILE* m_file;

  morf_node* LoadNode ();

 public:
  cLoader (char* file_name);
  virtual ~cLoader ();
  
  morf_node* Load ();

};

class cLogger {
  
 private:
  FILE* m_file;
  void SaveNode (morf_node* node);

 public:
  cLogger (char* file_name);
  virtual ~cLogger ();

  void Save (morf_node* node, double fitness);

};
