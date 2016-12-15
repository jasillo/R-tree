struct Node;
void delete_node (struct Node *a);

//=====================================================================
struct Node_h                   //datos nodo hoja
{
    struct Tuple* values[M_+1];
};

struct Node_h* create_nodeh()
{
    struct Node_h *n;
    n = (struct Node_h*)malloc(sizeof(struct Node_h));
    assert(n);
    int i;
    for (i=0 ; i <= M_ ; i++)
        n->values[i] = NULL;
    return n;
}

void delete_nodeh(struct Node_h* n)
{
    if (n)
    {
        int i;
        for (i=0 ; i<=M_ ; i++)
            delete_tuple(n->values[i]);
        free(n);
    }
}

//=============================================================================
struct Node_nh                   //datos nodo no hoja
{
    struct Node *values[M_+1];
};

struct Node_nh* create_nodenh()
{
    struct Node_nh *n;
    n = (struct Node_nh*)malloc(sizeof(struct Node_nh));
    assert(n);
    int i;
    for (i=0 ; i <= M_ ; i++)
        n->values[i] = NULL;
    return n;
}

//borrado es en cadena
void delete_nodenh(struct Node_nh* n)
{
    if (n)
    {
        int i;
        for (i=0 ; i<=M_ ; i++)
            delete_node(n->values[i]);
        free(n);
    }
}


//=============================================================================
struct Node
{
    int leaf;   //1 si es hoja
    struct Bounding_box *my_box;
    int size;
    void *my_nodes;
    struct Node *father;
};

//inicializar nodo
struct Node * create_node(int l)
{
    struct Node *n;
    n = (struct Node*)malloc(sizeof(struct Node));
    assert(n);
    if (l)
        n->my_nodes = create_nodeh();
    else
        n->my_nodes = create_nodenh();
    n->leaf = l;
    n->my_box = create_bbox();
    n->size = 0;
    n->father = NULL;
    return n;
};



void delete_node (struct Node *a)
{
    if (a)
    {
        a->father = NULL;
        delete_bbox(a->my_box);
        if (a->leaf)
            delete_nodeh((struct Node_h*)a->my_nodes);
        else
            delete_nodenh((struct Node_nh*)a->my_nodes);
        free(a);
    }
}

//se puede insert un puntero a un nodo o puntero a tupla, depende del tipo de nodo (nodo->leaf?)
void insert_node (struct Node *n,void *d)
{  
    if (n->leaf) //entonces d es un tupla
    {
        struct Tuple *p;
        p = (struct Tuple*)d;
        ((struct Node_h*)(n->my_nodes))->values[n->size] = p;

    }else{ //entonces d es un nodo
        struct Node *p;
        p = (struct Node*)d;
        ((struct Node_nh*)(n->my_nodes))->values[n->size] = p;
        p->father = n;
    }
    n->size +=1;
}

void swap (struct Node *n, int pos1, int pos2)
{
    if (n->leaf)
    {
        struct Tuple* aux;
        struct Node_h* p = (struct Node_h*)(n->my_nodes);
        aux = p->values[pos1];
        p->values[pos1] = p->values[pos2];
        p->values[pos2] = aux;
    }else{
        struct Node* aux;
        struct Node_nh* p = (struct Node_nh*)(n->my_nodes);
        aux = p->values[pos1];
        p->values[pos1] = p->values[pos2];
        p->values[pos2] = aux;
    }
}


