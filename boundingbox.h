
struct Bounding_box
{
    data_type min_boundary[Dim];
    data_type max_boundary[Dim];
};


struct Bounding_box * create_bbox ()
{
    struct Bounding_box *b;
    b = (struct Bounding_box*)malloc(sizeof(struct Bounding_box));
    assert(b);
    int i;
    for (i=0 ; i<Dim ;i++)
    {
        b->min_boundary[i] = 0;
        b->max_boundary[i] = 0;
    }
    return b;
}

void delete_bbox (struct Bounding_box *b)
{
    if( b != NULL )
        free(b);
}


int within_bounding (struct Tuple *a, struct Bounding_box *b)
{
    int i;
    for (i=0 ; i< Dim; i++)
        if ( (a->values[i] < b->min_boundary[i]) || (a->values[i] > b->max_boundary[i]) )
            return 0;
    return 1;
}


data_type wide_area(struct Bounding_box *b,struct Tuple *d) 
{
    if(within_bounding(d,b))
        return (data_type)0.0;
    data_type initial_area = (data_type)1.0;
    data_type final_area = (data_type)1.0;
    int i;       
    for (i=0 ; i<Dim ; i++)
    {
        initial_area *= b->max_boundary[i] - b->min_boundary[i];
        if (b->min_boundary[i] > d->values[i])
            final_area *= b->max_boundary[i]-d->values[i];
        else if (b->max_boundary[i] < d->values[i])
            final_area *= d->values[i]-b->min_boundary[i];
        else
            final_area *= b->max_boundary[i]-b->min_boundary[i];
    } 
    return final_area - initial_area;
}

data_type area_bb(struct Bounding_box *a)
{
    int i;
    data_type result = (data_type)1.0;
    for (i=0 ; i<Dim ;i ++)
        result *= a->max_boundary[i]-a->min_boundary[i];
    return result;
}

data_type area_between_bb(struct Bounding_box *a, struct Bounding_box *b)
{
    data_type mi[Dim];
    data_type ma[Dim];
    data_type result = (data_type)1.0;
    int i;
    for (i=0 ; i<Dim ; i++)
    {
        if (a->min_boundary[i] < b->min_boundary[i])
            mi[i] = a->min_boundary[i];
        else
            mi[i] = b->min_boundary[i];
        if (a->max_boundary[i] > b->max_boundary[i])
            ma[i] = a->max_boundary[i];
        else
            ma[i] = b->max_boundary[i];
        result *= ma[i]-mi[i]; 
    }
    return result - area_bb(a) - area_bb(b);
}


