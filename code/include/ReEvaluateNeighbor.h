#ifndef REEVALUATENEIGHBOR_H_INCLUDED
#define REEVALUATENEIGHBOR_H_INCLUDED

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
double reEvaluate(double h, double U_node, double Uh, double Uv, double F_node)
{
    double q = h/F_node;
    double U_new = U_node;
    double V = 0;
    double U1,U2= 0;

    U1 = Uh + Uv;    U2 = Uh - Uv;

    if (fabs(U2) <= q){
        V = 0.5*( U1 + sqrt( 2*q*q - U2*U2 ) );
        if(V < U_node){
            U_new = V;
        }
    }else{
        V = min(Uh, Uv) + q;
        if(V < U_node){
            U_new = V;
        }
    }
    return(U_new);
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void reEvaluateNeighbor(int node, int N, double h, double *U, int *status, struct MinHeap *minHeap, double F_node){

    if(status[node] != 2){

        double Uh = min( U[node-1], U[node+1] );
        double Uv = min( U[node-N], U[node+N] );
        U[node] = reEvaluate(h, U[node], Uh, Uv, F_node);

        if(status[node] == 0){

            minHeap->array[minHeap->hsize] = newMinHeapNode(node, DBL_MAX);
            minHeap->pos[node] = minHeap->hsize;
            minHeap->hsize = minHeap->hsize + 1;

            decreaseKey(minHeap, node, U[node]);

            status[node] = 1;

        }else{

            decreaseKey(minHeap, node, U[node]);
        }
    }    // End of neighbor
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // REEVALUATENEIGHBOR_H_INCLUDED
