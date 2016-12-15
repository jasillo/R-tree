enum type{type_int=0,type_bool=1,type_char=2,type_float=3,type_double=4,type_string=5};
typedef struct Lnode{void* L_data;struct Lnode* _next;enum type type;};
typedef struct list{struct Lnode* root; struct Lnode** last;};

struct list* builder()
{
    struct list* build_list=(struct list*) malloc(sizeof(struct list));
    build_list->root=NULL;
    build_list->last=&(build_list->root);
    return build_list;
};

bool push_element(struct list* list_, void* data,enum type type_)
{
    struct Lnode** new_node=list_->last;
    (*new_node) = (struct Lnode*) malloc(sizeof(struct Lnode));
    (*new_node)->L_data = data;
    (*new_node)->type=type_;
    (*new_node)->_next=NULL;
    list_->last=&((*new_node)->_next);
    return 1;
}

void print_list(struct list* aux_h)
{
    struct Lnode* aux_list= aux_h->root;
    while(aux_list!=NULL)
    {
        switch(aux_list->type)
        {
            case type_int:
                printf("%d -> ",aux_list->L_data);
                break;
            case type_char:
                printf("%c -> ",aux_list->L_data);
                break;
            case type_float:
                printf("%f -> ",aux_list->L_data);
                break;
            case type_double:
                printf("%d -> ",aux_list->L_data);
                break;
            case type_string:
                printf("%s -> ",aux_list->L_data);
                break;
            default:
                printf("Dont type -> ");
        }
        aux_list = aux_list->_next;
    }
}


struct Heap
{

};

//devuelve el ultimo dato y lo borra de la pila
struct Node* pop(struct Heap* h)
{
    return NULL;
}

void push(struct Heap* h, struct Node* n)
{

}