//actualizar tamaño de los limites
void updatebox(struct Node *n)
{
    struct Bounding_box *bb = n->my_box;
    int i,j;
    if (n->size == 0)
        return;
    if (n->leaf)
    {
        struct Node_h* nod = (struct Node_h*)n->my_nodes;
        struct Tuple* tup = nod->values[0];        
        for(i=0 ; i<Dim ; ++i)
        {
            bb->min_boundary[i] = tup->values[i];
            bb->max_boundary[i] = tup->values[i];
        }
        
        for(i=1 ; i<n->size ; ++i)
        {
            tup = nod->values[i];
            for(j=0 ; j<Dim ; ++j)
            {
                if(bb->min_boundary[j] > tup->values[j])
                    bb->min_boundary[j] = tup->values[j];
                else if (bb->max_boundary[j] < tup->values[j])
                    bb->max_boundary[j] = tup->values[j];
            }
        }    
    }
    else
    {
        struct Node_nh* nod = (struct Node_nh*)n->my_nodes;
        struct Bounding_box *box = nod->values[0]->my_box;
        for(i=0 ; i<Dim ; ++i)
        {
            bb->min_boundary[i] = box->min_boundary[i];
            bb->max_boundary[i] = box->max_boundary[i];
        }
        
        for(i=1 ; i<n->size ; ++i)
        {
            box = nod->values[i]->my_box;
            for(j=0 ; j<Dim ; ++j)
            {
                if (bb->min_boundary[j] > box->min_boundary[j])
                    bb->min_boundary[j] = box->min_boundary[j];
                if (bb->max_boundary[j] > box->max_boundary[j])
                    bb->max_boundary[j] = box->max_boundary[j];
            }
        }
    }  
}

//actualizar tamaño de los limites con rsepecto a un nodo
void updateboxtuple(struct Node *n, struct Tuple *a)
{
    if (n->size == 0)
        return;
    struct Bounding_box *box = n->my_box;
    if (within_bounding(a,box))
        return;
    int i;
    for(i=0;i<Dim;++i)
    {
        if(a->values[i] < box->min_boundary[i])
            box->min_boundary[i] = a->values[i];
        else if(a->values[i] > box->max_boundary[i])
            box->max_boundary[i] = a->values[i];
    }
}

struct Node * choose_leaf(struct Node * n,struct Tuple *t)
{
    struct Node *p = n;
    struct Node_nh *q;
    int i,pos;  // pos es la posicion de nodo elejido
    int s;  // s numero de elemntos de ese nodo
    dist_type extra_area1,extra_area2; //expansion del area
    while(!p->leaf)
    {
        q = ((struct Node_nh*)(p->my_nodes));
        s = M_+1;        
        extra_area1 = wide_area(q->values[0]->my_box,t);
        pos = 0;
        s = q->values[0]->size;       
        for (i=1 ; i < p->size ; i++) //recorrido por todos los hijos
        {
            extra_area2 = wide_area(q->values[i]->my_box,t);
            if (extra_area1 > extra_area2 || (extra_area1 == extra_area2 && s > q->values[i]->size))
            {
                extra_area1 = extra_area2;
                pos = i;
                s = q->values[i]->size;
            }            
        }        
        p = q->values[pos];
    }
    return p;
}

void pick_seed(struct Node * n)
{
    int i,j;
    int k=0,l=0;    
    data_type d = (data_type)0.0;
    data_type d_aux;
    
    if (n->leaf)
    {
        struct Node_h *p = (struct Node_h*) n->my_nodes;        
        for (i=0 ; i<M_ ; i++)
        {
            for (j=i ; j<=M_ ; j++)
            {
                d_aux = area_between_tuples(p->values[i],p->values[j]);
                if ( d < d_aux )
                {
                    d = d_aux;
                    k = i;
                    l = j;
                }
            }
        }
        swap(n,0,k);
        swap(n,M_,l);   
    }else{
        struct Node_nh *p = (struct Node_nh*) n->my_nodes;        
        for (i=0 ; i<M_ ; i++)
        {
            for (j=i ; j<=M_ ; j++)
            {
                d_aux = area_between_bb(p->values[i]->my_box,p->values[j]->my_box);
                if ( d < d_aux )
                {
                    d = d_aux;
                    k = i;
                    l = j;
                }
            }
        }
        swap(n,0,k);
        swap(n,M_,l); 
    }   
}

