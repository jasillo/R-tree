struct Tuple
{
    data_type values[Dim];
};

// devuelve un puntero a una estructura tupla
struct Tuple * create_tuple (data_type *a)
{
    struct Tuple *t;
    t = (struct Tuple*)malloc(sizeof(struct Tuple));
    assert(t);
    int i;
    for (i=0 ; i<Dim ;i++)
    {
        t->values[i]=a[i];
    }

    return t;
}

void delete_tuple(struct Tuple *a)
{
    if( a != NULL )
        free(a);
}

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

data_type area_between_tuples(struct Tuple *a,struct Tuple *b)
{
    data_type result = (data_type)1.0;
    int i;
    for (i=0 ; i<Dim ; i++)
    {
        if (a->values[i] < b->values[i])
            result *= b->values[i] - a->values[i];
        else
            result *= a->values[i] - b->values[i];         
    }
    return result;
}

int min_list (data_type *list, int size)
{
    int i;
    int j=0;
    for (i=1 ; i<size ; i++)
    {
        if (list[j] > list[i])
            j=i;
    }    
    return j;
}

