struct Rtree
{
    struct Node *root;
};

struct Rtree* create_rtree ()
{
    struct Rtree *t;
    t = (struct Rtree*)malloc(sizeof(struct Rtree));
    assert(t);
    t->root = create_node(1);
    return t;
}

void destroy_rtree(struct Rtree *t)
{
    if (t)
        delete_node(t->root);
}




/*el nodo a es el nodo sobrecargado y el nodo b recibira la mitad de sus elementos,
se debe actualizar los bboxes de ambos nodos con 'updatebox'*/
void split(struct Node *a,struct Node* b)
{
    pick_seed(a);
    //int i;
    //int j;/*
    if(a->leaf)
    { 
        struct Node_h *p = (struct Node_h*)(a->my_nodes);
        struct Node *tmp = create_node(a->leaf);
        insert_node(b,p->values[M_]);
        p->values[M_] = NULL;
        a->size -= 1;
        translate_node(a,tmp); 
        updatebox(a);
        updatebox(b);
        updatebox(tmp);
        while (a->size < m_)
        {
            pick_next(tmp,a);
            pick_next(tmp,b);
        }
        while (tmp->size > 0)
            pick_next_xor(tmp,a,b);
        delete_node(tmp);      
    }
    else
    {
        struct Node_nh *p = (struct Node_nh*)(a->my_nodes);
        struct Node *tmp = create_node(a->leaf);       
        insert_node(b,p->values[M_]);
        p->values[M_] = NULL;
        a->size -= 1;
        translate_node(a,tmp); 
        updatebox(a);
        updatebox(b);
        updatebox(tmp);
        while (a->size < m_)
        {
            pick_next(tmp,a);
            pick_next(tmp,b);
        }
        while (tmp->size > 0)
            pick_next_xor(tmp,a,b);
        delete_node(tmp);
    }
    
    /*
    printf("%d \n",b->size);
    for (i=0 ; i<b->size ; i++)
    {
        for (j=0 ; j<Dim ; j++)
        {
            printf("%d,", ((struct Node_h*)b->my_nodes)->values[i]->values[j]);
        }
        printf(" - ");
    }
    printf("\n");
    printf("%d \n",a->size);
    for (i=0 ; i<a->size ; i++)
    {
        for (j=0;j<Dim;j++)
        {
            printf("%d,", ((struct Node_h*)a->my_nodes)->values[i]->values[j]);
        }
        printf(" - ");
    }
    printf("\n ================================================================== \n");*/
}

//crear nuevo root y agregar p y q, y actualizar bbox del nuevo root
void create_newroot(struct Rtree *t,struct Node* q)
{
    struct Node* p = t->root;
    struct Node *n;
    n = create_node(0);
    t->root = n;
    insert_node(n,p);
    insert_node(n,q);
    updatebox(n);
}

//insercion en el arbol desde la raiz
void insert_tree(struct Rtree *t, data_type *d)
{
    //struct Heap *my_heap;   //almace los nodos no hojas desde el root hasta el destino a insertar
    
    struct Node *p;
    struct Node *q = NULL;
    struct Tuple *r = create_tuple(d);
/*    if (find(t->root,r))
    {
        delete_tuple(r);
        return;
    }*/
    p = choose_leaf(t->root,r); 
    insert_node(p,r);       //p es nodo hoja

    int key = 0; //nos dice si hubo un split anterior
    
    while (p != NULL)
    {
        if (key)
            insert_node(p,q);
            
        if (p->size > M_)
        {   
            q = create_node(p->leaf);
            split(p,q);
            key = 1;
        }
        else
        {
            updateboxtuple(p,r);
            key = 0;
        }        
        p = p->father;
    }
       
    if (key)
        create_newroot(t,q);
}