void pick_next(struct Node *a,struct Node* b)
{
    int i;
    data_type weight[a->size];
    int pos=0;
    if (a->leaf)
    {
        struct Node_h *p_a = (struct Node_h*) a->my_nodes;
        struct Node_h *p_b = (struct Node_h*) b->my_nodes;
        for (i=0 ; i<a->size ; i++)
            weight[i] = area_between_tuples(p_b->values[0], p_a->values[i]);
        pos = min_list(weight,a->size);
        swap(a,pos,a->size-1);
        insert_node(b,p_a->values[a->size-1]);
        p_a->values[a->size-1] = NULL;        
        a->size -= 1;
        updatebox(a);
        updatebox(b);
    }
    else
    {
        struct Node_nh *p_a = (struct Node_nh*) a->my_nodes;
        struct Node_nh *p_b = (struct Node_nh*) b->my_nodes;
        for (i=0 ; i<a->size ; i++)
            weight[i] = area_between_bb(p_b->values[0]->my_box, p_a->values[i]->my_box);
        pos = min_list(weight,a->size);
        swap(a,pos,a->size-1);
        insert_node(b,p_a->values[a->size-1]);
        p_a->values[a->size-1] = NULL; 
        a->size -= 1;
        updatebox(a);
        updatebox(b);
    }    
}

//'a' pierde un nodo y se lo da a 'b' o 'c'
void pick_next_xor(struct Node *a,struct Node* b,struct Node* c)
{
    data_type weight1;
    data_type weight2;
    if (a->leaf)
    {
        struct Node_h *p_a = (struct Node_h*) a->my_nodes;
        struct Node_h *p_b = (struct Node_h*) b->my_nodes;
        struct Node_h *p_c = (struct Node_h*) c->my_nodes;
        weight1 = area_between_tuples(p_a->values[a->size-1], p_b->values[0]);
        weight2 = area_between_tuples(p_a->values[a->size-1], p_c->values[0]);
        if (weight1 < weight2)
        {
            insert_node(b,p_a->values[a->size-1]);
            updatebox(b);
        }
        else
        {
            insert_node(c,p_a->values[a->size-1]);
            updatebox(c);
        }
        p_a->values[a->size-1] = NULL;
    }
    else
    {
        struct Node_nh *p_a = (struct Node_nh*) a->my_nodes;
        struct Node_nh *p_b = (struct Node_nh*) b->my_nodes;
        struct Node_nh *p_c = (struct Node_nh*) c->my_nodes;
        weight1 = area_between_bb(p_a->values[a->size-1]->my_box, p_b->values[0]->my_box);
        weight2 = area_between_bb(p_a->values[a->size-1]->my_box, p_c->values[0]->my_box);
        if (weight1 < weight2)
        {
            insert_node(b,p_a->values[a->size-1]);
            updatebox(b);
        }
        else
        {
            insert_node(c,p_a->values[a->size-1]);
            updatebox(c);
        }
        p_a->values[a->size-1] = NULL;
    } 
    a->size -= 1;
    updatebox(a); 
}

void translate_node (struct Node *a,struct Node* b)
{
    int i;
    if (a->leaf)
    {
        struct Node_h *p_a = (struct Node_h*) a->my_nodes;
        struct Node_h *p_b = (struct Node_h*) b->my_nodes;
        for (i=1 ; i<a->size; i++)
        {
            p_b->values[i-1] = p_a->values[i];
            p_a->values[i] = NULL;
        }
    }
    else
    {
        struct Node_nh *p_a = (struct Node_nh*) a->my_nodes;
        struct Node_nh *p_b = (struct Node_nh*) b->my_nodes;
        for (i=1 ; i<a->size; i++)
        {
            p_b->values[i-1] = p_a->values[i];
            p_a->values[i] = NULL;
        }
    }
    b->size = a->size -1;
    a->size = 1;   
}   

