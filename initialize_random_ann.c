#define WEIGHT_INIT_MAX 100.0
#define VARIANCE_INIT_MAX 100.0

/* Initializes randomly an ann (varies from -?_INIT_MAX to +?_INIT_MAX) */
void initialize(ann_genotype* ann)
{
  int i;
  double isnegativ; /* decides whether the generated random number is negativ */
  for(i=0; i < N_NEURAL; i++) {
    ann->weight[i] = (WEIGHT_INIT_MAX*rand()/(RAND_MAX+1.0));
    ann->variance[i] = (VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));
    /* Should the weight be negativ? */
    isnegativ = (rand()/(RAND_MAX+1.0));
    if(isnegativ > 0.5) {
      ann->weight[i] = ann->weight[i] * -1;
    }
    /* Should the variance be negativ? */
    isnegativ = (rand()/(RAND_MAX+1.0));
    if(isnegativ > 0.5)
      ann->variance[i] = ann->variance[i] * -1;
  }
}
