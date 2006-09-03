#include "logger.h"

cLoader::cLoader (char* file_name) {
  
  if ((m_file = fopen(file_name, "r")) == NULL) {
    exit(1);
  }
  
}

cLoader::~cLoader () {

  fclose(m_file);

}

morf_node* cLoader::Load () {

  int id;

  fscanf(m_file, "%d", &id);

  double fitness;

  fscanf(m_file, "%lf", &fitness);

  return LoadNode();

}

morf_node* cLoader::LoadNode () {

  double weight[N_NEURAL];
  ann_genotype ann;
  
  for (int i = 0; i < N_NEURAL; i++) {
    fscanf(m_file, "%lf", &(ann.weight[i]));
  }
  
  float float_vector[9];
  
  for (int i = 0; i < 9; i++) {
    fscanf(m_file, "%f", &float_vector[i]);
  }
  
  char char_vector[2];
  
  for (int i = 0; i < 2; i++) {
    fscanf(m_file, "%c", &char_vector[i]);
  }
  
  vector< morf_node* > subnodes;
  
  for (int i = 0; i < char_vector[1]; i++) {
    subnodes.push_back(LoadNode());
  }
  
  morf_node* node = new morf_node(vector3(float_vector[0], float_vector[1], float_vector[2]),
				  vector2(float_vector[3], float_vector[4]),
				  char_vector[0],
				  vector3(float_vector[5], float_vector[6], float_vector[7]),
				  float_vector[8],
				  ann);
  
  node->subnodes = subnodes;
  
  return(node);
  
}

cLogger::cLogger (char* file_name) {
  
  if ((m_file = fopen(file_name, "a")) == NULL) {
    exit(1);
  }
  
}

cLogger::~cLogger () {

  fclose(m_file);
  
}

void cLogger::Save (morf_node* root, double fitness) {
  
  static int id = 0;

  fprintf(m_file, "%d %lf ", id++, fitness);
  
  SaveNode(root);

  fprintf(m_file, "\n");
  
}

void cLogger::SaveNode (morf_node* root) {

  for (int i = 0; i < N_NEURAL; i++) {
    fprintf(m_file, "%lf ", root->ann.weight[i]);
  }
  
  fprintf(m_file, "%f %f %f %f %f %f %f %f %f ",
	  root->size.x, root->size.y, root->size.z,
	  root->pos.x, root->pos.y, 
	  root->rot.x, root->rot.y, root->rot.z,
	  root->density);
  
  char size = root->subnodes.size();
  
  fprintf(m_file, "%c %c ", root->face, size);
  
  for (int i = 0; i < size; i++) {
    SaveNode(root->subnodes[i]);
  }

}
