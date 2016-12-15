// calculo de la distancia para valores numericos, vectores
dist_type distance(struct Tuple *a, struct Tuple *b) 
{
    int i;
    dist_type sum = (dist_type) 0;
    for (i=0 ; i<Dim ; i++)    
        sum += pow((dist_type)(a->values[i] - b->values[i]), 2);
    sum = sqrt(sum);
    return sum;
}
