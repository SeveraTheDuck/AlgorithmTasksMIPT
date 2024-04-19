enum int_cmp_status
{
    INT_CMP_LESS    = -1,
    INT_CMP_EQUAL   =  0,
    INT_CMP_GREATER =  1
};



int CompareInt (void* a, void* b)
{
    assert (a);
    assert (b);

    int a_value = *(int*)a;
    int b_value = *(int*)b;

    if (a_value < b_value) return INT_CMP_LESS;
    if (a_value > b_value) return INT_CMP_GREATER;
    return INT_CMP_EQUAL;
}
